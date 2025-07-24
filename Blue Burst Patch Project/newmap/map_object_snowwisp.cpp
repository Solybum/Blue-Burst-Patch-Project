#include "common.h"
#include "d3d8types.h"
#ifdef PATCH_NEWMAP

#include <cstdint>

#define _USE_MATH_DEFINES
#include <cmath>
#undef _USE_MATH_DEFINES

#include "../mathutil.h"
#include "../ninja.h"
#include "../psobb_functions.h"

#include "map_object_snowwisp.h"

static void* xvm = nullptr;
static TBinModel* bml = nullptr;

RibbonTrail::RibbonTrail(
    size_t numSegments,
    float segmentLength,
    float ribbonWidth,
    float jointStiffness)
        : numSegments(numSegments),
          segmentLength(segmentLength),
          ribbonWidth(ribbonWidth),
          jointStiffness(jointStiffness)
{
    // Zero-alloc buffers
    auto numVertices = numSegments * 2;
    auto numIndices = (numSegments - 1) * 2 * 3;
    simulationPoints.resize(numSegments);
    meshVertices.resize(numVertices);
    meshIndices.resize(numIndices);

    // Arrange indices as trianglelist
    for (auto i = 0; i < numSegments - 1; i++)
    {
        // Two isolated triangles per segment
        auto a = i * 2;
        auto b = a + 1;
        auto c = a + 2;
        auto d = a + 3;
        auto j = i * 2 * 3;
        // a-b-c
        meshIndices[j + 0] = a;
        meshIndices[j + 1] = b;
        meshIndices[j + 2] = c;
        // b-d-c
        meshIndices[j + 3] = b;
        meshIndices[j + 4] = d;
        meshIndices[j + 5] = c;
    }

    // Make UVs
    for (auto i = 0; i < numSegments; i++)
    {
        auto u = float(i) / float(numSegments - 1);
        meshVertices[i * 2 + 0].u = u;
        meshVertices[i * 2 + 0].v = 0.0f;
        meshVertices[i * 2 + 1].u = u;
        meshVertices[i * 2 + 1].v = 1.0f;
    }
}

void RibbonTrail::Update(Vec3f headPos)
{
    // Compute inverse kinematics between segments
    simulationPoints[0].set(headPos);
    for (auto i = 1; i < numSegments; i++)
    {
        // Compute direction between segments
        auto prevSegment = simulationPoints[i - 1];
        auto& curSegment = simulationPoints[i];
        auto segmentsDir = Vec3f { curSegment };
        segmentsDir.Subtract(prevSegment);
        auto segmentsDist = segmentsDir.Length();
        // Apply a minimum distance to prevent jankyness
        if (segmentsDist < 1e-6)
        {
            segmentsDir.x = 0.0f;
            segmentsDir.y = 0.0f;
            segmentsDir.z = 1.0f;
        }
        else
        {
            segmentsDir.Normalize();
        }
        // Enforce segment length
        segmentsDir.Multiply(segmentLength);
        // Drag current segment towards previous segment
        auto curSegmentTarget = Vec3f { prevSegment };
        curSegmentTarget.Add(segmentsDir);
        curSegment.Lerp(curSegmentTarget, jointStiffness);
    }

    // Update vertex positions
    for (auto i = 0; i < numSegments; i++)
    {
        // Center coords to 0
        auto localPos = Vec3f { simulationPoints[i] };
        localPos.Subtract(headPos);

        auto taperRatio = 1.0f - (float(i) / float(numSegments - 1));
        auto ribbonMiddleOffset = Vec3f { 0.0f, ribbonWidth / 2.0f * taperRatio, 0.0f };
        auto j0 = i * 2;
        auto j1 = i * 2 + 1;

        // Bottom edge
        meshVertices[j0].x = localPos.x - ribbonMiddleOffset.x;
        meshVertices[j0].y = localPos.y - ribbonMiddleOffset.y;
        meshVertices[j0].z = localPos.z - ribbonMiddleOffset.z;
        // Top edge
        meshVertices[j1].x = localPos.x + ribbonMiddleOffset.x;
        meshVertices[j1].y = localPos.y + ribbonMiddleOffset.y;
        meshVertices[j1].z = localPos.z + ribbonMiddleOffset.z;
    }
}

void RibbonTrail::Render() const
{
    reinterpret_cast<void (__cdecl *)()>(0x0082f1d0)(); // Set world transform
    reinterpret_cast<void (__fastcall *)(uint32_t texStage, uint32_t texId)>(0x00839838)(0, 1); // Select texture
    (*d3dDevice)->lpVtbl->SetVertexShader(*d3dDevice, D3DFVF_XYZ | D3DFVF_TEX1);
    (*d3dDevice)->lpVtbl->DrawIndexedPrimitiveUP(
        *d3dDevice,
        D3DPT_TRIANGLELIST,
        0,
        meshVertices.size(),
        meshIndices.size() / 3,
        meshIndices.data(),
        D3DFMT_INDEX16,
        meshVertices.data(),
        sizeof(Vertex));
}

