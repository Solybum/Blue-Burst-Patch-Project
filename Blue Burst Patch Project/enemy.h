#pragma once

#include <unordered_map>
#include <vector>
#include <cstdint>
#include "mathutil.h"
#include "object_extension.h"
#include "battleparam.h"
#include "object.h"
#include "entity.h"

namespace Enemy
{

    enum class NpcType : uint16_t
    {
        Invalid = 0,
        FemaleBase = 1,
        FemaleChild = 2,
        FemaleDwarf = 3,
        FemaleFat = 4,
        FemaleMacho = 5,
        FemaleOld = 6,
        FemaleTall = 7,
        MaleBase = 8,
        MaleChild = 9,
        MaleDwarf = 10,
        MaleFat = 11,
        MaleMacho = 12,
        MaleOld = 13,
        MaleTall = 14,
        BlueSoldier = 25,
        RedSoldier = 26,
        Principle = 27,
        Tekker = 28,
        GuildLady = 29,
        Scientist = 30,
        Nurse = 31,
        Irene = 32,
        DefaultHumar = 33,
        DefaultHunewearl = 34,
        DefaultRamar = 36,
        DefaultRacast = 37,
        DefaultRacaseal = 38,
        DefaultFomarl = 39,
        DefaultFomewm = 40,
        DefaultFomewearl = 41,
        Unknown42 = 42,
        Unknown43 = 43,
        Unknown44 = 44,
        Dacci = 45,
        Hopkins = 46,
        Unknown47 = 47,
        HMNFRCW01 = 48,
        NMNFRCM01 = 49,
        NMNFRCW01 = 50,
        StageNPCs = 51,
        Hildebear = 64,
        RagRappy_SandRappy = 65,
        Monest = 66,
        SavageWolf = 67,
        Booma = 68,
        RappyNPC = 69,
        SmallHildebearNPC = 70,
        GrassAssassin = 96,
        PoisonLily_DelLily = 97,
        NanoDragon = 98,
        EvilShark = 99,
        PofuillySlime = 100,
        PanArms = 101,
        Gillchic_Dubchic = 128,
        Garanz = 129,
        SinowBeat = 130,
        Canadine = 131,
        Canane = 132,
        Dubwitch = 133,
        Delsaber = 160,
        ChaosSorcerer = 161,
        DarkGunner = 162,
        DeathGunner = 163,
        ChaosBringer = 164,
        DarkBelra = 165,
        Dimenian = 166,
        Bulclaw = 167,
        Claw = 168,
        NPCBringer = 169,
        Dragon_GalGryphon = 192,
        DeRolLe = 193,
        VolOptControl = 194,
        VolOptPart1 = 195,
        VolOptCore = 196,
        VolOptPart2 = 197,
        VolOptMonitor = 198,
        VolOptHiraisan = 199,
        DarkFalz = 200,
        OlgaFlow = 201,
        BarbaRay = 202,
        GolDragon = 204,
        Unknown208 = 208,
        Natasha = 209,
        Dan = 210,
        Unknown211 = 211,
        SinowBerill = 212,
        Merillia = 213,
        Mericarol = 214,
        UlGibbon = 215,
        Gibbles = 216,
        Gee = 217,
        GiGue = 218,
        Deldepth = 219,
        Delbiter = 220,
        Dolmolm = 221,
        Morfos = 222,
        Recobox = 223,
        Epsilon_SinowZoa = 224,
        IllGill = 225,
        ArmourShop = 240,
        ItemShop = 241,
        DefaultFomar = 242,
        DefaultRamarl = 243,
        Leo = 244,
        Paganini = 245,
        Unknown246 = 246,
        Nol = 247,
        Elly = 248,
        Unknown249 = 249,
        Ep2ItemShop = 250,
        Ep2WeaponShop = 251,
        SecurityGuard = 252,
        Ep2HuntersGuild = 253,
        Ep2Nurse = 254,
        Unknown255 = 255,
        Momoka = 256,
        Astark = 272,
        SatelliteLizard = 273,
        MerissaA = 274,
        Girtablulu = 275,
        Zu = 276,
        Boota = 277,
        Dorphon = 278,
        Goran = 279,
        Rupika = 280,
        SaintMillion = 281,
        ListTerminator = 0xffff,
        
