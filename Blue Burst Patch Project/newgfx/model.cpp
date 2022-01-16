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
#include <IL/devil_cpp_wrapper.hpp>
#include "../common.h"
#include "model.h"

uint32_t PackColor(const aiColor4D* color)
{
    const uint32_t a = (uint32_t) (color->a * 255.0) << 24;
    const uint32_t r = (uint32_t) (color->r * 255.0) << 16;
    const uint32_t g = (uint32_t) (color->g * 255.0) << 8;
    const uint32_t b = (uint32_t) (color->b * 255.0);
    return a | r | g | b;
}

IDirect3DTexture8* TextureFromData(size_t width, size_t height, const ilImage& img, bool flipY = false)
{
    img.Bind(); // Activate image for use

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
    auto textureData = reinterpret_cast<uint8_t*>(lockedRect.pBits);
    const auto copyX = 0; // Always start from the beginning of the row
    const auto copyZ = 0;
    const auto copyWidth = width;
    const auto copyHeight = 1; // One row at a time
    const auto copyDepth = 1;
    const auto copyType = IL_UNSIGNED_BYTE;
    // Iterate rows in specified order
    for (int y = flipY ? height - 1 : 0,
             end = flipY ? -1 : height,
             step = flipY ? -1 : 1;
         y != end; y += step)
    {
        // DevIL will convert to the pixel format we want
        if (!ilCopyPixels(copyX, y, copyZ, copyWidth, copyHeight, copyDepth, IL_BGRA, copyType, textureData))
            throw std::runtime_error("Failed to copy pixel data");

        // Pitch = row width + padding
        textureData += lockedRect.Pitch;
    }

    texture->lpVtbl->UnlockRect(texture, lockMipLevel);

    return texture;
}

IDirect3DTexture8* TextureFromFile(const std::string& filename, const std::string& directory, bool flipY = false)
{
    // Read file from FS
    auto path = joinPath({directory, filename});
    auto img = ilImage();
    if (!img.Load(path.c_str()))
        throw std::runtime_error("Failed to load image file: " + path);

    return TextureFromData(img.Width(), img.Height(), img, flipY);
}

IDirect3DTexture8* TextureFromEmbedded(const aiTexture* embedded, bool flipY = false)
{
    auto img = ilImage();
    auto hint = std::string(embedded->achFormatHint);

    // If mHeight == 0 it means data is compressed
    bool compressed = embedded->mHeight == 0;

    ILenum type;
    if (hint == "png")
    {
        type = IL_PNG;
    }
    else if (hint == "jpg")
    {
        type = IL_JPG;
    }
    else
    {
        throw std::runtime_error("Unsupported image format in embedded texture: " + hint);
    }

    // If compressed mWidth is size of buffer
    auto byteSize = compressed ? embedded->mWidth : 0;

    // Put data into image object
    ilLoadL(type, (void*) embedded->pcData, byteSize);

    return TextureFromData(img.Width(), img.Height(), img, flipY);
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
        ProcessMesh(i);
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
