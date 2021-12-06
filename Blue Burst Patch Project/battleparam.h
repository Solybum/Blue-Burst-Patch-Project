#pragma once

#include <cstdint>
#include "common.h"

namespace BattleParam
{
    enum class BPStatsIndex : uint8_t
    {
        ep1_Mothmant__ep2_Mothmant__ep4_Boota = 0x0,
        ep1_Monest__ep2_Monest__ep4_ZeBoota = 0x1,
        ep1_SavageWolf__ep2_SavageWolf__ep4_None = 0x2,
        ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota = 0x3,
        ep1_PoisonLily__ep2_PoisonLily__ep4_None = 0x4,
        ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater = 0x5,
        ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater = 0x6,
        ep1_Canadine__ep2_Gee__ep4_ZuCrater = 0x7,
        ep1_CanadineRing__ep2_None__ep4_PazuzuCrater = 0x8,
        ep1_Canane__ep2_None__ep4_Astark = 0x9,
        ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_None = 0xa,
        ep1_BeeR__ep2_BeeR__ep4_None = 0xb,
        ep1_BeeL__ep2_BeeL__ep4_None = 0xc,
        ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater = 0xd,
        ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater = 0xe,
        ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon = 0xf,
        ep1_DeRolLeShell__ep2_PigRay__ep4_DorphonEclair = 0x10,
        ep1_DeRolLeMine__ep2_UlRay__ep4_Goran = 0x11,
        ep1_Dragon__ep2_GolDragon__ep4_PyroGoran = 0x12,
        ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator = 0x13,
        // Empty slots
        ep1_None__ep2_None__ep4_SandRappyDesert = 0x17,
        ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert = 0x18,
        ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA = 0x19,
        ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA = 0x1a,
        ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert = 0x1b,
        ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert = 0x1c,
        ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert = 0x1d,
        ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert = 0x1e,
        ep1_Bulclaw__ep2_None__ep4_Girtablulu = 0x1f,
        ep1_Claw__ep2_None__ep4_SaintMilionPhase1 = 0x20,
        ep1_VolOptForm1__ep2_None__ep4_SpinnerSaintMilion1 = 0x21,
        ep1_VolOptPillar__ep2_None__ep4_SaintMilionPhase2 = 0x22,
        ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2 = 0x23,
        ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1 = 0x24,
        ep1_VolOptForm2__ep2_DelLily__ep4_SpinnerShambertin1 = 0x25,
        ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2 = 0x26,
        ep1_None__ep2_None__ep4_SpinnerShambertin2 = 0x27,
        ep1_None__ep2_None__ep4_KondrieuPhase1 = 0x28,
        ep1_None__ep2_None__ep4_SpinnerKondrieu1 = 0x29,
        ep1_None__ep2_None__ep4_KondrieuPhase2 = 0x2a,
        ep1_None__ep2_OlgaFlowForm1__ep4_SpinnerKondrieu2 = 0x2b,
        ep1_None__ep2_OlgaFlowForm2__ep4_None = 0x2c,
        ep1_None__ep2_Gael__ep4_None = 0x2d,
        ep1_None__ep2_Giel__ep4_None = 0x2e,
        ep1_PofuillySlime__ep2_Deldepth__ep4_None = 0x30,
        ep1_PanArms__ep2_PanArms__ep4_None = 0x31,
        ep1_Hidoom__ep2_Hidoom__ep4_None = 0x32,
        ep1_Migium__ep2_Migium__ep4_None = 0x33,
        ep1_PouillySlime__ep2_None__ep4_None = 0x34,
        ep1_Darvant__ep2_None__ep4_None = 0x35,
        ep1_DarkFalzForm1__ep2_None__ep4_None = 0x36,
        ep1_DarkFalzForm2__ep2_None__ep4_None = 0x37,
        ep1_DarkFalzForm3__ep2_None__ep4_None = 0x38,
        ep1_DarvantFalz__ep2_None__ep4_None = 0x39,
        ep1_None__ep2_Mericarol__ep4_None = 0x3a,
        ep1_None__ep2_UlGibbon__ep4_None = 0x3b,
        ep1_None__ep2_ZolGibbon__ep4_None = 0x3c,
        ep1_None__ep2_Gibbles__ep4_None = 0x3d,
        ep1_None__ep2_Morfos__ep4_None = 0x40,
        ep1_None__ep2_Recobox__ep4_None = 0x41,
        ep1_None__ep2_Recon__ep4_None = 0x42,
        ep1_None__ep2_SinowZoa__ep4_None = 0x43,
        ep1_None__ep2_SinowZele__ep4_None = 0x44,
        ep1_None__ep2_Merikle__ep4_None = 0x45,
        ep1_None__ep2_Mericus__ep4_None = 0x46,
        ep1_Dubwitch__ep2_Dubwitch__ep4_None = 0x48,
        ep1_Hildebear__ep2_Hildebear__ep4_None = 0x49,
        ep1_Hildeblue__ep2_Hildeblue__ep4_None = 0x4a,
        ep1_Booma__ep2_Merillia__ep4_None = 0x4b,
        ep1_Gobooma__ep2_Meriltas__ep4_None = 0x4c,
        ep1_Gigobooma__ep2_None__ep4_None = 0x4d,
        ep1_GrassAssassin__ep2_GrassAssassin__ep4_None = 0x4e,
        ep1_EvilShark__ep2_Dolmolm__ep4_None = 0x4f,
        ep1_PalShark__ep2_Dolmdarl__ep4_None = 0x50,
        ep1_GuilShark__ep2_None__ep4_None = 0x51,
        ep1_Delsaber__ep2_Delsaber__ep4_None = 0x52,
        ep1_Dimenian__ep2_Dimenian__ep4_None = 0x53,
        ep1_LaDimenian__ep2_LaDimenian__ep4_None = 0x54,
        ep1_SoDimenian__ep2_SoDimenian__ep4_None = 0x55,

