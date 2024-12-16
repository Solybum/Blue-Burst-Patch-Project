#ifdef PATCH_NEWMAP

#include <cmath>

#include "map_object_slippery.h"

#include "../entity.h"
#include "../entitylist.h"
#include "../mathutil.h"

Vec2::Vec2() : x(0.0f), y(0.0f) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

void __cdecl MapObjectSlippery::LoadAssets() { }

void __cdecl MapObjectSlippery::UnloadAssets() { }

void* __cdecl MapObjectSlippery::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectSlippery));
    MapObjectSlippery* obj = new (buf) MapObjectSlippery(*MapObject::rootMapObject, initData);
    return obj;
}

MapObjectSlippery::MapObjectSlippery(void* parentObject, MapObject::InitData::InnerData* initData) :
    MapObject::MapObjectBase(parentObject),
    effectRadius(0.0)
{
    OVERRIDE_METHOD(MapObjectSlippery, Destruct);
    OVERRIDE_METHOD(MapObjectSlippery, Update);
    
    vtable->ApplyInitData(this, initData);
    effectRadius = initData->param1.asFloat;
}

void MapObjectSlippery::Destruct(BOOL freeMemory)
{
    MapObjectBase::Destruct(false);


    MapObjectSlippery::~MapObjectSlippery();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void MapObjectSlippery::Update()
{
    auto ApplyVelocityToPlayer = reinterpret_cast<void (__thiscall *)(void* player, float x, float z)>(0x0077a0f8);

    for (auto playerPtr : EntityList::Players())
    {
        auto player = Entity::BaseEntityWrapper(playerPtr);
        auto playerIdx = Entity::EntityIndex(player.entityIndex());

        // Check if player within radius
        // Also check if vertical position is lower than player, this can be used to make slightly elevated areas non-slippery.
        if (player.position().y < xyz2.y && Squared(effectRadius) > DistanceSquaredXZ(player.position(), xyz2)) {
            auto& curV = player.velocity();
            auto oldSlideV = playerSlideVelocities[playerIdx];
            
            float a = 0.99;
            float b = 0.95;
            // Add a portion of previous speed and current speed
            auto newSlideV = Vec2 { curV.x * (1.0f - b) + oldSlideV.x * a, curV.z * (1.0f - b) + oldSlideV.y * a };

            auto newMag = std::sqrt(newSlideV.x * newSlideV.x + newSlideV.y * newSlideV.y);
            auto maxMag = 0.8f;
            auto cappedMag = std::min(newMag, maxMag);

            // Avoid divide by zero. Magnitude can be 0 when standing still.
            if (cappedMag != 0.0) {
                // Apply a limit on velocity
                auto newCapped = Vec2 { newSlideV.x / newMag * cappedMag, newSlideV.y / newMag * cappedMag };
                playerSlideVelocities[playerIdx] = newCapped;
                ApplyVelocityToPlayer(playerPtr, newCapped.x, newCapped.y);
            }

        } else {
            // Reset velocity when outside
            playerSlideVelocities[playerIdx] = Vec2 { 0.0, 0.0 };
        }
    }
}

#endif
