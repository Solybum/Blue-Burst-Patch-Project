#pragma once

#include <cstdint>
#include <vector>

#include "../enemy.h"
#include "../map_object.h"

void ReplaceEnemiesAndObjects(
    uint8_t origMap,
    const std::vector<Enemy::SpawnableDefinition>& enemies,
    const std::vector<MapObject::SpawnableDefinition>& objects);
void RestoreEnemiesAndObjects(uint8_t origMap);
