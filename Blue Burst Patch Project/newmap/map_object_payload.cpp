#ifdef PATCH_NEWMAP

#define _USE_MATH_DEFINES

#include <cstdint>
#include <cmath>
#include <windows.h>

#include "d3d8types.h"

#include "../enemy.h"
#include "../common.h"
#include "../entity.h"
#include "../entitylist.h"
#include "../mathutil.h"
#include "../object_wrapper.h"
#include "../ninja.h"
#include "../psobb_functions.h"
#include "../map_object.h"
#include "../object_extension.h"
#include "map_object_payload.h"

static auto IsSwitchUnlockedOnCurrentFloor = reinterpret_cast<bool (__cdecl *)(uint16_t switchId)>(0x00816eec);
static auto LockSwitchOnCurrentFloor = reinterpret_cast<void (__cdecl *)(uint16_t switchId)>(0x00816e60);
static auto UnlockSwitchOnCurrentFloor = reinterpret_cast<void (__cdecl *)(uint16_t switchId)>(0x00816dd8);
static auto UpdateCollisionBoxPosition = reinterpret_cast<void (__cdecl *)(void*)>(0x007b999c);
static auto currentFloor = reinterpret_cast<uint8_t*>(0x00aafca0);

static void SendUnlockPacket(uint16_t objectId, uint16_t switchId)
{
    auto SendPacket0x60 = reinterpret_cast<void (__fastcall *)(void* packet)>(0x008003e0);

    Packet0x60_0x5 packet = 
    {
        .subcmdType = 0x5,
        .packetSize = 0x3,
        .senderId = objectId,
        .unk1 = 0,
        .unk2 = 0,
        .switchId = switchId,
        .floor = *currentFloor,
        .flags = 1
    };
    
    SendPacket0x60(&packet);
}

enum ModelIndex
{
    Checkpoint = 0,
    PayloadCircle = 1,
    PayloadMiddle = 2
};

static auto checkpointTypeSentinel = "MapObjectPayloadCheckpoint";
static auto cylinderCollisionBox = Enemy::CollisionBox(0.0, 5.0, 0.0, 1.0);
static void* xvm = nullptr;
static TBinModel* bml = nullptr;

void __cdecl MapObjectPayloadCheckpoint::LoadAssets() {}

void __cdecl MapObjectPayloadCheckpoint::UnloadAssets() {}

void* __cdecl MapObjectPayloadCheckpoint::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectPayloadCheckpoint));
    MapObjectPayloadCheckpoint* obj = new (buf) MapObjectPayloadCheckpoint(*MapObject::rootMapObject, initData);
    return obj;
}

MapObjectPayloadCheckpoint::MapObjectPayloadCheckpoint(void* parentObject, MapObject::InitData::InnerData* initData) :
    MapObject::MapObjectBase(parentObject)    
{
    OVERRIDE_METHOD(MapObjectPayloadCheckpoint, Destruct);
    OVERRIDE_METHOD(MapObjectPayloadCheckpoint, Update);
    OVERRIDE_METHOD(MapObjectPayloadCheckpoint, Render);
    OVERRIDE_METHOD(MapObjectPayloadCheckpoint, RenderShadow);
    
    vtable->ApplyInitData(this, initData);
    collisionBoxes = nullptr;
    collisionBoxCount = 0;
    Enemy::InsertIntoEntityList(this);
    
    typeId = &checkpointTypeSentinel;
    switchId = initParam4.asDword;
}