void __cdecl MapObjectSnowWisp::LoadAssets() {
    static BmlMetadata bmlMeta = {
        "map_object_snowwisp.bml",
        nullptr,
        nullptr,
        2,
        0
    };

    xvm = LoadXvm("map_object_snowwisp.xvm");
    
    bml = (TBinModel*) pf_malloc(sizeof(TBinModel));
    LoadBml(bml, 5, &bmlMeta, nullptr, 0, nullptr);

    PrepareBmlMeshes(bml);
}

void __cdecl MapObjectSnowWisp::UnloadAssets() {
    if (xvm)
    {
        FreeXvm(xvm);
        xvm = nullptr;
    }

    if (bml) {
        FreeBml(bml, 1);
        bml = nullptr;
    }
}

void* __cdecl MapObjectSnowWisp::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectSnowWisp));
    MapObjectSnowWisp* obj = new (buf) MapObjectSnowWisp(*MapObject::rootMapObject, initData);
    return obj;
}

MapObjectSnowWisp::MapObjectSnowWisp(void* parentObject, MapObject::InitData::InnerData* initData) :
    MapObject::MapObjectBase(parentObject),
    forwardSpeed(0.1f),
    turnSpeed(0.005f),
    turnMaxAngleDeg(180),
    turnFrameIntervalRandMin(300),
    turnFrameIntervalRandMax(400),
    swayAmplitudeHorizontal(0.05f),
    swayAmplitudeVertical(0.1f),
    swayFrequencyHorizontal(0.004f),
    swayFrequencyVertical(0.002f),
    frameCounter(0),
    framesUntilNextTurn(turnFrameIntervalRandMin),
    forwardDirection(Vec3f { 0.0f, 0.0f, 1.0f }),
    targetDirection(Vec3f { 0.0f, 0.0f, 1.0f }),
    turnProgress(1.0f),
    lastGroundPosition(Vec3f { 0.0f, 0.0f, 0.0f }),
    ribbonTrail(10, 2.0f, 5.0f, 0.5f)
{
    OVERRIDE_METHOD(MapObjectSnowWisp, Destruct);
    OVERRIDE_METHOD(MapObjectSnowWisp, Update);
    OVERRIDE_METHOD(MapObjectSnowWisp, Render);
    
    vtable->ApplyInitData(this, initData);

    auto InitRng = reinterpret_cast<void (__thiscall *)(char* rng, uint32_t seed)>(0x0084c390);
    InitRng(rngState, entityIndex);
}

