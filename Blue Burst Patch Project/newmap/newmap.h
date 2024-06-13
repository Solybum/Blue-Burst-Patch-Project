#pragma once

#include "enemy.h"
#include "map_object.h"
#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct MapLoader
{
    const char* name;
    bool (__cdecl *Load)();
    void (__cdecl *Unload)();
};

struct MapAssetPrefixes
{
    struct Prefixes {
        const char* basename;
        const char* variantNames[];
    }* prefixes;
    uint32_t variantCount;
};

struct SetDataTable
{
    struct Inner1 {
        struct Inner2 {
            const char* variantName1;
            const char* variantName2;
            const char* baseName;
        }* objectSetVariants;
        uint32_t count;
    }* mapVariants;
    uint32_t count;
};

extern SetDataTable** setDataTable;

#define SpawnableEntity(id, EntityType) \
    { id, \
      EntityType::LoadAssets, \
      EntityType::UnloadAssets, \
      EntityType::Create }
  
#define SpawnableEntityWithCloneCount(id, EntityType, cloneCount) \
    { id, \
      EntityType::LoadAssets, \
      EntityType::UnloadAssets, \
      EntityType::Create, \
      cloneCount }

struct CustomMapDefinition
{
    const MapAssetPrefixes assetPrefixes;
    const MapLoader mapLoader;
    const std::string songFilename;
    const std::string slbgmFilePath;
    size_t slbgmIndex; // Assigned by patch
    const std::vector<Enemy::SpawnableDefinition> allowedMonsters;
    const std::vector<MapObject::SpawnableDefinition> allowedObjects;
};
#pragma pack(pop)


void ApplyNewMapPatch();
