#pragma once

#include "../map_object.h"

class MapObjectTurret : MapObject::MapObjectBase
{
public:
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static void* __cdecl Create(MapObject::InitData::InnerData* initData);

    MapObjectTurret(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();
    void Render();
    void RenderShadow();

private:
    int shootCooldownTimer;
};