        None = 0xff
    };

    enum class BPAttacksIndex : uint8_t
    {
        ep1_Mothmant__ep2_Mothmant__ep4_Boota = 0x0,
        ep1_Monest__ep2_Monest__ep4_ZeBoota = 0x1,
        ep1_SavageWolf__ep2_SavageWolf__ep4_ZeBootaAttack1 = 0x2,
        ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota = 0x3,
        ep1_PoisonLily__ep2_PoisonLily__ep4_BaBootaAttack1 = 0x4,
        ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater = 0x5,
        ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater = 0x6,
        ep1_Canadine__ep2_Gee__ep4_ZuCrater = 0x7,
        ep1_CanadineRing__ep2_None__ep4_PazuzuCrater = 0x8,
        ep1_Canane__ep2_None__ep4_None = 0x9,
        ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_Astark = 0xa,
        ep1_BeeR__ep2_BeeR__ep4_AstarkAttack1 = 0xb,
        ep1_BeeL__ep2_BeeL__ep4_AstarkAttack2 = 0xc,
        ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater = 0xd,
        ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater = 0xe,
        ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon = 0xf,
        ep1_DeRolLeShell__ep2_PigRay__ep4_DorphonEclair = 0x10,
        ep1_DeRolLeMine__ep2_UlRay__ep4_Goran = 0x11,
        ep1_Dragon__ep2_GolDragon__ep4_PyroGoran = 0x12,
        ep1_BelraFist__ep2_SinowSpigell__ep4_GoranDetonator = 0x13,
        ep1_None__ep2_None__ep4_GoranAttack1 = 0x14,
        ep1_None__ep2_None__ep4_PyroGoranAttack1 = 0x15,
        ep1_None__ep2_None__ep4_GoranDetonatorAttack1 = 0x16,
        ep1_None__ep2_None__ep4_SandRappyDesert = 0x17,
        ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert = 0x18,
        ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA = 0x19,
        ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA = 0x1a,
        ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert = 0x1b,
        ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert = 0x1c,
        ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert = 0x1d,
        ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert = 0x1e,
        ep1_Bulclaw__ep2_None__ep4_Girtablulu = 0x1f,
        ep1_Claw__ep2_None__ep4_SaintMilionPhase1 = 0x20,
        ep1_VolOptForm1__ep2_None__ep4_SpinnerSaintMilion1 = 0x21,
        ep1_VolOptPillar__ep2_None__ep4_SaintMilionPhase2 = 0x22,
        ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2 = 0x23,
        ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1 = 0x24,
        ep1_VolOptForm2__ep2_DelLily__ep4_SpinnerShambertin1 = 0x25,
        ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2 = 0x26,
        ep1_None__ep2_IllGillAttack1__ep4_SpinnerShambertin2 = 0x27,
        ep1_None__ep2_IllGillAttack2__ep4_KondrieuPhase1 = 0x28,
        ep1_None__ep2_IllGillAttack3__ep4_SpinnerKondrieu1 = 0x29,
        ep1_None__ep2_None__ep4_KondrieuPhase2 = 0x2a,
        ep1_None__ep2_OlgaFlowForm1__ep4_SpinnerKondrieu2 = 0x2b,
        ep1_None__ep2_OlgaFlowForm2__ep4_None = 0x2c,
        ep1_None__ep2_Gael__ep4_None = 0x2d,
        ep1_None__ep2_Giel__ep4_None = 0x2e,
        ep1_PofuillySlime__ep2_Deldepth__ep4_None = 0x30,
        ep1_PanArms__ep2_PanArms__ep4_None = 0x31,
        ep1_Hidoom__ep2_Hidoom__ep4_None = 0x32,
        ep1_Migium__ep2_Migium__ep4_None = 0x33,
        ep1_PouillySlime__ep2_None__ep4_None = 0x34,
        ep1_Darvant__ep2_None__ep4_None = 0x35,
        ep1_DarkFalzForm1__ep2_None__ep4_None = 0x36,
        ep1_DarkFalzForm2__ep2_None__ep4_None = 0x37,
        ep1_DarkFalzForm3__ep2_None__ep4_None = 0x38,
        ep1_DarvantFalz__ep2_None__ep4_None = 0x39,
        ep1_None__ep2_Mericarol__ep4_None = 0x3a,
        ep1_None__ep2_UlGibbon__ep4_None = 0x3b,
        ep1_None__ep2_ZolGibbon__ep4_None = 0x3c,
        ep1_None__ep2_Gibbles__ep4_None = 0x3d,
        ep1_None__ep2_GibblesAttack1__ep4_None = 0x3e,
        ep1_None__ep2_GibblesAttack2__ep4_None = 0x3f,
        ep1_None__ep2_Morfos__ep4_None = 0x40,
        ep1_None__ep2_Recobox__ep4_None = 0x41,
        ep1_None__ep2_Recon__ep4_None = 0x42,
        ep1_None__ep2_SinowZoa__ep4_None = 0x43,
        ep1_None__ep2_SinowZele__ep4_None = 0x44,
        ep1_None__ep2_Merikle__ep4_None = 0x45,
        ep1_None__ep2_Mericus__ep4_None = 0x46,
        ep1_SinowGold__ep2_SinowSpigell__ep4_None = 0x47,
        ep1_Hildebear__ep2_Hildebear__ep4_None = 0x48,
        ep1_HildebearAttack1__ep2_HildebearAttack1__ep4_None = 0x49,
        ep1_HildebearAttack2__ep2_HildebearAttack2__ep4_None = 0x4a,
        ep1_Hildeblue__ep2_Hildeblue__ep4_None = 0x4b,
        ep1_HildeblueAttack1__ep2_HildeblueAttack1__ep4_None = 0x4c,
        ep1_HildeblueAttack2__ep2_HildeblueAttack2__ep4_None = 0x4d,
        ep1_Booma__ep2_Merillia__ep4_None = 0x4e,
        ep1_Gobooma__ep2_Meriltas__ep4_None = 0x4f,
        ep1_Gigobooma__ep2_MorfosAttack1__ep4_None = 0x50,
        ep1_GrassAssassin__ep2_GrassAssassin__ep4_None = 0x51,
        ep1_GrassAssassinAttack1__ep2_GrassAssassinAttack1__ep4_None = 0x52,
        ep1_GrassAssassinAttack2__ep2_GrassAssassinAttack2__ep4_None = 0x53,
        ep1_EvilShark__ep2_Dolmolm__ep4_None = 0x54,
        ep1_PalShark__ep2_Dolmdarl__ep4_None = 0x55,
        ep1_GuilShark__ep2_None__ep4_None = 0x56,
        ep1_Delsaber__ep2_Delsaber__ep4_None = 0x57,
        ep1_DelsaberAttack1__ep2_DelsaberAttack1__ep4_None = 0x58,
        ep1_DelsaberAttack2__ep2_DelsaberAttack2__ep4_None = 0x59,
        ep1_Dimenian__ep2_Dimenian__ep4_None = 0x5a,
        ep1_LaDimenian__ep2_LaDimenian__ep4_None = 0x5b,
        ep1_SoDimenian__ep2_SoDimenian__ep4_None = 0x5c,

