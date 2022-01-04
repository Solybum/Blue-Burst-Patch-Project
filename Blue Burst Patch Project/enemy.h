#pragma once

#include <vector>
#include <cstdint>
#include "math.h"
#include "object_extension.h"

// Automatically enable patching if included
#define PATCH_ENEMY_CONSTRUCTOR_LISTS

namespace Enemy
{

    enum class NpcType : uint16_t
    {
        Invalid = 0,
        Female_Base = 1,
        Female_Child = 2,
        Female_Dwarf = 3,
        Female_Fat = 4,
        Female_Macho = 5,
        Female_Old = 6,
        Female_Tall = 7,
        Male_Base = 8,
        Male_Child = 9,
        Male_Dwarf = 10,
        Male_Fat = 11,
        Male_Macho = 12,
        Male_Old = 13,
        Male_Tall = 14,
        Blue_Soldier = 25,
        Red_Soldier = 26,
        Principle = 27,
        Tekker = 28,
        Guild_Lady = 29,
        Scientist = 30,
        Nurse = 31,
        Irene = 32,
        Default_Humar = 33,
        Default_Hunewearl = 34,
        Default_Ramar = 36,
        Default_Racast = 37,
        Default_Racaseal = 38,
        Default_Fomarl = 39,
        Default_Fomewm = 40,
        Default_Fomewearl = 41,
        Unknown_42 = 42,
        Unknown_43 = 43,
        Unknown_44 = 44,
        Dacci = 45,
        Hopkins = 46,
        Unknown_47 = 47,
        HMN_FRC_W_01 = 48,
        NMN_FRC_M_01 = 49,
        NMN_FRC_W_01 = 50,
        Stage_NPCs = 51,
        Hildebear = 64,
        Rag_Rappy_Sand_Rappy = 65,
        Monest = 66,
        Savage_Wolf = 67,
        Booma = 68,
        Rappy_NPC = 69,
        Small_Hildebear_NPC = 70,
        Grass_Assassin = 96,
        Poison_Lily_Del_Lily = 97,
        Nano_Dragon = 98,
        Evil_Shark = 99,
        Pofuilly_Slime = 100,
        Pan_Arms = 101,
        Gillchic = 128,
        Garanz = 129,
        Sinow_Beat = 130,
        Canadine = 131,
        Canane = 132,
        Dubchic_Switch = 133,
        Delsaber = 160,
        Chaos_Sorcerer = 161,
        Dark_Gunner = 162,
        Death_Gunner = 163,
        Chaos_Bringer = 164,
        Darth_Belra = 165,
        Dimenian = 166,
        Bulclaw = 167,
        Claw = 168,
        NPC_Bringer = 169,
        Dragon_Gal_Gryphon = 192,
        de_ro_le = 193,
        Vol_Opt_Control = 194,
        Vol_Opt_Part_1 = 195,
        Vol_Opt_Core = 196,
        Vol_Opt_Part_2 = 197,
        Vol_Opt_Monitor = 198,
        Vol_Opt_Hiraisan = 199,
        Dark_Falz = 200,
        Olga_Flow = 201,
        Barba_Ray = 202,
        Gol_Dragon_ = 204,
        Unknown_208 = 208,
        Natasha = 209,
        Dan = 210,
        Unknown_211 = 211,
        Sinow_Berill = 212,
        Merillias = 213,
        Mericarol = 214,
        Ul_Gibbon = 215,
        Gibbles = 216,
        Gee = 217,
        Gi_Gue = 218,
        Deldepth = 219,
        Delbiter = 220,
        Dolmdarl = 221,
        Morfos = 222,
        Recobox = 223,
        Epsilon_Sinow_Zoa = 224,
        Ill_Gill = 225,
        Armour_shop = 240,
        Item_Shop = 241,
        Default_Fomar = 242,
        Default_Ramarl = 243,
        Leo = 244,
        Paganini = 245,
        Unknown_246 = 246,
        Nol = 247,
        Elly = 248,
        Unknown_249 = 249,
        Ep_2_Item_Shop = 250,
        Ep_2_Weapon_Shop = 251,
        Security_Guard = 252,
        Ep_2_Hunters_Guild = 253,
        Ep_2_Nurse = 254,
        Unknown_255 = 255,
        Momoka = 256,
        Astark = 272,
        Satellite_Lizard = 273,
        Merissa_A = 274,
        Girt = 275,
        Zu = 276,
        Boota = 277,
        Dorphon = 278,
        Goran = 279,
        Rupika = 280,
        Saint_Million = 281,
        ListTerminator = 0xffff
    };

    typedef void* (__cdecl *EnemyConstructor)(void*);

#pragma pack(push, 1)
    struct TaggedEnemyConstructor
    {
        NpcType enemyType;
        uint16_t unknown1;
        EnemyConstructor constructor;
        float unknown2;
        uint32_t defaultCloneCount;

        TaggedEnemyConstructor(NpcType type, EnemyConstructor ctor);
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

    struct EnemyBase
    {
        struct Vtable {
            union {
                void (__thiscall *Destruct)(void* self, bool32 free_memory);
                DEFINE_FIELD(0x4, void (__thiscall *Update)(void* self));
                DEFINE_FIELD(0x8, void (__thiscall *Render)(void* self));
                DEFINE_FIELD(0xc, void (__thiscall *RenderShadow)(void* self));
                DEFINE_FIELD(0x14c, void (__thiscall *ApplyInitData)(void* self, void* initData));
            };
        };

        typedef void* (__thiscall *ConstructorFunction)(void* self, void* parentObject);
        static const ConstructorFunction Constructor;
        static const Vtable* origVtable;

        union
        {
            Vtable* vtable;

            union {
                DEFINE_FIELD(0x24, void* njtl);
                DEFINE_FIELD(0x28, uint16_t originalMapSection);
                DEFINE_FIELD(0x30, uint32_t entityFlags);
                DEFINE_FIELD(0x34, void* njcm);
                DEFINE_FIELD(0x38, Vec3<float> xyz2);
                DEFINE_FIELD(0x44, Vec3<float> xyz5);
                DEFINE_FIELD(0x50, Vec3<float> xyz1);
                DEFINE_FIELD(0x5c, Vec3<uint32_t> rotation);
                DEFINE_FIELD(0xb8, uint16_t animationId);
                DEFINE_FIELD(0xc0, float currentAnimationCounter);
                DEFINE_FIELD(0xc4, float currentAnimationLength);
                DEFINE_FIELD(0xc8, float currentAnimationSpeed);
                DEFINE_FIELD(0xd8, void* unknownAnimationData);
                DEFINE_FIELD(0xdc, void* njm);
                DEFINE_FIELD(0x2bc, int16_t maxHP);
                DEFINE_FIELD(0x298, Vec3<float> xyz4);
                DEFINE_FIELD(0x2a4, Vec3<float> xyz3);
                DEFINE_FIELD(0x300, Vec3<float> xyz6);
                DEFINE_FIELD(0x30c, Vec3<float> xyz7);
                DEFINE_FIELD(0x334, int16_t currentHP);
                DEFINE_FIELD(0x346, int16_t maxHP2);
                DEFINE_FIELD(0x378, uint32_t nameUnitxtIndex);
            };

            // Ensure object's size is at least the same as its superclass
            //uint8_t _padding[0x37c];
            uint8_t _padding[0x428];
        };

        EnemyBase::EnemyBase(void* parentObject);

        void Destruct(bool32 freeMemory);
    };

    typedef uint32_t (__thiscall *InsertIntoEntityListFunction)(void* entity);
    extern InsertIntoEntityListFunction InsertIntoEntityList;
#pragma pack(pop)
};
