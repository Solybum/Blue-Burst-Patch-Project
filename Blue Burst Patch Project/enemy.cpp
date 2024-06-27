#include <cstdint>
#include <unordered_map>

#include "enemy.h"
#include "map.h"
#include "object_extension.h"
#include "object.h"

namespace Enemy
{
    SpawnableDefinition::SpawnableDefinition(NpcType id, size_t loadAssetsAddr, size_t unloadAssetsAddr, size_t createAddr)
        : id((uint16_t) id),
          LoadAssets(reinterpret_cast<decltype(LoadAssets)>(loadAssetsAddr)),
          UnloadAssets(reinterpret_cast<decltype(UnloadAssets)>(unloadAssetsAddr)),
          Create(reinterpret_cast<decltype(Create)>(createAddr)),
          cloneCount(0) {}

    SpawnableDefinition::SpawnableDefinition(uint16_t id,
            decltype(LoadAssets) LoadAssets,
            decltype(UnloadAssets) UnloadAssets,
            decltype(Create) Create,
            uint32_t cloneCount)
        : id(id),
          LoadAssets(LoadAssets),
          UnloadAssets(UnloadAssets),
          Create(Create),
          cloneCount(cloneCount) {}

    /// Each map has a list of enemies that can be spawned in it. Index is map.
    TaggedEnemyConstructor** mapEnemyTable = reinterpret_cast<TaggedEnemyConstructor**>(0x009fba60);