        None = 0xff
    };

    enum class BPResistsIndex : uint8_t
    {
        ep1_Mothmant__ep2_Mothmant__ep4_Boota = 0x0,
        ep1_Monest__ep2_Monest__ep4_ZeBoota = 0x1,
        ep1_SavageWolf__ep2_SavageWolf__ep4_None = 0x2,
        ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota = 0x3,
        ep1_PoisonLily__ep2_PoisonLily__ep4_None = 0x4,
        ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater = 0x5,
        ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater = 0x6,
        ep1_Canadine__ep2_Gee__ep4_ZuCrater = 0x7,
        ep1_CanadineRing__ep2_None__ep4_PazuzuCrater = 0x8,
        ep1_Canane__ep2_None__ep4_Astark = 0x9,
        ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_None = 0xa,
        ep1_BeeR__ep2_BeeR__ep4_None = 0xb,
        ep1_BeeL__ep2_BeeL__ep4_None = 0xc,
        ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater = 0xd,
        ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater = 0xe,
        ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon = 0xf,
        ep1_DeRolLeShell__ep2_PigRay__ep4_DorphonEclair = 0x10,
        ep1_DeRolLeMine__ep2_UlRay__ep4_Goran = 0x11,
        ep1_Dragon__ep2_GolDragon__ep4_PyroGoran = 0x12,
        ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator = 0x13,
        // Empty slots
        ep1_None__ep2_None__ep4_SandRappyDesert = 0x17,
        ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert = 0x18,
        ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA = 0x19,
        ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA = 0x1a,
        ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert = 0x1b,
        ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert = 0x1c,
        ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert = 0x1d,
        ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert = 0x1e,
        ep1_Bulclaw__ep2_None__ep4_Girtablulu = 0x1f,
        ep1_Claw__ep2_None__ep4_SaintMilionPhase1 = 0x20,
        ep1_VolOptForm1__ep2_None__ep4_SpinnerSaintMilion1 = 0x21,
        ep1_VolOptPillar__ep2_None__ep4_SaintMilionPhase2 = 0x22,
        ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2 = 0x23,
        ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1 = 0x24,
        ep1_VolOptForm2__ep2_DelLily__ep4_SpinnerShambertin1 = 0x25,
        ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2 = 0x26,
        ep1_None__ep2_None__ep4_SpinnerShambertin2 = 0x27,
        ep1_None__ep2_None__ep4_KondrieuPhase1 = 0x28,
        ep1_None__ep2_None__ep4_SpinnerKondrieu1 = 0x29,
        ep1_None__ep2_None__ep4_KondrieuPhase2 = 0x2a,
        ep1_None__ep2_OlgaFlowForm1__ep4_SpinnerKondrieu2 = 0x2b,
        ep1_None__ep2_OlgaFlowForm2__ep4_None = 0x2c,
        ep1_None__ep2_Gael__ep4_None = 0x2d,
        ep1_None__ep2_Giel__ep4_None = 0x2e,
        ep1_PofuillySlime__ep2_Deldepth__ep4_None = 0x30,
        ep1_PanArms__ep2_PanArms__ep4_None = 0x31,
        ep1_Hidoom__ep2_Hidoom__ep4_None = 0x32,
        ep1_Migium__ep2_Migium__ep4_None = 0x33,
        ep1_PouillySlime__ep2_None__ep4_None = 0x34,
        ep1_Darvant__ep2_None__ep4_None = 0x35,
        ep1_DarkFalzForm1__ep2_None__ep4_None = 0x36,
        ep1_DarkFalzForm2__ep2_None__ep4_None = 0x37,
        ep1_DarkFalzForm3__ep2_None__ep4_None = 0x38,
        ep1_DarvantFalz__ep2_None__ep4_None = 0x39,
        ep1_None__ep2_Mericarol__ep4_None = 0x3a,
        ep1_None__ep2_UlGibbon__ep4_None = 0x3b,
        ep1_None__ep2_ZolGibbon__ep4_None = 0x3c,
        ep1_None__ep2_Gibbles__ep4_None = 0x3d,
        ep1_None__ep2_Morfos__ep4_None = 0x40,
        ep1_None__ep2_Recobox__ep4_None = 0x41,
        ep1_None__ep2_Recon__ep4_None = 0x42,
        ep1_None__ep2_SinowZoa__ep4_None = 0x43,
        ep1_None__ep2_SinowZele__ep4_None = 0x44,
        ep1_None__ep2_Merikle__ep4_None = 0x45,
        ep1_None__ep2_Mericus__ep4_None = 0x46,
        ep1_Hildebear__ep2_Hildebear__ep4_None = 0x48,
        ep1_Hildeblue__ep2_Hildeblue__ep4_None = 0x49,
        ep1_Booma__ep2_Merillia__ep4_None = 0x4a,
        ep1_Gobooma__ep2_Meriltas__ep4_None = 0x4b,
        ep1_Gigobooma__ep2_None__ep4_None = 0x4c,
        ep1_GrassAssassin__ep2_GrassAssassin__ep4_None = 0x4d,
        ep1_EvilShark__ep2_Dolmolm__ep4_None = 0x4e,
        ep1_PalShark__ep2_Dolmdarl__ep4_None = 0x4f,
        ep1_GuilShark__ep2_None__ep4_None = 0x50,
        ep1_Delsaber__ep2_Delsaber__ep4_None = 0x51,
        ep1_Dimenian__ep2_Dimenian__ep4_None = 0x52,
        ep1_LaDimenian__ep2_LaDimenian__ep4_None = 0x53,
        ep1_SoDimenian__ep2_SoDimenian__ep4_None = 0x54,

