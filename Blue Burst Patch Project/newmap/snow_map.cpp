#ifdef PATCH_NEWMAP

#include "snow_map.h"
#include "slbgm.h"
#include "../map.h"

static const char* snowMapName = "SNOW01";
static MapAssetPrefixes::Prefixes snowMapAssetPrefixStrings = {
    "map_snow01",
    "map_snow01"
};

bool __cdecl LoadSnowMapStuff()
{
    auto map = GetCurrentMap();
    reinterpret_cast<bool (__cdecl *)(Map::MapType)>(0x00815584)(map); // load_map_sound_data_
    if (reinterpret_cast<bool (__cdecl *)()>(0x00781fc4)()) { // warp_load_assets
      return false;
    }
    reinterpret_cast<void (__cdecl *)()>(0x00793f64)(); // unk_load_lightentry()
    reinterpret_cast<void (__cdecl *)()>(0x00782098)(); // unknown_create_map()
    Slbgm::LoadSlbgm(snowMapEntry.slbgmIndex);
    return true;
}

CustomMapDefinition snowMapEntry = {
    MapAssetPrefixes { &snowMapAssetPrefixStrings, 1 },
    MapLoader {
        snowMapName,
        LoadSnowMapStuff,
        reinterpret_cast<decltype(MapLoader::Unload)>(0x007a6dd8)
    },
    "slbgm_snow.ogg",
    "data/slbgm_snow.txt",
    0
};


#endif // PATCH_NEWMAP