    const std::unordered_map<NpcType, SpawnableDefinition>& GetEnemyDefinitions()
    {
        static std::unordered_map<NpcType, SpawnableDefinition> enemyDefinitions =
        {
            {NpcType::Monest               , SpawnableDefinition(NpcType::Monest, 0x0051ad94, 0x00519120, 0x519144)},
            {NpcType::SavageWolf           , SpawnableDefinition(NpcType::SavageWolf, 0x0051ea94, 0x0051b24c, 0x51b26c)},
            {NpcType::PoisonLily_DelLily   , SpawnableDefinition(NpcType::PoisonLily_DelLily, 0x00529cf8, 0x0052930c, 0x529330)},
            {NpcType::SinowBeat            , SpawnableDefinition(NpcType::SinowBeat, 0x0059051c, 0x0058d268, 0x58d314)},
            {NpcType::Canadine             , SpawnableDefinition(NpcType::Canadine, 0x005420f4, 0x0053e718, 0x53e770)},
            {NpcType::ChaosSorcerer        , SpawnableDefinition(NpcType::ChaosSorcerer, 0x0059d6ac, 0x0059b164, 0x59b1a4)},
            {NpcType::ChaosBringer         , SpawnableDefinition(NpcType::ChaosBringer, 0x0053c914, 0x0053a378, 0x53a3d8)},
            {NpcType::DarkBelra            , SpawnableDefinition(NpcType::DarkBelra, 0x00538ef4, 0x00536b0c, 0x536b80)},
            {NpcType::RagRappy_SandRappy   , SpawnableDefinition(NpcType::RagRappy_SandRappy, 0x00527934, 0x00526c64, 0x526cc4)},
            {NpcType::NanoDragon           , SpawnableDefinition(NpcType::NanoDragon, 0x0057fae0, 0x0057f5c4, 0x57f5e4)},
            {NpcType::Gillchic_Dubchic     , SpawnableDefinition(NpcType::Gillchic_Dubchic, 0x005569bc, 0x00556528, 0x556600)},
            {NpcType::Garanz               , SpawnableDefinition(NpcType::Garanz, 0x0056fc10, 0x0056f8b8, 0x56f8f4)},
            {NpcType::DarkGunner           , SpawnableDefinition(NpcType::DarkGunner, 0x005462b0, 0x00545e10, 0x545e54)},
            {NpcType::Bulclaw              , SpawnableDefinition(NpcType::Bulclaw, 0x00531808, 0x005317c8, 0x533f90)},
            {NpcType::PofuillySlime        , SpawnableDefinition(NpcType::PofuillySlime, 0x00599a20, 0x00595ba8, 0x59594c)},
            {NpcType::PanArms              , SpawnableDefinition(NpcType::PanArms, 0x00583450, 0x0058335c, 0x582e44)},
            {NpcType::Dubwitch             , SpawnableDefinition(NpcType::Dubwitch, 0x006194e4, 0x00618a48, 0x618a68)},
            {NpcType::Hildebear            , SpawnableDefinition(NpcType::Hildebear, 0x00516124, 0x005152e4, 0x515388)},
            {NpcType::Booma                , SpawnableDefinition(NpcType::Booma, 0x00536314, 0x00535d90, 0x535e50)},
            {NpcType::GrassAssassin        , SpawnableDefinition(NpcType::GrassAssassin, 0x005255b8, 0x00524888, 0x5248fc)},
            {NpcType::EvilShark            , SpawnableDefinition(NpcType::EvilShark, 0x0051303c, 0x00512a44, 0x512b04)},
            {NpcType::Delsaber             , SpawnableDefinition(NpcType::Delsaber, 0x005511a4, 0x00550030, 0x5500c0)},
            {NpcType::Dimenian             , SpawnableDefinition(NpcType::Dimenian, 0x00552198, 0x00551bb0, 0x551c70)},
            {NpcType::SinowBerill          , SpawnableDefinition(NpcType::SinowBerill, 0x005a155c, 0x0059dfa8, 0x59e01c)},
            {NpcType::Gee                  , SpawnableDefinition(NpcType::Gee, 0x0055f7dc, 0x0055c918, 0x55c974)},
            {NpcType::Delbiter             , SpawnableDefinition(NpcType::Delbiter, 0x0054d724, 0x0054a680, 0x54a6a0)},
            {NpcType::GiGue                , SpawnableDefinition(NpcType::GiGue, 0x0056c2e0, 0x0056bda4, 0x56bde8)},
            {NpcType::EpsilonDisambiguator , SpawnableDefinition(NpcType::Epsilon_SinowZoa, 0x00557f90, 0x00557f18, 0x00558530)},
            {NpcType::IllGill              , SpawnableDefinition(NpcType::IllGill, 0x00531240, 0x0052e25c, 0x52e380)},
            {NpcType::Deldepth             , SpawnableDefinition(NpcType::Deldepth, 0x00549db0, 0x00549a60, 0x549900)},
            {NpcType::Mericarol            , SpawnableDefinition(NpcType::Mericarol, 0x00573638, 0x00572b24, 0x572b84)},
            {NpcType::UlGibbon             , SpawnableDefinition(NpcType::UlGibbon, 0x00568fb0, 0x005649c8, 0x5649e8)},
            {NpcType::Gibbles              , SpawnableDefinition(NpcType::Gibbles, 0x00564440, 0x005630fc, 0x563134)},
            {NpcType::Morfos               , SpawnableDefinition(NpcType::Morfos, 0x0057c830, 0x0057a094, 0x57a0b8)},
            {NpcType::Recobox              , SpawnableDefinition(NpcType::Recobox, 0x00589d08, 0x005884fc, 0x588520)},
            {NpcType::SinowZoaDisambiguator, SpawnableDefinition(NpcType::Epsilon_SinowZoa, 0x00594c30, 0x0059147c, 0x005914dc)},
            {NpcType::Merillia             , SpawnableDefinition(NpcType::Merillia, 0x005762cc, 0x00575d1c, 0x575da4)},
            {NpcType::Dolmolm              , SpawnableDefinition(NpcType::Dolmolm, 0x00554114, 0x00553af0, 0x553bb0)},
            {NpcType::Boota                , SpawnableDefinition(NpcType::Boota, 0x005a60cc, 0x005a5b48, 0x5a5c08)},
            {NpcType::Zu                   , SpawnableDefinition(NpcType::Zu, 0x005b4f5c, 0x005b4798, 0x5b47b8)},
            {NpcType::Astark               , SpawnableDefinition(NpcType::Astark, 0x005a4c3c, 0x005a3cac, 0x5a3d60)},
            {NpcType::SatelliteLizard      , SpawnableDefinition(NpcType::SatelliteLizard, 0x005b21e4, 0x005ae7ac, 0x5ae7cc)},
            {NpcType::Dorphon              , SpawnableDefinition(NpcType::Dorphon, 0x005a9adc, 0x005a671c, 0x5a673c)},
            {NpcType::Goran                , SpawnableDefinition(NpcType::Goran, 0x005adfc8, 0x005ad9e8, 0x5adac4)},
            {NpcType::MerissaA             , SpawnableDefinition(NpcType::MerissaA, 0x005b70ac, 0x005b6f0c, 0x5b6b24)},
            {NpcType::Girtablulu           , SpawnableDefinition(NpcType::Girtablulu, 0x005ac6e4, 0x005ab954, 0x5ab9ac)},
        };
        return enemyDefinitions;
    }
    