        None = 0xff
    };

    enum class BPAnimationsIndex : uint8_t
    {
        ep1_Mothmant__ep2_Mothmant__ep4_Boota = 0x0,
        ep1_Monest__ep2_Monest__ep4_ZeBoota = 0x1,
        ep1_SavageWolf__ep2_SavageWolf__ep4_None = 0x2,
        ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota = 0x3,
        ep1_PoisonLily__ep2_PoisonLily__ep4_None = 0x4,
        ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater = 0x5,
        ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater = 0x6,
        ep1_Canadine__ep2_Gee__ep4_ZuCrater = 0x7,
        ep1_CanadineRing__ep2_None__ep4_PazuzuCrater = 0x8,
        ep1_Canane__ep2_None__ep4_Astark = 0x9,
        ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_None = 0xa,
        ep1_BeeR__ep2_BeeR__ep4_None = 0xb,
        ep1_BeeL__ep2_BeeL__ep4_None = 0xc,
        ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater = 0xd,
        ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater = 0xe,
        ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon = 0xf,
        ep1_SinowGold__ep2_SinowSpigell__ep4_DorphonEclair = 0x10,
        ep1_DeRolLeMine__ep2_UlRay__ep4_Goran = 0x11,
        ep1_Dragon__ep2_GolDragon__ep4_PyroGoran = 0x12,
        ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator = 0x13,
        // Empty slots
        ep1_None__ep2_None__ep4_SandRappyDesert = 0x17,
        ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert = 0x18,
        ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA = 0x19,
        ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA = 0x1a,
        ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert = 0x1b,
        ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert = 0x1c,
        ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert = 0x1d,
        ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert = 0x1e,
        ep1_Bulclaw__ep2_None__ep4_Girtablulu = 0x1f,
        ep1_Claw__ep2_None__ep4_SaintMilionPhase1 = 0x20,
        ep1_VolOptForm1__ep2_None__ep4_SpinnerSaintMilion1 = 0x21,
        ep1_VolOptPillar__ep2_None__ep4_SaintMilionPhase2 = 0x22,
        ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2 = 0x23,
        ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1 = 0x24,
        ep1_VolOptForm2__ep2_DelLily__ep4_SpinnerShambertin1 = 0x25,
        ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2 = 0x26,
        ep1_None__ep2_None__ep4_SpinnerShambertin2 = 0x27,
        ep1_None__ep2_None__ep4_KondrieuPhase1 = 0x28,
        ep1_None__ep2_None__ep4_SpinnerKondrieu1 = 0x29,
        ep1_VolOptForm2__ep2_None__ep4_KondrieuPhase2 = 0x2a,
        ep1_None__ep2_OlgaFlowForm1__ep4_SpinnerKondrieu2 = 0x2b,
        ep1_None__ep2_OlgaFlowForm2__ep4_None = 0x2c,
        ep1_None__ep2_Gael__ep4_None = 0x2d,
        ep1_None__ep2_Giel__ep4_None = 0x2e,
        ep1_PofuillySlime__ep2_Deldepth__ep4_None = 0x30,
        ep1_PanArms__ep2_PanArms__ep4_None = 0x31,
        ep1_Hidoom__ep2_Hidoom__ep4_None = 0x32,
        ep1_Migium__ep2_Migium__ep4_None = 0x33,
        ep1_PouillySlime__ep2_None__ep4_None = 0x34,
        ep1_Darvant__ep2_None__ep4_None = 0x35,
        ep1_DarkFalzForm1__ep2_None__ep4_None = 0x36,
        ep1_DarkFalzForm2__ep2_None__ep4_None = 0x37,
        ep1_DarkFalzForm3__ep2_None__ep4_None = 0x38,
        ep1_DarvantFalz__ep2_None__ep4_None = 0x39,
        ep1_None__ep2_Mericarol__ep4_None = 0x3a,
        ep1_None__ep2_UlGibbon__ep4_None = 0x3b,
        ep1_None__ep2_ZolGibbon__ep4_None = 0x3c,
        ep1_None__ep2_Gibbles__ep4_None = 0x3d,
        ep1_None__ep2_Morfos__ep4_None = 0x40,
        ep1_None__ep2_Recobox__ep4_None = 0x41,
        ep1_None__ep2_Recon__ep4_None = 0x42,
        ep1_None__ep2_SinowZoa__ep4_None = 0x43,
        ep1_None__ep2_SinowZele__ep4_None = 0x44,
        ep1_None__ep2_Merikle__ep4_None = 0x45,
        ep1_None__ep2_Mericus__ep4_None = 0x46,
        ep1_Hildebear__ep2_Hildebear__ep4_None = 0x48,
        ep1_Hildeblue__ep2_Hildeblue__ep4_None = 0x49,
        ep1_Booma__ep2_Merillia__ep4_None = 0x4a,
        ep1_Gobooma__ep2_Meriltas__ep4_None = 0x4b,
        ep1_Gigobooma__ep2_None__ep4_None = 0x4c,
        ep1_GrassAssassin__ep2_GrassAssassin__ep4_None = 0x4d,
        ep1_EvilShark__ep2_Dolmolm__ep4_None = 0x4e,
        ep1_PalShark__ep2_Dolmdarl__ep4_None = 0x4f,
        ep1_GuilShark__ep2_None__ep4_None = 0x50,
        ep1_Delsaber__ep2_Delsaber__ep4_None = 0x51,
        ep1_Dimenian__ep2_Dimenian__ep4_None = 0x52,
        ep1_LaDimenian__ep2_LaDimenian__ep4_None = 0x53,
        ep1_SoDimenian__ep2_SoDimenian__ep4_None = 0x54,

