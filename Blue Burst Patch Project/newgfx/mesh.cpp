#include "pch.h"
#ifdef USE_NEWGFX

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
    vertexBoneMap(boneData),
    shadingMode(ShadingMode::Normal)
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

auto isRenderingShadows = reinterpret_cast<bool32*>(0x00acbf1c);
auto shadowTextureFactor = reinterpret_cast<float*>(0x00acbf24);
auto SetAmbientLight = reinterpret_cast<void (__stdcall *)(float r, float g, float b)>(0x00843980);
auto defaultAmbientLight = reinterpret_cast<float*>(0x00a9d480);

void Mesh::NormalShading()
{
    // Mesh is affected by lighting
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_LIGHTING, true);
    // I don't really know what these all do, but they are commonly used by the game
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_ALPHABLENDENABLE, true);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_ALPHAREF, 0xbf); // 0xbf seems to be a common value
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_ZWRITEENABLE, true);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLORARG2, D3DTA_CURRENT);
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
}

void Mesh::TransparentShading()
{
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void Mesh::ShadowShading()
{
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_LIGHTING, false);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_ALPHAREF, 0);

    auto c0 = size_t(shadowTextureFactor[0] * 255.0) << 24;
    auto c1 = size_t(shadowTextureFactor[1] * 255.0) << 16;
    auto c2 = size_t(shadowTextureFactor[2] * 255.0) << 8;
    auto c3 = size_t(shadowTextureFactor[3] * 255.0);
    (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_TEXTUREFACTOR, c0 | c1 | c2 | c3);

    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
}

void Mesh::Draw()
{
    ApplyTransformStack();

    if (textures.size() > 0)
    {
        // Use ambient light color that is also used by the game elsewhere
        SetAmbientLight(*defaultAmbientLight, *defaultAmbientLight, *defaultAmbientLight);
        // Set vertex format
        (*d3dDevice)->lpVtbl->SetVertexShader(*d3dDevice, D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE | D3DFVF_NORMAL);
        // Normalize normals in case the mesh was scaled
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_NORMALIZENORMALS, true);
        // Two texture coordinates
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
        // TODO: Allow more than one texture?
        (*d3dDevice)->lpVtbl->SetTexture(*d3dDevice, 0, (IDirect3DBaseTexture8*) textures[0].object);

        if (*isRenderingShadows) shadingMode = ShadingMode::Shadow;

        switch (shadingMode)
        {
            case ShadingMode::Normal:
                NormalShading();
                break;
            case ShadingMode::Transparent:
                TransparentShading();
                break;
            case ShadingMode::Shadow:
                ShadowShading();
                break;
        }
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
        // Disable normalized normals because Ninja doesn't use it
        (*d3dDevice)->lpVtbl->SetRenderState(*d3dDevice, D3DRS_NORMALIZENORMALS, false);
    }

    if (*isRenderingShadows)
    {
        // Restore normal settings
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_COLORARG2, D3DTA_CURRENT);
        (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
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

void Mesh::UseNormalShading()
{
    shadingMode = ShadingMode::Normal;
}

void Mesh::UseTransparentShading()
{
    shadingMode = ShadingMode::Transparent;
}

#endif // USE_NEWGFX
