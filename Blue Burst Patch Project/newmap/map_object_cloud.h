#pragma once

#include <cstdint>
#include <windows.h>
#include "map_object.h"

class Cloud : public MapObject::MapObjectBase
{
public:
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static Cloud* __cdecl Create(MapObject::InitData::InnerData* initData);

    Cloud(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();
    void Render();

private:
    uint16_t texCoordX;
};

void EnableMapObjectCloud();