        None = 0xff
    };

    /// Highest used index
    const size_t END_INDEX = 0x5c;

    typedef union {
        BPStatsIndex stats;
        BPAttacksIndex attacks;
        BPResistsIndex resists;
        BPAnimationsIndex animations;
    } BPIndex;

#pragma pack(push, 1)
    typedef struct {
        int16_t atp;
        int16_t mst;
        int16_t evp;
        int16_t hp;
        int16_t dfp;
        int16_t ata;
        int16_t lck;
        int16_t esp;
        float field_0x10;
        float field_0x14;
        int unknown_hp_mst_modifier;
        int16_t xp;
        int16_t field_0x1e;
        int16_t field_0x20;
        int16_t field_0x22;
    } BPStatsEntry;

    typedef struct {
        short unknown_stat1_part1;
        short unknown_stat1_part2;
        short unknown_ata_modifier;
        short foo;
        float max_distance;
        int max_y_angle; /* angle encoded as int */
        float field_0x10;
        uint8_t field_0x14;
        uint8_t field_0x15;
        uint8_t field_0x16;
        uint8_t field_0x17;
        uint8_t field_0x18;
        uint8_t field_0x19;
        uint8_t field_0x1a;
        uint8_t field_0x1b;
        uint8_t field_0x1c;
        uint8_t field_0x1d;
        uint8_t field_0x1e;
        uint8_t field_0x1f;
        uint8_t field_0x20;
        uint8_t field_0x21;
        uint8_t field_0x22;
        uint8_t field_0x23;
        uint8_t field_0x24;
        uint8_t field_0x25;
        uint8_t field_0x26;
        uint8_t field_0x27;
        uint8_t field_0x28;
        uint8_t field_0x29;
        uint8_t field_0x2a;
        uint8_t field_0x2b;
        uint8_t field_0x2c;
        uint8_t field_0x2d;
        uint8_t field_0x2e;
        uint8_t field_0x2f;
    } BPAttacksEntry;

