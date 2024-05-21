#pragma once

#include <cstdint>

#include "map_object.h"

class MapObjectNewdoor : public MapObject::MapObjectBase
{
public:
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static void* __cdecl Create(MapObject::InitData::InnerData* initData);

    MapObjectNewdoor(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();
    void Render();

private:
    uint32_t animElapsed;
};
