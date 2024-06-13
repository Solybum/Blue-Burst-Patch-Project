#ifdef PATCH_NEWMAP

#include <cstddef>
#include <cstdlib>
#include <windows.h>

#include "d3d8types.h"

#include "../enemy.h"
#include "../map_object.h"
#include "../map.h"
#include "../player.h"
#include "enemy_icecube.h"
#include "fog.h"
#include "map_object_cloud.h"
#include "map_object_newdoor.h"
#include "map_object_snowfall.h"
#include "newmap.h"
#include "slbgm.h"
#include "snow_map.h"
#include "sunlight.h"

static const char* snowMapName = "SNOW01";
static MapAssetPrefixes::Prefixes snowMapAssetPrefixStrings = {
    "map_snow01",
    "map_snow01"
};
static MapAssetPrefixes snowMapAssetPrefixes = {
    &snowMapAssetPrefixStrings,
    1
};

static SetDataTable::Inner1::Inner2 snowSetDataTableInner2 = {
    "map_snow01",
    "map_snow01",
    "map_snow01"
};
static SetDataTable::Inner1 snowSetDataTableInner1 = {
    &snowSetDataTableInner2,
    1
};
static SetDataTable snowSetDataTable = {
    &snowSetDataTableInner1,
    1
};

static FogEntry originalFogEntry;
static LightEntry originalLightEntry;
static SetDataTable originalSetDataTable;

void __thiscall EnableNrelPointLight()
{
    auto player = GetPlayer(0);
    if (player)
    {
        auto light = D3DLIGHT8();
        light.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
        light.Diffuse.r = 1.0;
        light.Diffuse.g = 0.0;
        light.Diffuse.b = 0.0;
        light.Diffuse.a = 1.0;
        light.Ambient.r = 1.0;
        light.Ambient.g = 0.0;
        light.Ambient.b = 0.0;
        light.Ambient.a = 1.0;
        light.Specular.r = 1.0;
        light.Specular.g = 0.0;
        light.Specular.b = 0.0;
        light.Specular.a = 1.0;
        light.Position.x = player->position.x;
        light.Position.y = player->position.y + 10.0;
        light.Position.z = player->position.z;
        light.Range = 500.0;
        light.Attenuation0 = 0.0;
        light.Attenuation1 = 0.5;
        light.Attenuation2 = 2.0 / (light.Range * light.Range);
        
        auto lightIndex = 1;
        (*d3dDevice)->lpVtbl->LightEnable(*d3dDevice, lightIndex, 1);
        (*d3dDevice)->lpVtbl->SetLight(*d3dDevice, lightIndex, &light);
    }
    
    reinterpret_cast<decltype(EnableNrelPointLight)*>(0x008060fc)();
}

bool __cdecl LoadSnowMapStuff()
{
    // Replace SetDataTable (before warp_load_assets)
    auto map = GetCurrentMap();
    (*setDataTable)[(size_t) map] = snowSetDataTable;

    reinterpret_cast<bool (__cdecl *)(Map::MapType)>(0x00815584)(map); // load_map_sound_data_
    if (reinterpret_cast<bool (__cdecl *)()>(0x00781fc4)()) { // warp_load_assets
        return false;
    }

    // Replace fog (before unknown_create_map)
    auto fogIndex = (size_t) map;
    auto fog = ReadFogFile("data/fog_snow.txt");
    originalFogEntry = (*fogEntries)[fogIndex];
    (*fogEntries)[fogIndex] = fog;
    
    // Replace sunlight (before create_map_sunlight_from_lightentry)
    auto lightEntryIndex = (size_t) map;
    auto light = ReadLightFile("data/sun_snow.txt");
    if (IsUltEp1()) lightEntryIndex += 48;
    originalLightEntry = (*lightEntries)[lightEntryIndex];
    (*lightEntries)[lightEntryIndex] = light;

    reinterpret_cast<void (__cdecl *)()>(0x00793f64)(); // create_map_sunlight_from_lightentry()
    reinterpret_cast<void (__cdecl *)()>(0x00782098)(); // unknown_create_map()

    Slbgm::LoadSlbgm(snowMapEntry.slbgmIndex);
    
    PatchIcecubeUnitxt();
    *reinterpret_cast<decltype(EnableNrelPointLight)**>(0x00b475f8) = EnableNrelPointLight;

    return true;
}

void __cdecl UnloadSnowMapStuff()
{
    // Restore replaced fog
    auto map = GetCurrentMap();
    (*fogEntries)[(size_t) map] = originalFogEntry;
    
    // Restore replaced sunlight
    auto lightEntryIndex = (size_t) map;
    if (IsUltEp1()) lightEntryIndex += 48;
    (*lightEntries)[lightEntryIndex] = originalLightEntry;
    
    // Restore SetDataTable
    (*setDataTable)[(size_t) map] = originalSetDataTable;

    // Call normal unload stuff
    reinterpret_cast<decltype(MapLoader::Unload)>(0x007a6dd8)();
}

CustomMapDefinition snowMapEntry = {
    .assetPrefixes = snowMapAssetPrefixes,
    .mapLoader = {
        snowMapName,
        LoadSnowMapStuff,
        UnloadSnowMapStuff
    },
    .songFilename = "slbgm_snow.ogg",
    .slbgmFilePath = "data/slbgm_snow.txt",
    .slbgmIndex = 0,
    .allowedMonsters = {
        Enemy::GetEnemyDefinition(Enemy::NpcType::Booma),
        Enemy::GetEnemyDefinition(Enemy::NpcType::SavageWolf),
        SpawnableEntityWithCloneCount(1338, EnemyIcecube, 1)
    },
    .allowedObjects = {
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::PlayerSet1),
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::EventCollision),
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::FloatingJelifish),
        SpawnableEntity(1337, MapObjectCloud),
        SpawnableEntity(1338, MapObjectSnowfall),
        SpawnableEntity(1339, MapObjectNewdoor)
    }
};

#endif // PATCH_NEWMAP
