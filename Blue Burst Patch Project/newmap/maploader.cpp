#include "newmap/footsteps.h"
#ifdef PATCH_NEWMAP

#include <cstdint>

#include "../common.h"
#include "fog.h"
#include "maploader.h"
#include "newmap.h"
#include "setdata.h"
#include "sunlight.h"
#include "particle_effects.h"
#include "slbgm.h"

static MapLoader* mapLoaders = reinterpret_cast<MapLoader*>(0x00a160c0);
static const MapLoader* originalMapLoaders = []() {
    size_t originalArrayLength = 0;
    for (auto cursor = mapLoaders; cursor->name != nullptr; cursor++)
        originalArrayLength++;

    // Copy original to heap
    auto originalCopy = new MapLoader[originalArrayLength];
    std::copy(mapLoaders, mapLoaders + originalArrayLength, originalCopy);
    return originalCopy;
}();

bool __cdecl DefaultMapLoad()
{
    auto origMap = (uint8_t) GetCurrentMap();
    auto mapDef = GetCustomMapDefinition(origMap);

    // Replace SetDataTable (before warp_load_assets)
    ReplaceSetData(origMap, mapDef->setDataTable);

    reinterpret_cast<bool (__cdecl *)(uint8_t)>(0x00815584)(origMap); // load_map_sound_data_
    if (reinterpret_cast<bool (__cdecl *)()>(0x00781fc4)()) // warp_load_assets
        return false;
    
    ReplaceMapParticleEffects(origMap, "data/particles_snow.txt");

    // Replace fog (before unknown_create_map)
    ReplaceMapFog(origMap, "data/fog_snow.txt");
    
    // Replace sunlight (before create_map_sunlight_from_lightentry)
    ReplaceMapSunlight(origMap, "data/sun_snow.txt");
    
    ReplaceMapFootsteps(origMap, "data/footsteps_snow.txt");

    reinterpret_cast<void (__cdecl *)()>(0x00793f64)(); // create_map_sunlight_from_lightentry()
    reinterpret_cast<void (__cdecl *)()>(0x00782098)(); // unknown_create_map()

    LoadSlbgm(mapDef->slbgmIndex);

    // Call optional arbitrary map-specific code
    if (mapDef->mapLoader.Load != nullptr)
        mapDef->mapLoader.Load();

    return true;
}

void __cdecl DefaultMapUnload()
{
    auto origMap = (uint8_t) GetCurrentMap();
    auto mapDef = GetCustomMapDefinition(origMap);
    
    RestoreMapParticleEffects(origMap);

    // Restore replaced fog
    RestoreMapFog(origMap);
    
    // Restore replaced sunlight
    RestoreMapSunlight(origMap);
    
    // Restore SetDataTable
    RestoreSetData(origMap);
    
    RestoreMapFootsteps(origMap);

    // Call normal unload stuff
    reinterpret_cast<decltype(MapLoader::Unload)>(0x007a6dd8)();

    if (mapDef->mapLoader.Unload != nullptr)
        mapDef->mapLoader.Unload();
}

void PatchMapLoaders()
{
    // References to first field
    for (auto addr : {0x007a808c + 3})
        *reinterpret_cast<decltype(MapLoader::name)**>(addr) = &mapLoaders[0].name;
    
    // References to second field
    for (auto addr : {0x007a8915 + 3, 0x00815762 + 3, 0x00815776 + 3})
        *reinterpret_cast<decltype(MapLoader::Load)**>(addr) = &mapLoaders[0].Load;
    
    // References to third field
    for (auto addr : {0x008155c9 + 3, 0x008155da + 3, 0x0081561d + 3, 0x0081562e + 3})
        *reinterpret_cast<decltype(MapLoader::Unload)**>(addr) = &mapLoaders[0].Unload;
}

void ReplaceMapLoader(uint8_t origMap, const MapLoader& loader)
{
    mapLoaders[origMap] = loader;
}

void RestoreMapLoader(uint8_t origMap)
{
    mapLoaders[origMap] = originalMapLoaders[origMap];
}

#endif
