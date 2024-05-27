#ifdef PATCH_NEWMAP

#include <cstddef>
#include <cstdlib>

#include "snow_map.h"
#include "fog.h"
#include "sunlight.h"
#include "slbgm.h"
#include "../map.h"

static const char* snowMapName = "SNOW01";
static MapAssetPrefixes::Prefixes snowMapAssetPrefixStrings = {
    "map_snow01",
    "map_snow01"
};

static FogEntry originalFogEntry;
static LightEntry originalLightEntry;

bool __cdecl LoadSnowMapStuff()
{
    auto map = GetCurrentMap();
    reinterpret_cast<bool (__cdecl *)(Map::MapType)>(0x00815584)(map); // load_map_sound_data_
    if (reinterpret_cast<bool (__cdecl *)()>(0x00781fc4)()) { // warp_load_assets
      return false;
    }

    // Replace fog
    auto fogIndex = (size_t) map;
    auto fog = ReadFogFile("data/fog_snow.txt");
    originalFogEntry = (*fogEntries)[fogIndex];
    (*fogEntries)[fogIndex] = fog;
    
    // Replace sunlight
    auto lightEntryIndex = (size_t) map;
    auto light = ReadLightFile("data/sun_snow.txt");
    if (IsUltEp1()) lightEntryIndex += 48;
    originalLightEntry = (*lightEntries)[lightEntryIndex];
    (*lightEntries)[lightEntryIndex] = light;

    reinterpret_cast<void (__cdecl *)()>(0x00793f64)(); // unk_load_lightentry()
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

    // Call normal unload stuff
    reinterpret_cast<decltype(MapLoader::Unload)>(0x007a6dd8)();
}

CustomMapDefinition snowMapEntry = {
    MapAssetPrefixes { &snowMapAssetPrefixStrings, 1 },
    MapLoader {
        snowMapName,
        LoadSnowMapStuff,
        UnloadSnowMapStuff
    },
    "slbgm_snow.ogg",
    "data/slbgm_snow.txt",
    0
};

#endif // PATCH_NEWMAP
