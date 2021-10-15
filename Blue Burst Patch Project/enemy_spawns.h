#pragma once

#include <cstdint>

namespace EnemySpawns
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
    typedef struct
    {
        NpcType enemyType;
        uint16_t unknown1;
        EnemyConstructor constructor;
        float unknown2;
        uint32_t unknown3;
    } TaggedEnemyConstructor;
#pragma pack(pop)

    const TaggedEnemyConstructor enemyListTerminator = []()
    {
        TaggedEnemyConstructor terminator;
        terminator.enemyType = NpcType::ListTerminator;
        return terminator;
    }();

    void ApplyEnemySpawnPatch();
};