        EpsilonDisambiguator = 10000,
        SinowZoaDisambiguator = 10001
    };

    struct EnemyBase;

    struct InitData
    {
        int16_t skin;
        uint16_t flags;
        struct InnerData
        {
            int16_t indexSmall;
            uint16_t cloneCount;
            int16_t floorNumber;
            int16_t entityLarge;
            int16_t mapSection;
            int16_t waveNumber;
            int16_t waveNumber2;
            int16_t unknown;
            Vec3f position;
            Vec3<int32_t> rotation;
            float param1;
            float param2;
            float param3;
            float param4;
            float param5;
            int16_t param6;
            int16_t param7;
            EnemyBase* spawnedInstance;
        } data;
    };

    typedef void* (__cdecl *EnemyConstructor)(InitData::InnerData*);
    
    struct SpawnableDefinition
    {
        uint16_t id = 0;
        void (__cdecl *LoadAssets)(void) = nullptr;
        void (__cdecl *UnloadAssets)(void) = nullptr;
        void* (__cdecl *Create)(InitData::InnerData* initData) = nullptr;
        
        constexpr SpawnableDefinition() = default;
        SpawnableDefinition(NpcType id, size_t loadAssetsAddr, size_t unloadAssetsAddr, size_t createAddr);
        SpawnableDefinition(uint16_t id,
            decltype(LoadAssets) LoadAssets,
            decltype(UnloadAssets) UnloadAssets,
            decltype(Create) Create);
    };
    
    const std::unordered_map<NpcType, SpawnableDefinition>& GetEnemyDefinitions();
    const SpawnableDefinition& GetEnemyDefinition(NpcType);

#pragma pack(push, 1)
    struct TaggedEnemyConstructor
    {
        uint16_t enemyType;
        uint16_t unknown1;
        EnemyConstructor constructor;
        float unknown2;
        uint32_t defaultCloneCount;

        TaggedEnemyConstructor(uint16_t type, EnemyConstructor ctor);
        TaggedEnemyConstructor();
    };

    std::vector<TaggedEnemyConstructor>& GetEnemyConstructorList(Map::MapType);
    TaggedEnemyConstructor* FindEnemyConstructor(NpcType);
    void PatchEnemyConstructorLists();

    struct BmlContentsInfo
    {
        const char* bmlName;
        const char** njNames;
        const char** njmNames;
        const uint16_t njCount;
        const uint16_t njmCount;
    };

    struct NjChunk
    {
        uint32_t flags;
        uint32_t unknown;
        void* njcm;
        void* njtl;
    };

    struct NjmData
    {
        uint32_t unknown;
        void* data;
    };

    struct BmlData
    {
        union
        {
            uint8_t _padding[0x440];

            DEFINE_FIELD(0x42c, NjChunk* nj);
            DEFINE_FIELD(0x430, NjmData* njm);
            DEFINE_FIELD(0x434, size_t njCount);
            DEFINE_FIELD(0x438, size_t njmCount);
        };

        typedef BmlData* (__cdecl *LoadBmlFunction)(BmlContentsInfo* toc);
        typedef BmlData* (__thiscall *FreeBmlFunction)(BmlData* self, bool32 free_memory);
    };

    extern BmlData::LoadBmlFunction LoadBml;
    extern BmlData::FreeBmlFunction FreeBml;

    extern void** rootEnemyObject;

    enum Attribute : uint32_t
    {
        Native = 1,
        ABeast = 2,
        Machine = 4,
        Dark = 8,
        Special = 0x40
    };