void MapObjectSnowWisp::Destruct(BOOL freeMemory)
{
    MapObjectBase::Destruct(false);

    MapObjectSnowWisp::~MapObjectSnowWisp();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void MapObjectSnowWisp::SnapToMapSurface()
{
    Vec3f pos;
    pos.set(xyz1);
    // Move the projection point slightly up to ensure it's actually above the ground
    pos.y += 10.0;
    // 0x15 is what other entities seem to use
    auto ground = GetGround(&pos, 0x15);
    
    if (ground != nullptr && ground->position != nullptr)
    {
        xyz1.set(ground->position);
        lastGroundPosition.set(ground->position);
    }
}

float MapObjectSnowWisp::RandomRange(float min, float max)
{
    auto GetRandom = reinterpret_cast<uint32_t (__fastcall *)(char* rng)>(0x00763498);
    auto r = (GetRandom(rngState) >> 0x10) / 65536.0f;
    return min + r * (max - min);
}

void MapObjectSnowWisp::ChangeDirection(Vec3f newDir)
{
    targetDirection.set(newDir);
    turnProgress = 0.0f;
}

void MapObjectSnowWisp::Update()
{
    frameCounter++;

    // Calculate current amount of sway
    const auto swayPhaseOffset = M_PI / 3.0;
    auto swayScalarHorizontal = sinf(frameCounter * swayFrequencyHorizontal * M_PI * 2.0) * swayAmplitudeHorizontal;
    auto swayScalarVertical = sinf(frameCounter * swayFrequencyVertical * M_PI * 2.0 + swayPhaseOffset) * swayAmplitudeVertical;

    // Count down until it's time to make a turn
    framesUntilNextTurn--;
    if (framesUntilNextTurn <= 0)
    {
        // Make a random turn
        auto maxAngleRad = turnMaxAngleDeg * M_PI / 180.0;
        auto angleChange = RandomRange(-maxAngleRad, maxAngleRad);
        auto newDir = Vec3f { forwardDirection };
        newDir.RotateWithAxis(Vec3f { 0.0f, 1.0f, 0.0f } , angleChange);
        ChangeDirection(newDir);
        framesUntilNextTurn = int(RandomRange(float(turnFrameIntervalRandMin), float(turnFrameIntervalRandMax)));
    }

    // Turn until fully turned towards target direction
    if (turnProgress < 1.0f)
    {
        turnProgress += turnSpeed;
        if (turnProgress > 1.0f) turnProgress = 1.0f;
        forwardDirection.Lerp(targetDirection, turnProgress);
        forwardDirection.Normalize();
    }

    // Calculate movement vectors
    auto forwardMoveVec = Vec3f { forwardDirection };
    forwardMoveVec.Multiply(forwardSpeed);
    auto swayVecHoriz = Vec3f { forwardDirection };
    swayVecHoriz.Cross(Vec3f { 0.0f, 1.0f, 0.0f }); // Perpendicular of forward around up axis
    swayVecHoriz.Normalize();
    swayVecHoriz.Multiply(swayScalarHorizontal);
    auto swayVecVert = Vec3f { 0.0f, 1.0f, 0.0f };
    swayVecVert.Multiply(swayScalarVertical);

    auto newPos = Vec3f { };
    newPos.set(xyz2);
    // Always sway even if stuck
    newPos.Add(swayVecHoriz);
    newPos.Add(swayVecVert);
    // TODO: Try to not move out of bounds
    newPos.Add(forwardMoveVec);
    xyz2.set(newPos);
    ribbonTrail.Update(newPos);
    SnapToMapSurface();
}

void MapObjectSnowWisp::Render()
{
    // Make billboard
    auto viewMatrix = D3DMATRIX { };
    (*d3dDevice)->lpVtbl->GetTransform(*d3dDevice, D3DTS_VIEW, &viewMatrix);
    auto worldCameraRight = Vec3f { viewMatrix._11, viewMatrix._21, viewMatrix._31 };
    auto worldCameraTop = Vec3f { viewMatrix._12, viewMatrix._22, viewMatrix._32 };
    std::vector<Vertex> localBillboardVertices =
    {
        Vertex { -0.5f, -0.5f,  0.0f, 0.0f, 0.0f },
        Vertex {  0.5f, -0.5f,  0.0f, 1.0f, 0.0f },
        Vertex { -0.5f,  0.5f,  0.0f, 0.0f, 1.0f },
        Vertex {  0.5f,  0.5f,  0.0f, 1.0f, 1.0f }
    };
    auto billboardWidth = 5.0f;
    auto billboardHeight = 5.0f;
    std::vector<Vertex> worldBillboardVertices(localBillboardVertices.size());
    for (auto i = 0; i < worldBillboardVertices.size(); i++)
    {
        auto right = Vec3f { worldCameraRight };
        right.Multiply(localBillboardVertices[i].x * billboardWidth);
        auto top = Vec3f { worldCameraTop };
        top.Multiply(localBillboardVertices[i].y * billboardHeight);
        right.Add(top);

        worldBillboardVertices[i].x = right.x;
        worldBillboardVertices[i].y = right.y;
        worldBillboardVertices[i].z = right.z;
        worldBillboardVertices[i].u = localBillboardVertices[i].u;
        worldBillboardVertices[i].v = localBillboardVertices[i].v;
    }

    UseTextureArchive(xvm);

    auto baseHeightFromGround = 10.0f;
    Vec3f renderPosition;
    renderPosition.set(xyz2);
    renderPosition.y += baseHeightFromGround;

    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(&renderPosition);

    int savedDepthBufferSettings[4] = {};
    auto currentDepthBufferSettings = reinterpret_cast<int*>(0x00aab47c);
    std::copy(currentDepthBufferSettings, currentDepthBufferSettings + 4, savedDepthBufferSettings);

    int depthBufferSettings2[] = { D3DCMP_LESSEQUAL, 0, 0, 0 };
    reinterpret_cast<void (__fastcall *)(int *)>(0x00836500)(depthBufferSettings2); // set_depth_buffer_settings

    ribbonTrail.Render();

    int depthBufferSettings1[] = { D3DCMP_LESSEQUAL, 1, 0, 0 };
    reinterpret_cast<void (__fastcall *)(int *)>(0x00836500)(depthBufferSettings1); // set_depth_buffer_settings

    reinterpret_cast<void (__cdecl *)()>(0x0082f1d0)(); // Set world transform
    reinterpret_cast<void (__fastcall *)(uint32_t texStage, uint32_t texId)>(0x00839838)(0, 0); // Select texture
    (*d3dDevice)->lpVtbl->SetVertexShader(*d3dDevice, D3DFVF_XYZ | D3DFVF_TEX1);
    (*d3dDevice)->lpVtbl->DrawPrimitiveUP(*d3dDevice, D3DPT_TRIANGLESTRIP, 2, worldBillboardVertices.data(), sizeof(Vertex));

    reinterpret_cast<void (__fastcall *)(int *)>(0x00836500)(savedDepthBufferSettings); // set_depth_buffer_settings

    Transform::PopTransformStack();
}

#endif
