#include <unordered_map>
#include "enemy.h"
#include "map.h"
#include "object_extension.h"

namespace Enemy
{
    /// Each map has a list of enemies that can be spawned in it. Index is map.
    TaggedEnemyConstructor** mapEnemyTable = reinterpret_cast<TaggedEnemyConstructor**>(0x009fba60);

    TaggedEnemyConstructor::TaggedEnemyConstructor(NpcType type, EnemyConstructor ctor) :
        enemyType(type), constructor(ctor), unknown1(0), unknown2(250000.0), defaultCloneCount(0) {}

    TaggedEnemyConstructor::TaggedEnemyConstructor() :
        enemyType(NpcType::Invalid), constructor(nullptr), unknown1(0), unknown2(250000.0), defaultCloneCount(0) {}

    const TaggedEnemyConstructor enemyListTerminator= TaggedEnemyConstructor(NpcType::ListTerminator, nullptr);

    /// Save used lists here
    std::unordered_map<Map::MapType, std::vector<TaggedEnemyConstructor>> enemyConstructorListCache;

    std::vector<TaggedEnemyConstructor>& GetEnemyConstructorList(Map::MapType map)
    {
        // Get cached or read new
        auto found = enemyConstructorListCache.find(map);

        if (found != enemyConstructorListCache.end())
        {
            return (*found).second;
        }

        std::vector<TaggedEnemyConstructor> enemyList;
        auto entries = mapEnemyTable[(size_t) map];

        while (entries->enemyType != NpcType::ListTerminator)
        {
            enemyList.push_back(*entries);
            entries++;
        }

        enemyConstructorListCache[map] = enemyList;

        return enemyConstructorListCache[map];
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
};
