#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/texture.h>
#include <assimp/matrix4x4.h>
#include "mesh.h"

struct BoneInfo
{
    size_t id;
    aiMatrix4x4 offset;
};

class Model
{
protected:
    std::vector<Mesh> meshes;
    std::string directory;
    const aiScene* scene;

public:
    Model(const std::string& path);
    ~Model();
    void Draw();

private:
    void ProcessMesh(size_t meshIndex);
    void ProcessNode(aiNode* node);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType textureType);
};
