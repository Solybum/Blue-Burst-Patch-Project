#ifdef PATCH_NEWMAP

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "../enemy.h"
#include "../map_object.h"

// Key is original map, value is original list
static std::unordered_map<uint8_t, std::vector<InitList::FunctionPair>> replacedInitLists;
static std::unordered_map<uint8_t, std::vector<Enemy::TaggedEnemyConstructor>> replacedEnemyLists;
static std::unordered_map<uint8_t, std::vector<MapObject::TaggedMapObjectConstructor>> replacedMapObjectLists;

void ReplaceEnemiesAndObjects(
    uint8_t origMap,
    const std::vector<Enemy::SpawnableDefinition>& enemies,
    const std::vector<MapObject::SpawnableDefinition>& objects)
{
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
    
    for (const auto& obj : objects)
    {
        if (obj.LoadAssets != nullptr)
            initList.AddFunctionPair(InitList::FunctionPair(obj.LoadAssets, obj.UnloadAssets));
        objectList.emplace_back(obj.id, obj.Create);
    }
    
    for (const auto& mon : enemies)
    {
        if (mon.LoadAssets != nullptr)
            initList.AddFunctionPair(InitList::FunctionPair(mon.LoadAssets, mon.UnloadAssets));
        monsterList.emplace_back(mon.id, mon.Create, mon.cloneCount);
    }

    initList.Patch();
    MapObject::PatchMapObjectConstructorLists();
    Enemy::PatchEnemyConstructorLists();
}

void RestoreEnemiesAndObjects(uint8_t origMap)
{
    auto& monsterList = Enemy::GetEnemyConstructorList((Map::MapType) origMap);
    monsterList = replacedEnemyLists[origMap];
    replacedEnemyLists.erase(origMap);
    Enemy::PatchEnemyConstructorLists();

    auto& objectList = MapObject::GetMapObjectConstructorList((Map::MapType) origMap);
    objectList = replacedMapObjectLists[origMap];
    replacedMapObjectLists.erase(origMap);
    MapObject::PatchMapObjectConstructorLists();
    
    auto& initList = Map::GetMapInitList((Map::MapType) origMap);
    initList.SetFunctions(replacedInitLists[origMap]);
    initList.Patch();
    replacedInitLists.erase(origMap);
}


#endif
