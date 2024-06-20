#pragma once

#include <cstdint>
#include <vector>
#include <windows.h>

#include "../map_object.h"
#include "mathutil.h"

#pragma pack(push, 1)
struct Packet0x60_0x5
{
    uint8_t subcmdType;
    uint8_t packetSize;
    uint16_t senderId;
    uint16_t unk1;
    uint16_t unk2;
    uint16_t switchId;
    uint8_t floor;
    uint8_t flags;
};
#pragma pack(pop)

class MapObjectPayloadCheckpoint : public MapObject::MapObjectBase
{
public:
    static const size_t objectTypeId = 1341;
    
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static void* __cdecl Create(MapObject::InitData::InnerData* initData);

    MapObjectPayloadCheckpoint(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();
    void Render();
    void RenderShadow();
    
    uint16_t switchId;
};


class MapObjectPayload : public MapObject::MapObjectBase
{
public:
    static const size_t objectTypeId = 1340;

    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static void* __cdecl Create(MapObject::InitData::InnerData* initData);

    MapObjectPayload(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();
    void Render();
    void RenderShadow();
    void HandleUnlockEvent(Packet0x60_0x5* packet);

private:
    Vec3f originalPosition;
    std::vector<uint16_t> switches;
    float circleRadius;
    float moveSpeed;
    /// Furthest checkpoint this object instance has reached.
    /// Can differ from furthest unlocked switch due to desync, or when entering area mid-progress.
    int furthestReachedCheckpointIdx;
    int16_t circleRotation;
    
    struct Vertex
    {
        float x;
        float y;
        float z;
        float u;
        float v;
    };
    
    std::vector<Vertex> floorPathMesh;
    
    void SnapToMapSurface();
    MapObjectPayloadCheckpoint* FindCheckpoint(uint16_t switchId);
    /// Returns true if reached target
    bool MoveTowards(const Vec3f& targetPosition);
    void MakeFloorPathMesh();
};
