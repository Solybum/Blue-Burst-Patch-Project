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

    bool patchedConstructorLists = false;

    void PatchEnemyConstructorLists()
    {
        if (patchedConstructorLists) return;

        patchedConstructorLists = true;

        // Write used lists
        for (const auto& entry : enemyConstructorListCache)
        {
            auto map = entry.first;
            auto enemyList = entry.second;

            // Add terminator
            enemyList.push_back(enemyListTerminator);

            // Copy to heap (deliberate leak) and write new list to table
            TaggedEnemyConstructor* copy = new TaggedEnemyConstructor[enemyList.size()];
            std::copy(enemyList.begin(), enemyList.end(), copy);
            mapEnemyTable[(size_t) map] = copy;
        }
    }

    BmlData::LoadBmlFunction LoadBml = reinterpret_cast<BmlData::LoadBmlFunction>(0x0051f6b0);
    BmlData::FreeBmlFunction FreeBml = reinterpret_cast<BmlData::FreeBmlFunction>(0x004066a8);

    void** rootEnemyObject = reinterpret_cast<void**>(0x00aca360);

    EnemyBase::EnemyBase(void* parentObject)
    {
        // Call original constructor
        Constructor(this, parentObject);

        // Inherit vtable
        vtable = new Vtable(*origVtable);

        OVERRIDE_METHOD(EnemyBase, Destruct);
    }

    void EnemyBase::Destruct(bool32 freeMemory)
    {
        // Superdestructors will reassign the vtable so it's fine to delete our vtable here
        delete vtable;
        vtable = nullptr;

        // Recursively call superdestructors, without letting them deallocate
        origVtable->Destruct(this, false);

        // We can deallocate our own memory if this object was not derived further
        if (freeMemory)
        {
            MainArenaDealloc(this);
        }
    }

    const EnemyBase::ConstructorFunction EnemyBase::Constructor = reinterpret_cast<EnemyBase::ConstructorFunction>(0x007865e8);
    const EnemyBase::Vtable* EnemyBase::origVtable = reinterpret_cast<Vtable*>(0x00b44b40);

    InsertIntoEntityListFunction InsertIntoEntityList = reinterpret_cast<InsertIntoEntityListFunction>(0x007b4f54);

    decltype(InitCollisionBoxes) InitCollisionBoxes = reinterpret_cast<decltype(InitCollisionBoxes)>(0x007b9a88);

    decltype(SetStatsFromBattleParams) SetStatsFromBattleParams = reinterpret_cast<decltype(SetStatsFromBattleParams)>(0x007868f8);

    CollisionBox::CollisionBox(float x_, float y_, float z_, float r_)
    {
        x = x_;
        y = y_;
        z = z_;
        r = r_;
        unknownFlags = 0x8003;
    }
};
