#ifdef PATCH_NEWMAP

#include <cstdint>
#include "map_object_snowfall.h"
#include "map_object.h"
#include "map.h"
#include "common.h"
#include "player.h"

static auto CreateParticleEffect = reinterpret_cast<ParticleEffect* (__cdecl *)(volatile Vec3f* xyz, uint32_t id, uint32_t delay, uint32_t duration)>(0x0050a984);

void __cdecl MapObjectSnowfall::LoadAssets()
{
}

void __cdecl MapObjectSnowfall::UnloadAssets()
{
}

static const float particleDistance = 50.0;
static const float particleHeight = 20.0;

Vec3f particleOffsets[numParticles] = {
    {0.0, particleHeight, 0.0},
    {particleDistance, particleHeight, 0.0},
    {-particleDistance, particleHeight, 0.0},
    {0.0, particleHeight, particleDistance},
    {0.0, particleHeight, -particleDistance},
};

MapObjectSnowfall::MapObjectSnowfall(void* parentObject, MapObject::InitData::InnerData* initData)
    : MapObject::MapObjectBase(parentObject)
{
    OVERRIDE_METHOD(MapObjectSnowfall, Destruct);
    OVERRIDE_METHOD(MapObjectSnowfall, Update);
    OVERRIDE_METHOD(MapObjectSnowfall, Render);

    vtable->ApplyInitData(this, initData);

    auto player = GetPlayer(0);
    for (size_t i = 0; i < numParticles; i++) {
        particles[i] = CreateParticleEffect(&player->position, 0x200, 0, 0);
    }
}

void MapObjectSnowfall::Destruct(BOOL freeMemory)
{
    MapObjectBase::Destruct(false);

    // Manually call own C++ destructor
    MapObjectSnowfall::~MapObjectSnowfall();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void MapObjectSnowfall::Update()
{
    auto player = GetPlayer(0);

    xyz2.set(player->position);

    for (size_t i = 0; i < numParticles; i++) {
        auto x = xyz2.x + particleOffsets[i].x;
        auto y = xyz2.y + particleOffsets[i].y;
        auto z = xyz2.z + particleOffsets[i].z;
        particles[i]->x1 = x;
        particles[i]->y1 = y;
        particles[i]->z1 = z;
        particles[i]->x2 = x;
        particles[i]->y2 = y;
        particles[i]->z2 = z;
    }
}

void MapObjectSnowfall::Render()
{
}

void* __cdecl MapObjectSnowfall::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectSnowfall));
    MapObjectSnowfall* cloud = new (buf) MapObjectSnowfall(*MapObject::rootMapObject, initData);
    return cloud;
}

#endif