void MapObjectPayloadCheckpoint::Destruct(BOOL freeMemory)
{
    MapObjectBase::Destruct(false);

    Enemy::RemoveFromEntityList(this);

    MapObjectPayloadCheckpoint::~MapObjectPayloadCheckpoint();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void MapObjectPayloadCheckpoint::Update()
{
}

void MapObjectPayloadCheckpoint::Render()
{
    auto model = GetModel(bml, ModelIndex::Checkpoint);
    
    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(const_cast<Vec3f*>(&xyz1));
    Transform::RotateMatrix(nullptr, rotation.y);

    UseTextureArchive(xvm);

    RenderXj(model);

    Transform::PopTransformStack();
}

void MapObjectPayloadCheckpoint::RenderShadow() {}

MapObjectPayload::MapObjectPayload(void* parentObject, MapObject::InitData::InnerData* initData) :
    furthestReachedCheckpointIdx(-1),
    circleRotation(0),
    MapObject::MapObjectBase(parentObject)
{
    OVERRIDE_METHOD(MapObjectPayload, Destruct);
    OVERRIDE_METHOD(MapObjectPayload, Update);
    OVERRIDE_METHOD(MapObjectPayload, Render);
    OVERRIDE_METHOD(MapObjectPayload, RenderShadow);
    OVERRIDE_METHOD(MapObjectPayload, HandleUnlockEvent);
    
    vtable->ApplyInitData(this, initData);
    collisionBoxes = nullptr;
    Enemy::InitCollisionBoxes(this, &cylinderCollisionBox, 1);
    Enemy::InsertIntoEntityList(this);

    // Init params
    originalPosition.set(initData->position);
    circleRadius = initParam1.asFloat;
    moveSpeed = initParam2.asFloat;
    auto baseSwitchId = initParam4.asDword;
    auto switchCount = initParam5.asDword;

    for (uint16_t i = 0; i < switchCount; i++)
    {
        switches.push_back(baseSwitchId + i);
    }
}

void MapObjectPayload::Destruct(BOOL freeMemory)
{
    FreeCollisionBoxes(this);

    MapObjectBase::Destruct(false);

    Enemy::RemoveFromEntityList(this);

    MapObjectPayload::~MapObjectPayload();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

MapObjectPayloadCheckpoint* MapObjectPayload::FindCheckpoint(uint16_t switchId)
{
    bool foundCheckpointObject = false;
    Vec3f targetPosition;
    for (auto ptr : EntityList::Objects())
    {
        auto obj = ObjectWrapper(ptr);
        // Identify using type sentinel
        if (obj.typeId() == &checkpointTypeSentinel)
        {
            auto checkpoint = reinterpret_cast<MapObjectPayloadCheckpoint*>(ptr);
            if (switchId == checkpoint->switchId)
                return checkpoint;
        }
    }
    
    return nullptr;
}

bool MapObjectPayload::MoveTowards(const Vec3f& targetPosition)
{
    auto dist = (float) std::sqrt(DistanceSquaredXZ(targetPosition, xyz1));
    if (dist < 0.1) // Small threshold to mitigate jittering
    {
        // Reached target
        return true;
    }

    auto moveAmount = std::min(dist, moveSpeed);
    auto angle = std::atan2(targetPosition.z - xyz1.z, targetPosition.x - xyz1.x);
    xyz1.x += std::cos(angle) * moveAmount;
    xyz1.z += std::sin(angle) * moveAmount;
    
    return false;
}

void MapObjectPayload::MakeFloorPathMesh()
{
    // Get positions of all checkpoints
    auto offsetY = 0.2f;
    std::vector<Vec3f> checkpointPositions = {{0.0, offsetY, 0.0}};
    for (const auto& sw : switches)
    {
        auto c = FindCheckpoint(sw);
        if (c == nullptr) break;

        Vec3f pos = {
            c->xyz1.x - originalPosition.x,
            c->xyz1.y - originalPosition.y + offsetY,
            c->xyz1.z - originalPosition.z};
        // Normalize to original payload position
        checkpointPositions.push_back(pos);
    }

    const float pathMeshWidth = 2.5;
    const float pathMeshHalfWidth = pathMeshWidth / 2.0;

    floorPathMesh.clear();

    bool uvToggle = false;
    for (size_t i = 0; i < checkpointPositions.size() - 1; i++)
    {
        const auto& cur = checkpointPositions[i];
        const auto& next = checkpointPositions[i + 1];

        float forwardAngle = std::atan2(next.z - cur.z, next.x - cur.x);
        // Scale to 0..pi2, add 90deg, mod%pi, scale back down to -pi..pi
        // TODO: This would look better if the angle was half turned towards the third checkpoint
        float sideAngle = std::fmod(forwardAngle + M_PI + M_PI / 2.0, M_PI) - M_PI;
        float dist = std::sqrt(DistanceSquaredXZ(cur, next));
        float sinSide = std::sin(sideAngle);
        float cosSide = std::cos(sideAngle);

        if (i == 0)
        {
            floorPathMesh.push_back({
                .x = cur.x - pathMeshHalfWidth * cosSide,
                .y = cur.y,
                .z = cur.z - pathMeshHalfWidth * sinSide,
                .u = 0.0,
                .v = 0.0
            });
            floorPathMesh.push_back({
                .x = cur.x + pathMeshHalfWidth * cosSide,
                .y = cur.y,
                .z = cur.z + pathMeshHalfWidth * sinSide,
                .u = 0.0,
                .v = 0.0
            });
        }

        float uv = uvToggle ? 0.0 : 1.0;
        uvToggle = !uvToggle;
        floorPathMesh.push_back({
            .x = next.x - pathMeshHalfWidth * cosSide,
            .y = next.y,
            .z = next.z - pathMeshHalfWidth * sinSide,
            .u = uv,
            .v = uv
        });
        floorPathMesh.push_back({
            .x = next.x + pathMeshHalfWidth * cosSide,
            .y = next.y,
            .z = next.z + pathMeshHalfWidth * sinSide,
            .u = uv,
            .v = uv
        });
    }
    
}

void MapObjectPayload::Update()
{
    SnapToMapSurface();
    UpdateCollisionBoxPosition(this);
    circleRotation += 10;
    MakeFloorPathMesh();

    // It's probably best to save as little state as possible in the object to mitigate desync.
    // We can probably derive the object state from state that is shared across clients e.g. quest events.

    if (switches.empty())
    {
        // Misconfigured object, nothing to do
        return;
    }

    // Find unlocked switch with greatest id.
    // This will be the furthest checkpoint that we have unlocked, or -1 if we have not unlocked any yet.
    int furthestUnlockedCheckpointIdx = -1;
    for (int i = switches.size() - 1; i >= 0; --i)
    {
        if (IsSwitchUnlockedOnCurrentFloor(switches[i]))
        {
            furthestUnlockedCheckpointIdx = i;
            break;
        }
    }

    // These values can differ due to desync
    if (furthestUnlockedCheckpointIdx > furthestReachedCheckpointIdx)
    {
        // Fast-forward to unlocked checkpoint
        furthestReachedCheckpointIdx = furthestUnlockedCheckpointIdx;
        if (auto checkpoint = FindCheckpoint(switches[furthestUnlockedCheckpointIdx]))
        {
            xyz1.set(checkpoint->xyz1);
        }
    }

    auto nextCheckpointIdx = furthestUnlockedCheckpointIdx + 1;
    if (nextCheckpointIdx >= switches.size())
    {
        // Reached final checkpoint, do nothing
        return;
    }
    
    // Check if a player is inside our radius
    bool isPlayerInsideRadius = false;
    bool isPlayerInsideExtendedRadius = false;
    for (auto playerPtr : EntityList::Players())
    {
        auto player = Entity::BaseEntityWrapper(playerPtr);
        auto distSq = DistanceSquaredXZ(player.position(), xyz1);
        if (distSq < Squared(circleRadius + 1.0))
        {
            isPlayerInsideExtendedRadius = true;

            if (distSq < Squared(circleRadius))
                isPlayerInsideRadius = true;

            break;
        }
    }

    if (!isPlayerInsideRadius)
    {
        if (isPlayerInsideExtendedRadius)
        {
            // A small buffer radius that will cause the payload to simply stop.
            // Prevents jittery movement when player is standing on the edge of radius.
            return;
        }

        // If nobody is inside our radius then try move backwards
        Vec3f targetPosition;
        if (furthestUnlockedCheckpointIdx < 0)
        {
            targetPosition.set(originalPosition);
        }
        else if (auto prevCheckpoint = FindCheckpoint(switches[furthestUnlockedCheckpointIdx]))
        {
            targetPosition.set(prevCheckpoint->xyz1);
        }
        MoveTowards(targetPosition);
        return;
    }

    auto nextSwitchId = switches[nextCheckpointIdx];
    auto nextCheckpoint = FindCheckpoint(nextSwitchId);
    if (nextCheckpoint == nullptr)
    {
        // It's possible that the checkpoint object doesn't exist yet because it is too far away or something.
        // Let's just do nothing instead of throwing.
        return;
    }

    // Move forwards
    Vec3f targetPosition;
    targetPosition.set(nextCheckpoint->xyz1);
    auto reachedNewCheckpoint = MoveTowards(targetPosition);
    // Send unlock packet when we reach a new checkpoint
    if (reachedNewCheckpoint)
    {
        furthestReachedCheckpointIdx = nextCheckpointIdx;
        SendUnlockPacket(entityIndex, nextSwitchId);
    }
}

void MapObjectPayload::Render()
{
    auto checkpointModel = GetModel(bml, ModelIndex::Checkpoint);
    auto circleModel = GetModel(bml, ModelIndex::PayloadCircle);
    auto middleModel = GetModel(bml, ModelIndex::PayloadMiddle);

    UseTextureArchive(xvm);

    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(&originalPosition);

    // Draw payload path on the floor
    reinterpret_cast<void (__cdecl*)()>(0x0082f1d0)(); // Set world transform
    (*d3dDevice)->lpVtbl->SetVertexShader(*d3dDevice, D3DFVF_XYZ | D3DFVF_TEX1);
    (*d3dDevice)->lpVtbl->DrawPrimitiveUP(*d3dDevice, D3DPT_TRIANGLESTRIP, floorPathMesh.size() - 2, floorPathMesh.data(), sizeof(Vertex));

    // Draw a checkpoint at the start position
    RenderXj(checkpointModel);
    
    Transform::PopTransformStack();

    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(const_cast<Vec3f*>(&xyz1));
    Transform::RotateMatrix(nullptr, rotation.y);

    // Draw circle
    auto circleScale = circleRadius / 32.0;
    Transform::PushTransformStackCopy();
    Transform::ScaleMatrix(nullptr, circleScale, 1.0, circleScale);
    Transform::RotateMatrix(nullptr, circleRotation);
    RenderXj(circleModel);
    Transform::PopTransformStack();

    // Draw middle
    RenderXj(middleModel);

    Transform::PopTransformStack();
}

void MapObjectPayload::RenderShadow() {}

void MapObjectPayload::HandleUnlockEvent(Packet0x60_0x5* packet)
{
    if (packet->floor != *currentFloor) return;

    if (packet->flags & 1)
    {
        UnlockSwitchOnCurrentFloor(packet->switchId);
    }
    else
    {
        LockSwitchOnCurrentFloor(packet->switchId);
    }
}

void MapObjectPayload::SnapToMapSurface()
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
    }
}

void __cdecl MapObjectPayload::LoadAssets()
{
    static BmlMetadata bmlMeta = {
        "bm_obj_payload.bml",
        nullptr,
        nullptr,
        3,
        0
    };

    xvm = LoadXvm("bm_obj_payload.xvm");
    
    bml = (TBinModel*) pf_malloc(sizeof(TBinModel));
    LoadBml(bml, 5, &bmlMeta, nullptr, 0, nullptr);

    PrepareBmlMeshes(bml);
}

void __cdecl MapObjectPayload::UnloadAssets()
{
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

void* __cdecl MapObjectPayload::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectPayload));
    MapObjectPayload* obj = new (buf) MapObjectPayload(*MapObject::rootMapObject, initData);
    return obj;
}

#undef _USE_MATH_DEFINES

#endif
