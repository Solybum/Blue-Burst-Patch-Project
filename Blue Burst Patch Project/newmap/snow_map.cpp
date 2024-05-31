#ifdef PATCH_NEWMAP

#include <cstddef>
#include <cstdlib>

#include "enemy.h"
#include "snow_map.h"
#include "fog.h"
#include "newmap.h"
#include "../map.h"
#include "map_object.h"
#include "map_object_cloud.h"
#include "map_object_snowfall.h"
#include "map_object_newdoor.h"
#include "sunlight.h"
#include "slbgm.h"

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
        Enemy::GetEnemyDefinition(Enemy::NpcType::SavageWolf)
    },
    .allowedObjects = {
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::PlayerSet1),
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::FloatingJelifish),
        SpawnableEntity(1337, MapObjectCloud),
        SpawnableEntity(1338, MapObjectSnowfall),
        SpawnableEntity(1339, MapObjectNewdoor)
    }
};

#endif // PATCH_NEWMAP
