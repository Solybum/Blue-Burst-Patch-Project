#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <cstddef>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <assimp/vector2.h>
#include <assimp/color4.h>
#include <assimp/material.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../common.h"
#include "model.h"

// Force images to always have 4 channels
const size_t IMAGE_CHANNEL_COUNT = 4;

uint32_t PackColor(const aiColor4D* color)
{
    const uint32_t a = (uint32_t) (color->a * 255.0) << 24;
    const uint32_t r = (uint32_t) (color->r * 255.0) << 16;
    const uint32_t g = (uint32_t) (color->g * 255.0) << 8;
    const uint32_t b = (uint32_t) (color->b * 255.0);
    return a | r | g | b;
}

IDirect3DTexture8* TextureFromData(int width, int height, stbi_uc* img, bool flipY = true)
{
    // The only 4-component 32-bit format with alpha in d3d8
    auto dstFmt = D3DFMT_A8R8G8B8;

    // Create texture object
    const auto mipCount = 1;
    const auto usageFlags = 0;
    IDirect3DTexture8* texture = nullptr;
    if (FAILED((*d3dDevice)->lpVtbl->CreateTexture(*d3dDevice, width, height, mipCount, usageFlags, dstFmt, D3DPOOL_MANAGED, &texture)))
        throw std::runtime_error("Failed to create texture");
    
    // Lock texture object so that we can write to its buffer
    const auto lockMipLevel = 0;
    D3DLOCKED_RECT lockedRect; // We write to this
    const auto rectToLock = nullptr; // Null means entire texture
    const auto lockFlags = 0;
    if (FAILED(texture->lpVtbl->LockRect(texture, lockMipLevel, &lockedRect, rectToLock, lockFlags)))
        throw std::runtime_error("Failed to lock texture");

    // Copy pixel data from image into texture object
    // We do it row by row because the size of a row might be padded (pitch) in the destination buffer
    auto textureDst = reinterpret_cast<uint8_t*>(lockedRect.pBits);
    auto rowSize = width * IMAGE_CHANNEL_COUNT;
    // Iterate rows in specified order
    for (int y = flipY ? height - 1 : 0,
             end = flipY ? -1 : height,
             step = flipY ? -1 : 1;
         y != end; y += step)
    {
        auto textureSrc = img + y * rowSize;

        // Convert pixels from RGBA to BGRA
        for (auto x = 0; x < rowSize; x += IMAGE_CHANNEL_COUNT)
        {
            textureDst[x + 0] = textureSrc[x + 2]; // B
            textureDst[x + 1] = textureSrc[x + 1]; // G
            textureDst[x + 2] = textureSrc[x + 0]; // R
            textureDst[x + 3] = textureSrc[x + 3]; // A
        }

        // Pitch = row width + padding
        textureDst += lockedRect.Pitch;
    }

    texture->lpVtbl->UnlockRect(texture, lockMipLevel);

    return texture;
}

IDirect3DTexture8* TextureFromFile(const std::string& filename, const std::string& directory, bool flipY = true)
{
    // Read file from FS
    auto path = joinPath({directory, filename});
    
    int width, height, origNumChannels;
    auto img = stbi_load(path.c_str(), &width, &height, &origNumChannels, IMAGE_CHANNEL_COUNT);
    if (img == nullptr)
        throw std::runtime_error("Failed to load image file: " + path);

    return TextureFromData(width, height, img, flipY);
}

IDirect3DTexture8* TextureFromEmbedded(const aiTexture* embedded, bool flipY = true)
{
    // If mHeight == 0 it means data is compressed
    bool compressed = embedded->mHeight == 0;
    // If compressed mWidth is size of buffer
    auto byteSize = compressed ? embedded->mWidth : embedded->mWidth * embedded->mHeight * sizeof(aiTexel);

    int width, height, origNumChannels;
    auto img = stbi_load_from_memory(
        reinterpret_cast<stbi_uc*>(embedded->pcData), byteSize,
        &width, &height, &origNumChannels, IMAGE_CHANNEL_COUNT);
    if (img == nullptr)
        throw std::runtime_error("Failed to load embedded texture");

    return TextureFromData(width, height, img, flipY);
}

Model::Model(const std::string& path)
{
    Assimp::Importer importer;
    // Read file
    auto maybeScene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

    if (maybeScene == nullptr || maybeScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || maybeScene->mRootNode == nullptr)
        throw std::runtime_error(importer.GetErrorString());
    
    // Take ownership of scene
    scene = importer.GetOrphanedScene();
    
    // Save the directory so that we can search for related resources there
    directory = dirname(path);

    // Start processing nodes starting from the root node
    ProcessNode(scene->mRootNode);
}

Model::~Model()
{
    delete scene;
}

void Model::Draw()
{
    for (auto& mesh : meshes)
    {
        mesh.Draw();
    }
}

void Model::ProcessNode(aiNode* node)
{
    // Process meshes in node
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        ProcessMesh(node->mMeshes[i]);
    }

    // Process child nodes
    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i]);
    }
}

void Model::ProcessMesh(size_t meshIndex)
{
    auto mesh = scene->mMeshes[meshIndex];

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    // Get vertices
    for (auto i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = mesh->mVertices[i];

        if (mesh->HasNormals())
        {
            vertex.normal = mesh->mNormals[i];
        }

        if (mesh->HasVertexColors(0))
        {
            vertex.color = PackColor(mesh->mColors[i]);
        }
        else
        {
            vertex.color = 0xffffffff;
        }

        if (mesh->mTextureCoords != nullptr)
        {
            vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.texCoords = aiVector2D(0.0, 0.0);
        }

        vertices.push_back(vertex);
    }

    // Get indices from faces to match D3DPT_TRIANGLELIST
    for (auto i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (auto j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Load textures (only diffuse for now)
    auto material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    meshes.emplace_back(meshIndex, vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* material, aiTextureType textureType)
{
    std::vector<Texture> textures;

    // Load textures
    for (auto i = 0; i < material->GetTextureCount(textureType); i++)
    {
        aiString texturePath;
        material->GetTexture(textureType, i, &texturePath);

        Texture texture;
        texture.path = texturePath.C_Str();
        texture.type = textureType;

        // Asterisk prefix means embedded texture
        if (texture.path[0] == '*')
        {
            texture.object = TextureFromEmbedded(scene->GetEmbeddedTexture(texture.path.c_str()));
        }
        else
        {
            texture.object = TextureFromFile(texture.path, this->directory);
        }

        textures.push_back(texture);
    }

    return textures;
}