    typedef struct {
        float field_0x0;
        float field_0x4;
        float field_0x8;
        float field_0xc;
        float field_0x10;
        float field_0x14;
        float field_0x18;
        float field_0x1c;
        float field_0x20;
        float field_0x24;
        float field_0x28;
        float field_0x2c;
    } BPAnimationsEntry;

    typedef struct {
        int16_t unknown_evp_modifier;
        int16_t efr;
        int16_t eic;
        int16_t eth;
        int16_t elt;
        int16_t edk;
        uint8_t field_0xc;
        uint8_t field_0xd;
        uint8_t field_0xe;
        uint8_t field_0xf;
        uint8_t field_0x10;
        uint8_t field_0x11;
        uint8_t field_0x12;
        uint8_t field_0x13;
        uint8_t field_0x14;
        uint8_t field_0x15;
        uint8_t field_0x16;
        uint8_t field_0x17;
        uint8_t field_0x18;
        uint8_t field_0x19;
        uint8_t field_0x1a;
        uint8_t field_0x1b;
        int unknown_dfp_modifier;
    } BPResistsEntry;

    /// All difficulties in one, this is the full file
    typedef struct {
        BPStatsEntry stats[4][96];
        BPAttacksEntry attacks[4][96];
        BPResistsEntry resists[4][96];
        BPAnimationsEntry animations[4][96];
    } BPFile;

    /// Only one difficulty, this is what's loaded by the game when joining a room
    typedef struct {
        BPStatsEntry stats[96];
        BPAttacksEntry attacks[96];
        BPResistsEntry resists[96];
        BPAnimationsEntry animations[96];
    } BPDifficultyFile;
#pragma pack(pop)

    /// Custom type
    enum class BPEntryType
    {
        Stats = 0,
        Attacks = 1,
        Resists = 2,
        Animations = 3
    };

    void* GetBPEntry(Episode ep, bool solo_mode, uint8_t i, BPEntryType entryType);
};
