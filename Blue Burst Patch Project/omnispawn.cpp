#include "omnispawn.h"
#include "map.h"
#include "enemy.h"

using Enemy::NpcType;
using Map::MapType;

void PatchMapEnemyLists()
{
    // As a test, allow certain enemies to spawn in the falz shrine area
    auto falzEnemies = Enemy::ReadEntriesIntoEnemyConstructorList(Enemy::mapEnemyTable[(size_t) MapType::Boss_Darkfalz]);

    NpcType newEnemies[] = {
        NpcType::Booma,
        NpcType::Hildebear,
        NpcType::Rag_Rappy_Sand_Rappy,
        NpcType::Poison_Lily_Del_Lily,
        NpcType::Gillchic,
        NpcType::Evil_Shark,
        NpcType::Nano_Dragon,
        NpcType::Garanz,
        NpcType::Canadine,
        NpcType::Sinow_Beat,
        NpcType::Delsaber,
        NpcType::Chaos_Bringer,
        NpcType::Chaos_Sorcerer,
        NpcType::Dark_Gunner,
        NpcType::Darth_Belra,
        NpcType::Dimenian,
        NpcType::Delbiter
    };

    for (NpcType newType : newEnemies)
    {
        Enemy::TaggedEnemyConstructor* enemy = Enemy::FindEnemyConstructor(newType);

        if (enemy != nullptr)
        {
            falzEnemies.push_back(*enemy);
        }
    }

    falzEnemies.push_back(Enemy::enemyListTerminator);
    Enemy::mapEnemyTable[(size_t) Map::MapType::Boss_Darkfalz] = Enemy::CopyEnemyConstructorListToHeap(falzEnemies);
}

void PatchMapInitLists()
{
    // As a test, append all previous map init function lists to the falz area init list
    auto falzFuncs = Map::ReadEntriesIntoMapInitList(Map::mapInitFunctionTable[(size_t) MapType::Boss_Darkfalz]);

    for (size_t i = 0; i < (size_t) MapType::Boss_Darkfalz; i++) {
        auto list = Map::ReadEntriesIntoMapInitList(Map::mapInitFunctionTable[i]);
        falzFuncs.insert(falzFuncs.end(), list.begin(), list.end());
    }

    falzFuncs.push_back(Map::mapInitListTerminator);
    Map::mapInitFunctionTable[(size_t) MapType::Boss_Darkfalz] = Map::CopyMapInitFunctionListToHeap(falzFuncs);
}

bool patchApplied = false;

void ApplyOmnispawnPatch()
{
    if (patchApplied) {
        return;
    }

    PatchMapEnemyLists();
    PatchMapInitLists();

    patchApplied = true;
}