    struct EnemyBase
    {
        struct Vtable {
            union {
                void (__thiscall *Destruct)(void* self, bool32 free_memory);
                DEFINE_FIELD(0x4, void (__thiscall *Update)(void* self));
                DEFINE_FIELD(0x8, void (__thiscall *Render)(void* self));
                DEFINE_FIELD(0xc, void (__thiscall *RenderShadow)(void* self));
                DEFINE_FIELD(0x14c, void (__thiscall *ApplyInitData)(void* self, InitData::InnerData* initData));
            };
        };

        typedef void* (__thiscall *ConstructorFunction)(void* self, void* parentObject);
        static const ConstructorFunction Constructor;
        static const Vtable* origVtable;

        union
        {
            Vtable* vtable;

            union {
                DEFINE_FIELD(0x8, ObjectFlag objectFlags);
                DEFINE_FIELD(0x1c, Entity::EntityIndex entityIndex);
                DEFINE_FIELD(0x24, void* njtl);
                DEFINE_FIELD(0x28, uint16_t originalMapSection);
                DEFINE_FIELD(0x30, Entity::EntityFlag entityFlags);
                DEFINE_FIELD(0x34, void* njcm);
                DEFINE_FIELD(0x38, Vec3<float> xyz2);
                DEFINE_FIELD(0x44, Vec3<float> xyz5);
                DEFINE_FIELD(0x50, Vec3<float> xyz1);
                DEFINE_FIELD(0x5c, Vec3<uint32_t> rotation);
                DEFINE_FIELD(0x80, InitData* initData);
                DEFINE_FIELD(0xb8, uint16_t animationId);
                DEFINE_FIELD(0xc0, float currentAnimationCounter);
                DEFINE_FIELD(0xc4, float currentAnimationLength);
                DEFINE_FIELD(0xc8, float currentAnimationSpeed);
                DEFINE_FIELD(0xd8, void* unknownAnimationData);
                DEFINE_FIELD(0xdc, void* njm);
                DEFINE_FIELD(0x2b4, BattleParam::BPStatsEntry* bpStats);
                DEFINE_FIELD(0x2bc, int16_t maxHP);
                DEFINE_FIELD(0x298, Vec3<float> xyz4);
                DEFINE_FIELD(0x2a4, Vec3<float> xyz3);
                DEFINE_FIELD(0x2e8, Attribute attribute);
                DEFINE_FIELD(0x300, Vec3<float> xyz6);
                DEFINE_FIELD(0x30c, Vec3<float> xyz7);
                DEFINE_FIELD(0x31c, BattleParam::BPAttacksEntry* bpAttacks);
                DEFINE_FIELD(0x320, BattleParam::BPResistsEntry* bpResists);
                DEFINE_FIELD(0x334, int16_t currentHP);
                DEFINE_FIELD(0x346, int16_t maxHP2);
                DEFINE_FIELD(0x378, uint32_t nameUnitxtIndex);
            };

            // Ensure object's size is at least the same as its superclass
            uint8_t _padding[0x37c];
        };

        EnemyBase(void* parentObject);

        void Destruct(bool32 freeMemory);
    };

    struct CollisionBox
    {
        union
        {
            float x;
            DEFINE_FIELD(0x4, float y);
            DEFINE_FIELD(0x8, float z);
            DEFINE_FIELD(0xc, float r);
            DEFINE_FIELD(0x18, uint32_t unknownFlags);
            uint8_t _padding[0x2c];
        };

        CollisionBox(float x, float y, float z, float r);
    };

#pragma pack(pop)

    typedef uint32_t (__thiscall *InsertIntoEntityListFunction)(void* entity);
    extern InsertIntoEntityListFunction InsertIntoEntityList;
    extern void (__thiscall *InitCollisionBoxes)(void* self, const CollisionBox* collisionData, uint32_t collisionBoxCount);
    extern bool32 (__thiscall *SetStatsFromBattleParams)(void* self,
        const BattleParam::BPStatsEntry* stats,
        const BattleParam::BPAttacksEntry* attacks,
        const BattleParam::BPResistsEntry* resists);
};
