#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>
#include <d3d8.h>
#include <assimp/vector3.h>
#include <assimp/vector2.h>
#include <assimp/matrix4x4.h>
#include <assimp/material.h>

struct Vertex
{
    aiVector3D position;
    aiVector3D normal;
    uint32_t color;
    aiVector2D texCoords;
};

// Store bone data for vertices separately because we don't want to upload it to the GPU
struct VertexBoneData
{
    size_t boneId;
    float boneWeight;
};

struct Texture
{
    std::string path;
    aiTextureType type;
    IDirect3DTexture8* object;
};

class Mesh
{
private:
    const size_t sceneMeshIndex;
    const std::vector<Vertex> untransformedVertices;
    const std::vector<uint32_t> indices;
    const std::vector<Texture> textures;
    std::vector<std::vector<VertexBoneData>> vertexBoneMap;

    IDirect3DVertexBuffer8* vertexBuffer;
    IDirect3DIndexBuffer8* indexBuffer;

    enum ShadingMode
    {
        Normal,
        Transparent,
        Shadow
    };

    ShadingMode shadingMode;

public:
    Mesh(const size_t sceneMeshIndex,
         const std::vector<Vertex>& vertices,
         const std::vector<uint32_t>& indices,
         const std::vector<Texture>& textures,
         const std::vector<std::vector<VertexBoneData>>& boneData);
    Mesh(const size_t sceneMeshIndex,
         const std::vector<Vertex>& vertices,
         const std::vector<uint32_t>& indices,
         const std::vector<Texture>& textures);
    void SetVertexBoneMap(const std::vector<std::vector<VertexBoneData>>& boneData);
    void Draw();
    void ApplyBoneTransformations(const std::vector<aiMatrix4x4>& transforms);
    size_t VertexCount() const;
    size_t TextureCount() const;
    size_t SceneMeshIndex() const;
    void UseNormalShading();
    void UseTransparentShading();

private:
    void SetupMesh();
    void NormalShading();
    void TransparentShading();
    void ShadowShading();
};
