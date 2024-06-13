#ifdef PATCH_NEWMAP

#include <cstdint>
#include <cstdlib>
#include <unordered_map>
#include <vector>

#include "../common.h"
#include "../quest.h"
#include "../map.h"
#include "enemy.h"
#include "initlist.h"
#include "newmap.h"
#include "snow_map.h"
#include "map_object.h"
#include "slbgm.h"
#include "sound_effects.h"

SetDataTable** setDataTable = reinterpret_cast<SetDataTable**>(0x00aafdd0);

static std::vector<CustomMapDefinition*> customMaps = {
    &snowMapEntry
};

// Key is original map, value is original list
static std::unordered_map<uint8_t, std::vector<InitList::FunctionPair>> replacedInitLists;
static std::unordered_map<uint8_t, std::vector<Enemy::TaggedEnemyConstructor>> replacedEnemyLists;
static std::unordered_map<uint8_t, std::vector<MapObject::TaggedMapObjectConstructor>> replacedMapObjectLists;

#pragma pack(push, 1)
struct MapDesignation
{
    uint32_t map_id;
    uint32_t map_variant;
    uint32_t object_set;
};
#pragma pack(pop)

static const size_t VANILLA_FLOOR_COUNT = 18;
static const size_t VANILLA_MAP_COUNT = 47;
static auto vanillaFloorMapDesignations = reinterpret_cast<MapDesignation*>(0x00aafce0);
static auto vanillaMapAssetPrefixes = reinterpret_cast<MapAssetPrefixes*>(0x00a116e0);
static auto vanillaUltMapAssetPrefixes = reinterpret_cast<MapAssetPrefixes*>(0x00a114e0);

static MapLoader* mapLoaders = reinterpret_cast<MapLoader*>(0x00a160c0);
static const MapLoader* originalMapLoaders = []() {
    size_t originalArrayLength = 0;
    for (auto cursor = mapLoaders; cursor->name != nullptr; cursor++)
        originalArrayLength++;
    
    auto originalCopy = new MapLoader[originalArrayLength];
    std::copy(mapLoaders, mapLoaders + originalArrayLength, originalCopy);
    return originalCopy;
}();

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
    for (const auto& [origMap, newMap] : currentMapSubstitutions)
    {
        mapLoaders[origMap] = originalMapLoaders[origMap];

        auto& monsterList = Enemy::GetEnemyConstructorList((Map::MapType) origMap);
        monsterList = replacedEnemyLists[origMap];

        auto& objectList = MapObject::GetMapObjectConstructorList((Map::MapType) origMap);
        objectList = replacedMapObjectLists[origMap];
        
        auto& initList = Map::GetMapInitList((Map::MapType) origMap);
        initList.SetFunctions(replacedInitLists[origMap]);
        initList.Patch();
        
        RestoreMapPac(origMap);
    }
    Enemy::PatchEnemyConstructorLists();
    MapObject::PatchMapObjectConstructorLists();
    replacedEnemyLists.clear();
    replacedMapObjectLists.clear();
    replacedInitLists.clear();
    currentMapSubstitutions.clear();
    // Code we overwrote
    *reinterpret_cast<uint32_t*>(0x00aafdb8) = episode;
    // Return to original code
    return episode;
}

void __cdecl NewOpcodeDesignateCustomMap(uint8_t origMap, uint8_t newMap)
{
    currentMapSubstitutions.insert({origMap, newMap});

    // Replace map loader
    mapLoaders[origMap] = customMaps[newMap]->mapLoader;
    
    // Replace enemies and objects
    auto& initList = Map::GetMapInitList((Map::MapType) origMap);
    replacedInitLists[origMap] = initList.GetFunctions();
    initList.Clear();
    initList.AddFunctionPair({0x00578d44, 0x00578128}); // load_bm_ene_common_all.bml
    initList.AddFunctionPair({0x00521950, 0x0051f59c}); // unknown_map_init_common
    
    auto& objectList = MapObject::GetMapObjectConstructorList((Map::MapType) origMap);
    replacedMapObjectLists[origMap] = objectList;
    objectList.clear();
    
    auto& monsterList = Enemy::GetEnemyConstructorList((Map::MapType) origMap);
    replacedEnemyLists[origMap] = monsterList;
    monsterList.clear();
    
    for (const auto& obj : customMaps[newMap]->allowedObjects)
    {
        if (obj.LoadAssets != nullptr)
            initList.AddFunctionPair(InitList::FunctionPair(obj.LoadAssets, obj.UnloadAssets));
        objectList.emplace_back(obj.id, obj.Create);
    }
    
    for (const auto& mon : customMaps[newMap]->allowedMonsters)
    {
        if (mon.LoadAssets != nullptr)
            initList.AddFunctionPair(InitList::FunctionPair(mon.LoadAssets, mon.UnloadAssets));
        monsterList.emplace_back(mon.id, mon.Create, mon.cloneCount);
    }

    initList.Patch();
    MapObject::PatchMapObjectConstructorLists();
    Enemy::PatchEnemyConstructorLists();

    // Replace sound effects
    ReplaceMapPac(origMap, "snow.pac", "snow_pac.txt");
}

void PatchMapDesignateOpcode()
{
    PatchJMP(0x0080bee8, 0x0080bf11, (int) GetMapAssetPrefixes);
    PatchCALL(0x0080c7a0, 0x0080c7a5, (int) Before_InitEpisodeMaps);
    Quest::SetOpcode(0xf962, Quest::SetupOpcodeOperand11, (void*) NewOpcodeDesignateCustomMap);
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

void ApplyNewMapPatch()
{
    PatchMapDesignateOpcode();

    PatchMapLoaders();
    
    PatchSoundEffectLimit();

    // Add slbgm definitions
    auto& slbgmDefs = Slbgm::GetAllTransitionData();
    for (auto& mapDef : customMaps)
    {
        slbgmDefs.push_back(new Slbgm::SlbgmDef(mapDef->songFilename, mapDef->slbgmFilePath));
        mapDef->slbgmIndex = slbgmDefs.size() - 1;
    }
    Slbgm::ApplySlbgmPatch();
}

#endif // PATCH_NEWMAP
