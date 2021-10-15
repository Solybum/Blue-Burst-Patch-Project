#include <vector>
#include "enemy_spawns.h"
#include "map.h"

namespace EnemySpawns
{
    TaggedEnemyConstructor** mapEnemyTable = reinterpret_cast<TaggedEnemyConstructor**>(0x009fba60);

    std::vector<TaggedEnemyConstructor> ReadEntriesIntoEnemyConstructorList(TaggedEnemyConstructor* entries)
    {
        std::vector<TaggedEnemyConstructor> enemyList;

        while (entries->enemyType != NpcType::ListTerminator)
        {
            enemyList.push_back(*entries);
            entries++;
        }

        return enemyList;
    }

    TaggedEnemyConstructor* CopyEnemyConstructorListToHeap(const std::vector<TaggedEnemyConstructor>& enemyList)
    {
        TaggedEnemyConstructor* copy = new TaggedEnemyConstructor[enemyList.size()];
        std::copy(enemyList.begin(), enemyList.end(), copy);
        return copy;
    }

    TaggedEnemyConstructor* FindEnemyConstructor(NpcType needle)
    {
        TaggedEnemyConstructor** currentList = mapEnemyTable;

        while (*currentList != nullptr)
        {
            TaggedEnemyConstructor* currentEntry = *currentList;

            while (currentEntry->enemyType != NpcType::ListTerminator)
            {
                if (currentEntry->enemyType == needle)
                {
                    return currentEntry;
                }

                currentEntry++;
            }

            currentList++;
        }

        return nullptr;
    }

    bool patchApplied = false;

    void ApplyEnemySpawnPatch()
    {
        if (patchApplied) {
            return;
        }

        // As a test, allow certain enemies to spawn in the falz shrine area
        auto falzEnemies = ReadEntriesIntoEnemyConstructorList(mapEnemyTable[(size_t) Map::MapType::Boss_Darkfalz]);

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
            TaggedEnemyConstructor* enemy = FindEnemyConstructor(newType);

            if (enemy != nullptr)
            {
                falzEnemies.push_back(*enemy);
            }
        }

        falzEnemies.push_back(enemyListTerminator);
        mapEnemyTable[(size_t) Map::MapType::Boss_Darkfalz] = CopyEnemyConstructorListToHeap(falzEnemies);

        patchApplied = true;
    }
};
