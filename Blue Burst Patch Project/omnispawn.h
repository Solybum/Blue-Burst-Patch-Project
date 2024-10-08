#pragma once

#include <cstddef>
#include "initlist.h"
#include "enemy.h"
#include "battleparam.h"

namespace Omnispawn
{
    // Must be 1 byte because we rewrite indices in places where
    // it would be inconvenient to write larger values (instruction operands)
    enum class NewBPIndex : uint8_t
    {
        // Start from the end of vanilla indices
        Mothmant = (size_t) BattleParam::END_INDEX + 1,
        Monest,
        SavageWolf,
        BarbarousWolf,
        PoisonLily,
        NarLily,
        SinowBeat,
        Canadine,
        CanadineRing,
        Canane,
        ChaosSorcerer,
        BeeR,
        BeeL,
        ChaosBringer,
        DarkBelra,
        BelraFist,
        SinowGold,
        RagRappy,
        AlRappy,
        NanoDragon,
        // Gillchic must always come right after Dubchic
        Dubchic,
        Gillchic,
        Garanz,
        DarkGunner,
        Bulclaw,
        Claw,
        PofuillySlime,
        PanArms,
        Hidoom,
        Migium,
        PouillySlime,
        Dubwitch,
        Hildebear,
        HildebearAttack1,
        HildebearAttack2,
        Hildeblue,
        HildeblueAttack1,
        HildeblueAttack2,
        Booma,
        Gobooma,
        Gigobooma,
        GrassAssassin,
        GrassAssassinAttack1,
        GrassAssassinAttack2,
        EvilShark,
        PalShark,
        GuilShark,
        Delsaber,
        DelsaberAttack1,
        DelsaberAttack2,
        Dimenian,
        LaDimenian,
        SoDimenian,

        SinowBerill,
        Gee,
        Delbiter,
        SinowSpigell,
        GiGue,
        Epsilon,
        Epsigard,
        IllGill,
        IllGillAttack1,
        IllGillAttack2,
        IllGillAttack3,
        Deldepth,
        Mericarol,
        UlGibbon,
        ZolGibbon,
        Gibbles,
        GibblesAttack1,
        GibblesAttack2,
        Morfos,
        MorfosAttack1,
        Recobox,
        Recon,
        SinowZoa,
        SinowZele,
        Merikle,
        Mericus,
        Merillia,
        Meriltas,
        Dolmolm,
        Dolmdarl,

        Boota,
        ZeBoota,
        ZeBootaAttack1,
        BaBoota,
        BaBootaAttack1,
        ZuCrater,
        PazuzuCrater,
        ZuDesert,
        PazuzuDesert,
        Astark,
        AstarkAttack1,
        AstarkAttack2,
        SatelliteLizardCrater,
        SatelliteLizardDesert,
        YowieCrater,
        YowieDesert,
        Dorphon,
        DorphonEclair,
        Goran,
        GoranAttack1,
        PyroGoran,
        PyroGoranAttack1,
        GoranDetonator,
        GoranDetonatorAttack1,
        MerissaA,
        MerissaAA,
        Girtablulu,

        EP1_START_INDEX = Mothmant,
        EP1_END_INDEX = SoDimenian,

        EP2_START_INDEX = SinowBerill,
        EP2_END_INDEX = Dolmdarl,

        EP4_START_INDEX = Boota,
        EP4_END_INDEX = Girtablulu
    };

    void PatchOmnispawnParticles();
    void PatchOmnispawnSoundEffects();
    void ApplyOmnispawnPatch();
};
