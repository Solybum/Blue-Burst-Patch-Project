#pragma once

#include <cstdint>
#include <string>

#include "../enemy.h"
#include "map_object.h"
#include "maploader.h"
#include "setdata.h"

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

#pragma pack(push, 1)
struct MapAssetPrefixes
{
    struct Prefixes {
        const char* basename;
        const char* variantNames[];
    }* prefixes;
    uint32_t variantCount;
};

struct CustomMapDefinition
{
    const MapAssetPrefixes assetPrefixes;
    const SetDataTable setDataTable;
    const MapLoader mapLoader;
    const std::string songFilename;
    const std::string slbgmFilePath;
    const std::string pacFilename;
    const std::string pacMetadataFilename;
    size_t slbgmIndex; // Assigned by patch
    const std::vector<Enemy::SpawnableDefinition> allowedMonsters;
    const std::vector<MapObject::SpawnableDefinition> allowedObjects;
};
#pragma pack(pop)

void ApplyNewMapPatch();

const CustomMapDefinition* GetCustomMapDefinition(uint8_t origMap);
