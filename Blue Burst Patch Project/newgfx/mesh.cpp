#include <stdexcept>
#include <cstring>
#include <cassert>
#include "mesh.h"
#include "../common.h"

auto ApplyTransformStack = reinterpret_cast<void (__cdecl *)()>(0x0082f1d0);

Mesh::Mesh(const size_t sceneMeshIndex,
           const std::vector<Vertex>& vertices,
           const std::vector<uint32_t>& indices,
           const std::vector<Texture>& textures,
           const std::vector<std::vector<VertexBoneData>>& boneData) :
    sceneMeshIndex(sceneMeshIndex),
    untransformedVertices(vertices),
    indices(indices),
    textures(textures),
    vertexBoneMap(boneData)
{
    assert(vertices.size() > 0 && vertices.size() == boneData.size());

    SetupMesh();
}

Mesh::Mesh(const size_t sceneMeshIndex,
           const std::vector<Vertex>& vertices,
           const std::vector<uint32_t>& indices,
           const std::vector<Texture>& textures) :
    sceneMeshIndex(sceneMeshIndex),
    untransformedVertices(vertices),
    indices(indices),
    textures(textures)
{
    SetupMesh();
}

void Mesh::SetupMesh()
{
    auto vbSize = vectorSize(untransformedVertices);

    // Create a vertex buffer
    if (FAILED((*d3dDevice)->lpVtbl->CreateVertexBuffer(*d3dDevice, vbSize, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vertexBuffer)))
        throw std::runtime_error("Failed to create vertex buffer");

    // To write our vertex data into the buffer we must first lock it
    uint8_t* vertBufData;
    if (FAILED(vertexBuffer->lpVtbl->Lock(vertexBuffer, 0, vbSize, &vertBufData, 0)))
        throw std::runtime_error("Failed to lock vertex buffer");

    // Write vertex data and unlock buffer
    memcpy(vertBufData, untransformedVertices.data(), vbSize);
    vertexBuffer->lpVtbl->Unlock(vertexBuffer);

    auto ibSize = vectorSize(indices);

    // Create an index buffer with options matching Sega's usage, except for the format, which was increased to 32 bits
    if (FAILED((*d3dDevice)->lpVtbl->CreateIndexBuffer(*d3dDevice, ibSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &indexBuffer)))
        throw std::runtime_error("Failed to create index buffer");

    // Lock, write, unlock
    uint8_t* idxBufData;
    if (FAILED(indexBuffer->lpVtbl->Lock(indexBuffer, 0, ibSize, &idxBufData, 0)))
        throw std::runtime_error("Failed to lock index buffer");
    
    memcpy(idxBufData, indices.data(), ibSize);
    indexBuffer->lpVtbl->Unlock(indexBuffer);
}

void Mesh::Draw()
{
    ApplyTransformStack();

    if (textures.size() > 0)
    {
        // Sensible defaults?
        (*d3dDevice)->lpVtbl->SetVertexShader(*d3dDevice, D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL);
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_LIGHTING, true);
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_ALPHABLENDENABLE, true);
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_ALPHAREF, 0xbf);
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_ZWRITEENABLE, true);
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLORARG2, D3DTA_CURRENT);
        // Two texture coordinates
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        (*d3dDevice)->lpVtbl->SetTexture(*d3dDevice, 0, (IDirect3DBaseTexture8*) textures[0].object);
    }

    // Activate vertex buffer
    (*d3dDevice)->lpVtbl->SetStreamSource(*d3dDevice, 0, vertexBuffer, sizeof(Vertex));
    // Activate index buffer
    (*d3dDevice)->lpVtbl->SetIndices(*d3dDevice, indexBuffer, 0);
    // Draw triangleCount triangles using vertexCount vertices starting from the beginning of each buffer
    auto triangleCount = indices.size() / 3;
    auto vertexCount = untransformedVertices.size();
    (*d3dDevice)->lpVtbl->DrawIndexedPrimitive(*d3dDevice, D3DPT_TRIANGLELIST, 0, vertexCount, 0, triangleCount);

    // Unset resources to decrement their reference count
    (*d3dDevice)->lpVtbl->SetStreamSource(*d3dDevice, 0, nullptr, 0);

    if (textures.size() > 0)
    {
        (*d3dDevice)->lpVtbl->SetTexture(*d3dDevice, 0, nullptr);
    }
}

void Mesh::SetVertexBoneMap(const std::vector<std::vector<VertexBoneData>>& boneData)
{
    assert(boneData.size() == untransformedVertices.size());
    vertexBoneMap = boneData;
}

void Mesh::ApplyBoneTransformations(const std::vector<aiMatrix4x4>& transforms)
{
    if (vertexBoneMap.size() != untransformedVertices.size()) return;

    Vertex* vertices;
    if (FAILED(vertexBuffer->lpVtbl->Lock(vertexBuffer, 0, 0, reinterpret_cast<BYTE**>(&vertices), 0)))
        throw std::runtime_error("Failed to lock vertex buffer");

    for (auto vertIdx = 0; vertIdx < untransformedVertices.size(); vertIdx++)
    {
        auto& vert = vertices[vertIdx];

        // Clear previous transformation
        vert.position = untransformedVertices[vertIdx].position;
        vert.normal = untransformedVertices[vertIdx].normal;

        auto bones = vertexBoneMap[vertIdx];
        if (bones.empty()) continue;

        // For each bone that affects this vertex
        auto totalTransform = transforms[bones[0].boneId] * bones[0].boneWeight;
        for (auto i = 1; i < bones.size(); i++)
        {
            // Apply bone's transformation 
            totalTransform = totalTransform + (transforms[bones[i].boneId] * bones[i].boneWeight);
        }

        vert.position *= totalTransform;
        vert.normal *= totalTransform;
    }
    
    vertexBuffer->lpVtbl->Unlock(vertexBuffer);
}

size_t Mesh::VertexCount() const
{
    return untransformedVertices.size();
}

size_t Mesh::TextureCount() const
{
    return textures.size();
}

size_t Mesh::SceneMeshIndex() const
{
    return sceneMeshIndex;
}
