#pragma once

#include <cstdint>
#include <windows.h>
#include "map_object.h"
#include "object_extension.h"

static const size_t numParticles = 5;

struct ParticleEffect
{
    union
    {
        union {
            DEFINE_FIELD(0x6c, float x1);
            DEFINE_FIELD(0x70, float y1);
            DEFINE_FIELD(0x74, float z1);
            DEFINE_FIELD(0x78, float x2);
            DEFINE_FIELD(0x7c, float y2);
            DEFINE_FIELD(0x80, float z2);
        };

        uint8_t _padding[0xd0];
    };
};

class MapObjectSnowfall : public MapObject::MapObjectBase
{
public:
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static MapObjectSnowfall* __cdecl Create(MapObject::InitData::InnerData* initData);

    MapObjectSnowfall(void* parentObject, MapObject::InitData::InnerData* initData);

    void Destruct(BOOL freeMemory);
    void Update();
    void Render();

private:
    ParticleEffect* particles[numParticles];
};

void EnableMapObjectSnowfall();
