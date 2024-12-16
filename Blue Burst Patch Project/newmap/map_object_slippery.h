#pragma once

#include <unordered_map>
#include <windows.h>

#include "../map_object.h"
#include "../entity.h"

struct Vec2 {
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);
};

class MapObjectSlippery : public MapObject::MapObjectBase
{
public:
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static void* __cdecl Create(MapObject::InitData::InnerData* initData);

    MapObjectSlippery(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();

private:
    float effectRadius;
    std::unordered_map<Entity::EntityIndex, Vec2> playerSlideVelocities;
};
