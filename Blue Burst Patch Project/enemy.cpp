#include "enemy.h"
#include "map.h"

namespace Enemy
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
};