    const SpawnableDefinition& GetEnemyDefinition(NpcType tp)
    {
        return GetEnemyDefinitions().at(tp);
    }

    TaggedEnemyConstructor::TaggedEnemyConstructor(uint16_t type, EnemyConstructor ctor) :
        enemyType(type), constructor(ctor), unknown1(0), unknown2(250000.0), defaultCloneCount(0) {}
    
    TaggedEnemyConstructor::TaggedEnemyConstructor(uint16_t type, EnemyConstructor ctor, uint32_t cloneCount) :
        enemyType(type), constructor(ctor), unknown1(0), unknown2(250000.0), defaultCloneCount(cloneCount) {}

    TaggedEnemyConstructor::TaggedEnemyConstructor() :
        enemyType((uint16_t) NpcType::Invalid), constructor(nullptr), unknown1(0), unknown2(250000.0), defaultCloneCount(0) {}

    const TaggedEnemyConstructor enemyListTerminator = TaggedEnemyConstructor((uint16_t) NpcType::ListTerminator, nullptr);

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

        while ((NpcType) entries->enemyType != NpcType::ListTerminator)
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

            while ((NpcType) currentEntry->enemyType != NpcType::ListTerminator)
            {
                if ((NpcType) currentEntry->enemyType == needle)
                {
                    return currentEntry;
                }

                currentEntry++;
            }

            currentList++;
        }

        return nullptr;
    }

    void PatchEnemyConstructorLists()
    {
        // Write used lists
        for (auto& [map, enemyList] : enemyConstructorListCache)
        {
            // Add terminator
            if (enemyList.empty() || enemyList[enemyList.size() - 1].enemyType != enemyListTerminator.enemyType)
                enemyList.push_back(enemyListTerminator);

            // Write new list to table
            mapEnemyTable[(size_t) map] = enemyList.data();
        }
    }

    BmlData::LoadBmlFunction LoadBml = reinterpret_cast<BmlData::LoadBmlFunction>(0x0051f6b0);
    BmlData::FreeBmlFunction FreeBml = reinterpret_cast<BmlData::FreeBmlFunction>(0x004066a8);

    BaseObject** rootEnemyObject = reinterpret_cast<BaseObject**>(0x00aca360);

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

    decltype(InsertIntoEntityList) InsertIntoEntityList = reinterpret_cast<decltype(InsertIntoEntityList)>(0x007b4f54);
    decltype(RemoveFromEntityList) RemoveFromEntityList = reinterpret_cast<decltype(RemoveFromEntityList)>(0x007b4e84);

    decltype(InitCollisionBoxes) InitCollisionBoxes = reinterpret_cast<decltype(InitCollisionBoxes)>(0x007b9a88);

    decltype(SetStatsFromBattleParams) SetStatsFromBattleParams = reinterpret_cast<decltype(SetStatsFromBattleParams)>(0x007868f8);

    CollisionBox::CollisionBox(float x, float y, float z, float r) :
        x(x), y(y), z(z), r(r),
        unk1(0.0), unk2(0),
        x2(0.0), y2(0.0), z2(0.0),
        unknownFlags(0x8003),
        unk3(0)
    {}
};
