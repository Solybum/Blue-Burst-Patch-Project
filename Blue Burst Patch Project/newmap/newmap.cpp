#ifdef PATCH_NEWMAP

#include <cstdint>
#include <cstdlib>
#include <unordered_map>
#include <vector>

#include "../common.h"
#include "../helpers.h"
#include "../quest.h"
#include "enemies_and_objects.h"
#include "maploader.h"
#include "newmap.h"
#include "slbgm.h"
#include "snow_map.h"
#include "sound_effects.h"

// Order of maps here defines the id of each custom map (0 based)
static std::vector<CustomMapDefinition*> customMaps =
{
    &snowMapEntry
};

static auto vanillaMapAssetPrefixes = reinterpret_cast<MapAssetPrefixes*>(0x00a116e0);
static auto vanillaUltMapAssetPrefixes = reinterpret_cast<MapAssetPrefixes*>(0x00a114e0);

static std::unordered_map<uint32_t, uint32_t> currentMapSubstitutions; // (Original map index, custom map index)

/**
 * @brief Custom maps will use the objects and enemies from the matching vanilla maps, but can have custom map geometry files.
 */
const MapAssetPrefixes::Prefixes* __cdecl GetMapAssetPrefixes(uint32_t map)
{
    if (!currentMapSubstitutions.count(map))
    {
        // Do vanilla behavior
        if (IsUltEp1()) return vanillaUltMapAssetPrefixes[map].prefixes;
        return vanillaMapAssetPrefixes[map].prefixes;
    }

    return customMaps[currentMapSubstitutions[map]]->assetPrefixes.prefixes;
}

__attribute__((regparm(1))) // Take argument in EAX
uint32_t __cdecl Before_InitEpisodeMaps(uint32_t episode)
{
    // This gets called when entering or leaving a game or the lobby and from set_episode opcode
    // (but not when going to main menu, but will get called when entering lobby again).
    // Seems like a good place to reset the map substitutions.
    for (auto [origMap, newMap] : currentMapSubstitutions)
    {
        RestoreMapLoader(origMap);
        RestoreMapPac(origMap);
        RestoreEnemiesAndObjects(origMap);
    }
    currentMapSubstitutions.clear();
    // Code we overwrote
    *reinterpret_cast<uint32_t*>(0x00aafdb8) = episode;
    // Return to original code
    return episode;
}

void __cdecl NewOpcodeDesignateCustomMap(uint8_t origMap, uint8_t newMap)
{
    currentMapSubstitutions.insert({origMap, newMap});
    
    auto mapDef = customMaps[newMap];
    ReplaceMapLoader(origMap, MapLoader {
        mapDef->mapLoader.name,
        DefaultMapLoad,
        DefaultMapUnload
    });
    ReplaceMapPac(origMap, mapDef->pacFilename, mapDef->pacMetadataFilename);
    ReplaceEnemiesAndObjects(origMap, mapDef->allowedMonsters, mapDef->allowedObjects);
}

void PatchMapDesignateOpcode()
{
    PatchJMP(0x0080bee8, 0x0080bf11, (int) GetMapAssetPrefixes);
    PatchCALL(0x0080c7a0, 0x0080c7a5, (int) Before_InitEpisodeMaps);
    Quest::SetOpcode(0xf962, Quest::SetupOpcodeOperand11, (void*) NewOpcodeDesignateCustomMap);
}

void ApplyNewMapPatch()
{
    PatchMapDesignateOpcode();
    PatchMapLoaders();
    PatchSoundEffectLimit();

    // Add slbgm definitions
    auto& slbgmDefs = GetSlbgmMetadata();
    for (auto& mapDef : customMaps)
    {
        slbgmDefs.push_back(new SlbgmDef(mapDef->songFilename, mapDef->slbgmFilePath));
        // Write index of map's slbgm data into mapDef to allow maploader to use it for loading slbgm
        mapDef->slbgmIndex = slbgmDefs.size() - 1;
    }
    ApplySlbgmPatch();
}

const CustomMapDefinition* GetCustomMapDefinition(uint8_t origMap)
{
    return customMaps.at(currentMapSubstitutions.at(origMap));
}

#endif // PATCH_NEWMAP
