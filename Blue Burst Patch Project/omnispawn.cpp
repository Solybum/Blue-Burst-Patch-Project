#include <cstddef>
#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include "omnispawn.h"
#include "common.h"
#include "map.h"
#include "enemy.h"
#include "battleparam.h"
#include "helpers.h"
#include "initlist.h"

namespace Omnispawn
{
    using Enemy::NpcType;
    using Map::MapType;
    using BattleParam::BPIndex;
    using BattleParam::BPStatsIndex;
    using BattleParam::BPAttacksIndex;
    using BattleParam::BPResistsIndex;
    using BattleParam::BPAnimationsIndex;
    using BattleParam::BPEntryType;
    using Omnispawn::NewBPIndex;

    /// Enemies that appear in both Episode 1 and 2
    std::vector<NewBPIndex> ep1AndEp2SharedEnemies = {
        NewBPIndex::Mothmant,
        NewBPIndex::Monest,
        NewBPIndex::SavageWolf,
        NewBPIndex::BarbarousWolf,
        NewBPIndex::PoisonLily,
        NewBPIndex::NarLily,
        NewBPIndex::ChaosSorcerer,
        NewBPIndex::BeeR,
        NewBPIndex::BeeL,
        NewBPIndex::DarkBelra,
        NewBPIndex::RagRappy,
        NewBPIndex::AlRappy,
        NewBPIndex::Dubchic,
        NewBPIndex::Gillchic,
        NewBPIndex::Garanz,
        NewBPIndex::PanArms,
        NewBPIndex::Hidoom,
        NewBPIndex::Migium,
        NewBPIndex::Dubwitch,
        NewBPIndex::Hildebear,
        NewBPIndex::Hildeblue,
        NewBPIndex::Delsaber,
        NewBPIndex::Dimenian,
        NewBPIndex::LaDimenian,
        NewBPIndex::SoDimenian
    };

    struct BPIndexAll {
        BPStatsIndex stats;
        BPAttacksIndex attacks;
        BPResistsIndex resists;
        BPAnimationsIndex animations;

        BPIndexAll(BPStatsIndex stats_, BPAttacksIndex attacks_, BPResistsIndex resists_, BPAnimationsIndex animations_) :
            stats(stats_), attacks(attacks_), resists(resists_), animations(animations_) {}

        BPIndexAll(BPAttacksIndex attacks_) :
            stats(BPStatsIndex::None), attacks(attacks_), resists(BPResistsIndex::None), animations(BPAnimationsIndex::None) {}
    };

    /// Map new indices to old
    std::map<NewBPIndex, BPIndexAll> newBpIndexMap = {
        {NewBPIndex::Mothmant, BPIndexAll(
            BPStatsIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota,
            BPAttacksIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota,
            BPResistsIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota,
            BPAnimationsIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota
        )},
        {NewBPIndex::Monest, BPIndexAll(
            BPStatsIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota,
            BPAttacksIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota,
            BPResistsIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota,
            BPAnimationsIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota
        )},
        {NewBPIndex::SavageWolf, BPIndexAll(
            BPStatsIndex::ep1_SavageWolf__ep2_SavageWolf__ep4_None,
            BPAttacksIndex::ep1_SavageWolf__ep2_SavageWolf__ep4_ZeBootaAttack1,
            BPResistsIndex::ep1_SavageWolf__ep2_SavageWolf__ep4_None,
            BPAnimationsIndex::ep1_SavageWolf__ep2_SavageWolf__ep4_None
        )},
        {NewBPIndex::BarbarousWolf, BPIndexAll(
            BPStatsIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota,
            BPAttacksIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota,
            BPResistsIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota,
            BPAnimationsIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota
        )},
        {NewBPIndex::PoisonLily, BPIndexAll(
            BPStatsIndex::ep1_PoisonLily__ep2_PoisonLily__ep4_None,
            BPAttacksIndex::ep1_PoisonLily__ep2_PoisonLily__ep4_BaBootaAttack1,
            BPResistsIndex::ep1_PoisonLily__ep2_PoisonLily__ep4_None,
            BPAnimationsIndex::ep1_PoisonLily__ep2_PoisonLily__ep4_None
        )},
        {NewBPIndex::NarLily, BPIndexAll(
            BPStatsIndex::ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater,
            BPAttacksIndex::ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater,
            BPResistsIndex::ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater,
            BPAnimationsIndex::ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater
        )},
        {NewBPIndex::SinowBeat, BPIndexAll(
            BPStatsIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater,
            BPAttacksIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater,
            BPResistsIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater,
            BPAnimationsIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater
        )},
        {NewBPIndex::Canadine, BPIndexAll(
            BPStatsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPAttacksIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPResistsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPAnimationsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater
        )},
        {NewBPIndex::CanadineRing, BPIndexAll(
            BPStatsIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater,
            BPAttacksIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater,
            BPResistsIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater,
            BPAnimationsIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater
        )},
        {NewBPIndex::Canane, BPIndexAll(
            BPStatsIndex::ep1_Canane__ep2_UlRay__ep4_Astark,
            BPAttacksIndex::ep1_Canane__ep2_UlRay__ep4_None,
            BPResistsIndex::ep1_Canane__ep2_UlRay__ep4_Astark,
            BPAnimationsIndex::ep1_Canane__ep2_UlRay__ep4_Astark
        )},
        {NewBPIndex::ChaosSorcerer, BPIndexAll(
            BPStatsIndex::ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_None,
            BPAttacksIndex::ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_Astark,
            BPResistsIndex::ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_None,
            BPAnimationsIndex::ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_None
        )},
        {NewBPIndex::BeeR, BPIndexAll(
            BPStatsIndex::ep1_BeeR__ep2_BeeR__ep4_None,
            BPAttacksIndex::ep1_BeeR__ep2_BeeR__ep4_AstarkAttack1,
            BPResistsIndex::ep1_BeeR__ep2_BeeR__ep4_None,
            BPAnimationsIndex::ep1_BeeR__ep2_BeeR__ep4_None
        )},
        {NewBPIndex::BeeL, BPIndexAll(
            BPStatsIndex::ep1_BeeL__ep2_BeeL__ep4_None,
            BPAttacksIndex::ep1_BeeL__ep2_BeeL__ep4_AstarkAttack2,
            BPResistsIndex::ep1_BeeL__ep2_BeeL__ep4_None,
            BPAnimationsIndex::ep1_BeeL__ep2_BeeL__ep4_None
        )},
        {NewBPIndex::ChaosBringer, BPIndexAll(
            BPStatsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPAttacksIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPResistsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPAnimationsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater
        )},
        {NewBPIndex::DarkBelra, BPIndexAll(
            BPStatsIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater,
            BPAttacksIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater,
            BPResistsIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater,
            BPAnimationsIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater
        )},
        {NewBPIndex::BelraFist, BPIndexAll(
            BPAttacksIndex::ep1_BelraFist__ep2_SinowSpigell__ep4_GoranDetonator
        )},
        {NewBPIndex::SinowGold, BPIndexAll(
            BPStatsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator,
            BPAttacksIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_None,
            BPResistsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator,
            BPAnimationsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_DorphonEclair
        )},
        {NewBPIndex::RagRappy, BPIndexAll(
            BPStatsIndex::ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert,
            BPAttacksIndex::ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert,
            BPResistsIndex::ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert,
            BPAnimationsIndex::ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert
        )},
        {NewBPIndex::AlRappy, BPIndexAll(
            BPStatsIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA,
            BPAttacksIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA,
            BPResistsIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA,
            BPAnimationsIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA
        )},
        {NewBPIndex::NanoDragon, BPIndexAll(
            BPStatsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPAttacksIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPResistsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPAnimationsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA
        )},
        {NewBPIndex::Dubchic, BPIndexAll(
            BPStatsIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert,
            BPAttacksIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert,
            BPResistsIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert,
            BPAnimationsIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert
        )},
        {NewBPIndex::Gillchic, BPIndexAll(
            BPStatsIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert,
            BPAttacksIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert,
            BPResistsIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert,
            BPAnimationsIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert
        )},
        {NewBPIndex::Garanz, BPIndexAll(
            BPStatsIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert,
            BPAttacksIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert,
            BPResistsIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert,
            BPAnimationsIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert
        )},
        {NewBPIndex::DarkGunner, BPIndexAll(
            BPStatsIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert,
            BPAttacksIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert,
            BPResistsIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert,
            BPAnimationsIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert
        )},
        {NewBPIndex::Bulclaw, BPIndexAll(
            BPStatsIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu,
            BPAttacksIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu,
            BPResistsIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu,
            BPAnimationsIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu
        )},
        {NewBPIndex::Claw, BPIndexAll(
            BPStatsIndex::ep1_Claw__ep2_None__ep4_SaintMilionPhase1,
            BPAttacksIndex::ep1_Claw__ep2_None__ep4_SaintMilionPhase1,
            BPResistsIndex::ep1_Claw__ep2_None__ep4_SaintMilionPhase1,
            BPAnimationsIndex::ep1_Claw__ep2_None__ep4_SaintMilionPhase1
        )},
        {NewBPIndex::PofuillySlime, BPIndexAll(
            BPStatsIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None,
            BPAttacksIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None,
            BPResistsIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None,
            BPAnimationsIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None
        )},
        {NewBPIndex::PanArms, BPIndexAll(
            BPStatsIndex::ep1_PanArms__ep2_PanArms__ep4_None,
            BPAttacksIndex::ep1_PanArms__ep2_PanArms__ep4_None,
            BPResistsIndex::ep1_PanArms__ep2_PanArms__ep4_None,
            BPAnimationsIndex::ep1_PanArms__ep2_PanArms__ep4_None
        )},
        {NewBPIndex::Hidoom, BPIndexAll(
            BPStatsIndex::ep1_Hidoom__ep2_Hidoom__ep4_None,
            BPAttacksIndex::ep1_Hidoom__ep2_Hidoom__ep4_None,
            BPResistsIndex::ep1_Hidoom__ep2_Hidoom__ep4_None,
            BPAnimationsIndex::ep1_Hidoom__ep2_Hidoom__ep4_None
        )},
        {NewBPIndex::Migium, BPIndexAll(
            BPStatsIndex::ep1_Migium__ep2_Migium__ep4_None,
            BPAttacksIndex::ep1_Migium__ep2_Migium__ep4_None,
            BPResistsIndex::ep1_Migium__ep2_Migium__ep4_None,
            BPAnimationsIndex::ep1_Migium__ep2_Migium__ep4_None
        )},
        {NewBPIndex::PouillySlime, BPIndexAll(
            BPStatsIndex::ep1_PouillySlime__ep2_None__ep4_None,
            BPAttacksIndex::ep1_PouillySlime__ep2_None__ep4_None,
            BPResistsIndex::ep1_PouillySlime__ep2_None__ep4_None,
            BPAnimationsIndex::ep1_PouillySlime__ep2_None__ep4_None
        )},
        // Dubwitch can be skipped because it doesn't have BP
        {NewBPIndex::Hildebear, BPIndexAll(
            BPStatsIndex::ep1_Hildebear__ep2_Hildebear__ep4_None,
            BPAttacksIndex::ep1_Hildebear__ep2_Hildebear__ep4_None,
            BPResistsIndex::ep1_Hildebear__ep2_Hildebear__ep4_None,
            BPAnimationsIndex::ep1_Hildebear__ep2_Hildebear__ep4_None
        )},
        {NewBPIndex::HildebearAttack1, BPIndexAll(
            BPAttacksIndex::ep1_HildebearAttack1__ep2_HildebearAttack1__ep4_None
        )},
        {NewBPIndex::HildebearAttack2, BPIndexAll(
            BPAttacksIndex::ep1_HildebearAttack2__ep2_HildebearAttack2__ep4_None
        )},
        {NewBPIndex::HildeblueAttack1, BPIndexAll(
            BPAttacksIndex::ep1_HildeblueAttack1__ep2_HildeblueAttack1__ep4_None
        )},
        {NewBPIndex::HildeblueAttack2, BPIndexAll(
            BPAttacksIndex::ep1_HildeblueAttack2__ep2_HildeblueAttack2__ep4_None
        )},
        {NewBPIndex::Hildeblue, BPIndexAll(
            BPStatsIndex::ep1_Hildeblue__ep2_Hildeblue__ep4_None,
            BPAttacksIndex::ep1_Hildeblue__ep2_Hildeblue__ep4_None,
            BPResistsIndex::ep1_Hildeblue__ep2_Hildeblue__ep4_None,
            BPAnimationsIndex::ep1_Hildeblue__ep2_Hildeblue__ep4_None
        )},
        {NewBPIndex::Booma, BPIndexAll(
            BPStatsIndex::ep1_Booma__ep2_Merillia__ep4_None,
            BPAttacksIndex::ep1_Booma__ep2_Merillia__ep4_None,
            BPResistsIndex::ep1_Booma__ep2_Merillia__ep4_None,
            BPAnimationsIndex::ep1_Booma__ep2_Merillia__ep4_None
        )},
        {NewBPIndex::Gobooma, BPIndexAll(
            BPStatsIndex::ep1_Gobooma__ep2_Meriltas__ep4_None,
            BPAttacksIndex::ep1_Gobooma__ep2_Meriltas__ep4_None,
            BPResistsIndex::ep1_Gobooma__ep2_Meriltas__ep4_None,
            BPAnimationsIndex::ep1_Gobooma__ep2_Meriltas__ep4_None
        )},
        {NewBPIndex::Gigobooma, BPIndexAll(
            BPStatsIndex::ep1_Gigobooma__ep2_None__ep4_None,
            BPAttacksIndex::ep1_Gigobooma__ep2_MorfosAttack1__ep4_None,
            BPResistsIndex::ep1_Gigobooma__ep2_None__ep4_None,
            BPAnimationsIndex::ep1_Gigobooma__ep2_None__ep4_None
        )},
        {NewBPIndex::GrassAssassin, BPIndexAll(
            BPStatsIndex::ep1_GrassAssassin__ep2_GrassAssassin__ep4_None,
            BPAttacksIndex::ep1_GrassAssassin__ep2_GrassAssassin__ep4_None,
            BPResistsIndex::ep1_GrassAssassin__ep2_GrassAssassin__ep4_None,
            BPAnimationsIndex::ep1_GrassAssassin__ep2_GrassAssassin__ep4_None
        )},
        {NewBPIndex::GrassAssassinAttack1, BPIndexAll(
            BPAttacksIndex::ep1_GrassAssassinAttack1__ep2_GrassAssassinAttack1__ep4_None
        )},
        {NewBPIndex::GrassAssassinAttack2, BPIndexAll(
            BPAttacksIndex::ep1_GrassAssassinAttack2__ep2_GrassAssassinAttack2__ep4_None
        )},
        {NewBPIndex::EvilShark, BPIndexAll(
            BPStatsIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None,
            BPAttacksIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None,
            BPResistsIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None,
            BPAnimationsIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None
        )},
        {NewBPIndex::PalShark, BPIndexAll(
            BPStatsIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None,
            BPAttacksIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None,
            BPResistsIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None,
            BPAnimationsIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None
        )},
        {NewBPIndex::GuilShark, BPIndexAll(
            BPStatsIndex::ep1_GuilShark__ep2_None__ep4_None,
            BPAttacksIndex::ep1_GuilShark__ep2_None__ep4_None,
            BPResistsIndex::ep1_GuilShark__ep2_None__ep4_None,
            BPAnimationsIndex::ep1_GuilShark__ep2_None__ep4_None
        )},
        {NewBPIndex::Delsaber, BPIndexAll(
            BPStatsIndex::ep1_Delsaber__ep2_Delsaber__ep4_None,
            BPAttacksIndex::ep1_Delsaber__ep2_Delsaber__ep4_None,
            BPResistsIndex::ep1_Delsaber__ep2_Delsaber__ep4_None,
            BPAnimationsIndex::ep1_Delsaber__ep2_Delsaber__ep4_None
        )},
        {NewBPIndex::DelsaberAttack1, BPIndexAll(
            BPAttacksIndex::ep1_DelsaberAttack1__ep2_DelsaberAttack1__ep4_None
        )},
        {NewBPIndex::DelsaberAttack2, BPIndexAll(
            BPAttacksIndex::ep1_DelsaberAttack2__ep2_DelsaberAttack2__ep4_None
        )},
        {NewBPIndex::Dimenian, BPIndexAll(
            BPStatsIndex::ep1_Dimenian__ep2_Dimenian__ep4_None,
            BPAttacksIndex::ep1_Dimenian__ep2_Dimenian__ep4_None,
            BPResistsIndex::ep1_Dimenian__ep2_Dimenian__ep4_None,
            BPAnimationsIndex::ep1_Dimenian__ep2_Dimenian__ep4_None
        )},
        {NewBPIndex::LaDimenian, BPIndexAll(
            BPStatsIndex::ep1_LaDimenian__ep2_LaDimenian__ep4_None,
            BPAttacksIndex::ep1_LaDimenian__ep2_LaDimenian__ep4_None,
            BPResistsIndex::ep1_LaDimenian__ep2_LaDimenian__ep4_None,
            BPAnimationsIndex::ep1_LaDimenian__ep2_LaDimenian__ep4_None
        )},
        {NewBPIndex::SoDimenian, BPIndexAll(
            BPStatsIndex::ep1_SoDimenian__ep2_SoDimenian__ep4_None,
            BPAttacksIndex::ep1_SoDimenian__ep2_SoDimenian__ep4_None,
            BPResistsIndex::ep1_SoDimenian__ep2_SoDimenian__ep4_None,
            BPAnimationsIndex::ep1_SoDimenian__ep2_SoDimenian__ep4_None
        )},
        {NewBPIndex::SinowBerill, BPIndexAll(
            BPStatsIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater,
            BPAttacksIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater,
            BPResistsIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater,
            BPAnimationsIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater
        )},
        {NewBPIndex::Gee, BPIndexAll(
            BPStatsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPAttacksIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPResistsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPAnimationsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater
        )},
        {NewBPIndex::Delbiter, BPIndexAll(
            BPStatsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPAttacksIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPResistsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPAnimationsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater
        )},
        {NewBPIndex::SinowSpigell, BPIndexAll(
            BPStatsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator,
            BPAttacksIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_None,
            BPResistsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator,
            BPAnimationsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_DorphonEclair
        )},
        {NewBPIndex::GiGue, BPIndexAll(
            BPStatsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPAttacksIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPResistsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPAnimationsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA
        )},
        {NewBPIndex::Epsilon, BPIndexAll(
            BPStatsIndex::ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2,
            BPAttacksIndex::ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2,
            BPResistsIndex::ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2,
            BPAnimationsIndex::ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2
        )},
        {NewBPIndex::Epsigard, BPIndexAll(
            BPStatsIndex::ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1,
            BPAttacksIndex::ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1,
            BPResistsIndex::ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1,
            BPAnimationsIndex::ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1
        )},
        {NewBPIndex::IllGill, BPIndexAll(
            BPStatsIndex::ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2,
            BPAttacksIndex::ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2,
            BPResistsIndex::ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2,
            BPAnimationsIndex::ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2
        )},
        {NewBPIndex::IllGillAttack1, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_IllGillAttack1__ep4_SpinnerShambertin2
        )},
        {NewBPIndex::IllGillAttack2, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_IllGillAttack2__ep4_KondrieuPhase1
        )},
        {NewBPIndex::IllGillAttack3, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_IllGillAttack3__ep4_SpinnerKondrieu1
        )},
        {NewBPIndex::Deldepth, BPIndexAll(
            BPStatsIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None,
            BPAttacksIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None,
            BPResistsIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None,
            BPAnimationsIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None
        )},
        {NewBPIndex::Mericarol, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_Mericarol__ep4_None,
            BPAttacksIndex::ep1_None__ep2_Mericarol__ep4_None,
            BPResistsIndex::ep1_None__ep2_Mericarol__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_Mericarol__ep4_None
        )},
        {NewBPIndex::UlGibbon, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_UlGibbon__ep4_None,
            BPAttacksIndex::ep1_None__ep2_UlGibbon__ep4_None,
            BPResistsIndex::ep1_None__ep2_UlGibbon__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_UlGibbon__ep4_None
        )},
        {NewBPIndex::ZolGibbon, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_ZolGibbon__ep4_None,
            BPAttacksIndex::ep1_None__ep2_ZolGibbon__ep4_None,
            BPResistsIndex::ep1_None__ep2_ZolGibbon__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_ZolGibbon__ep4_None
        )},
        {NewBPIndex::Gibbles, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_Gibbles__ep4_None,
            BPAttacksIndex::ep1_None__ep2_Gibbles__ep4_None,
            BPResistsIndex::ep1_None__ep2_Gibbles__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_Gibbles__ep4_None
        )},
        {NewBPIndex::GibblesAttack1, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_GibblesAttack1__ep4_None
        )},
        {NewBPIndex::GibblesAttack2, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_GibblesAttack2__ep4_None
        )},
        {NewBPIndex::Morfos, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_Morfos__ep4_None,
            BPAttacksIndex::ep1_None__ep2_Morfos__ep4_None,
            BPResistsIndex::ep1_None__ep2_Morfos__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_Morfos__ep4_None
        )},
        {NewBPIndex::MorfosAttack1, BPIndexAll(
            BPAttacksIndex::ep1_Gigobooma__ep2_MorfosAttack1__ep4_None
        )},
        {NewBPIndex::Recobox, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_Recobox__ep4_None,
            BPAttacksIndex::ep1_None__ep2_Recobox__ep4_None,
            BPResistsIndex::ep1_None__ep2_Recobox__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_Recobox__ep4_None
        )},
        {NewBPIndex::Recon, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_Recon__ep4_None,
            BPAttacksIndex::ep1_None__ep2_Recon__ep4_None,
            BPResistsIndex::ep1_None__ep2_Recon__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_Recon__ep4_None
        )},
        {NewBPIndex::SinowZoa, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_SinowZoa__ep4_None,
            BPAttacksIndex::ep1_None__ep2_SinowZoa__ep4_None,
            BPResistsIndex::ep1_None__ep2_SinowZoa__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_SinowZoa__ep4_None
        )},
        {NewBPIndex::SinowZele, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_SinowZele__ep4_None,
            BPAttacksIndex::ep1_None__ep2_SinowZele__ep4_None,
            BPResistsIndex::ep1_None__ep2_SinowZele__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_SinowZele__ep4_None
        )},
        {NewBPIndex::Merikle, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_Merikle__ep4_None,
            BPAttacksIndex::ep1_None__ep2_Merikle__ep4_None,
            BPResistsIndex::ep1_None__ep2_Merikle__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_Merikle__ep4_None
        )},
        {NewBPIndex::Mericus, BPIndexAll(
            BPStatsIndex::ep1_None__ep2_Mericus__ep4_None,
            BPAttacksIndex::ep1_None__ep2_Mericus__ep4_None,
            BPResistsIndex::ep1_None__ep2_Mericus__ep4_None,
            BPAnimationsIndex::ep1_None__ep2_Mericus__ep4_None
        )},
        {NewBPIndex::Merillia, BPIndexAll(
            BPStatsIndex::ep1_Booma__ep2_Merillia__ep4_None,
            BPAttacksIndex::ep1_Booma__ep2_Merillia__ep4_None,
            BPResistsIndex::ep1_Booma__ep2_Merillia__ep4_None,
            BPAnimationsIndex::ep1_Booma__ep2_Merillia__ep4_None
        )},
        {NewBPIndex::Meriltas, BPIndexAll(
            BPStatsIndex::ep1_Gobooma__ep2_Meriltas__ep4_None,
            BPAttacksIndex::ep1_Gobooma__ep2_Meriltas__ep4_None,
            BPResistsIndex::ep1_Gobooma__ep2_Meriltas__ep4_None,
            BPAnimationsIndex::ep1_Gobooma__ep2_Meriltas__ep4_None
        )},
        {NewBPIndex::Dolmolm, BPIndexAll(
            BPStatsIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None,
            BPAttacksIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None,
            BPResistsIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None,
            BPAnimationsIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None
        )},
        {NewBPIndex::Dolmdarl, BPIndexAll(
            BPStatsIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None,
            BPAttacksIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None,
            BPResistsIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None,
            BPAnimationsIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None
        )},
        {NewBPIndex::Boota, BPIndexAll(
            BPStatsIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota,
            BPAttacksIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota,
            BPResistsIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota,
            BPAnimationsIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota
        )},
        {NewBPIndex::ZeBoota, BPIndexAll(
            BPStatsIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota,
            BPAttacksIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota,
            BPResistsIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota,
            BPAnimationsIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota
        )},
        {NewBPIndex::ZeBootaAttack1, BPIndexAll(
            BPAttacksIndex::ep1_SavageWolf__ep2_SavageWolf__ep4_ZeBootaAttack1
        )},
        {NewBPIndex::BaBoota, BPIndexAll(
            BPStatsIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota,
            BPAttacksIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota,
            BPResistsIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota,
            BPAnimationsIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota
        )},
        {NewBPIndex::BaBootaAttack1, BPIndexAll(
            BPAttacksIndex::ep1_PoisonLily__ep2_PoisonLily__ep4_BaBootaAttack1
        )},
        {NewBPIndex::ZuCrater, BPIndexAll(
            BPStatsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPAttacksIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPResistsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater,
            BPAnimationsIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater
        )},
        {NewBPIndex::PazuzuCrater, BPIndexAll(
            BPStatsIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater,
            BPAttacksIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater,
            BPResistsIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater,
            BPAnimationsIndex::ep1_CanadineRing__ep2_PigRay__ep4_PazuzuCrater
        )},
        {NewBPIndex::ZuDesert, BPIndexAll(
            BPStatsIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert,
            BPAttacksIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert,
            BPResistsIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert,
            BPAnimationsIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert
        )},
        {NewBPIndex::PazuzuDesert, BPIndexAll(
            BPStatsIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert,
            BPAttacksIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert,
            BPResistsIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert,
            BPAnimationsIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert
        )},
        {NewBPIndex::Astark, BPIndexAll(
            BPStatsIndex::ep1_Canane__ep2_UlRay__ep4_Astark,
            BPAttacksIndex::ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_Astark,
            BPResistsIndex::ep1_Canane__ep2_UlRay__ep4_Astark,
            BPAnimationsIndex::ep1_Canane__ep2_UlRay__ep4_Astark
        )},
        {NewBPIndex::AstarkAttack1, BPIndexAll(
            BPAttacksIndex::ep1_BeeR__ep2_BeeR__ep4_AstarkAttack1
        )},
        {NewBPIndex::AstarkAttack2, BPIndexAll(
            BPAttacksIndex::ep1_BeeL__ep2_BeeL__ep4_AstarkAttack2
        )},
        {NewBPIndex::SatelliteLizardCrater, BPIndexAll(
            BPStatsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPAttacksIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPResistsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater,
            BPAnimationsIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater
        )},
        {NewBPIndex::YowieCrater, BPIndexAll(
            BPStatsIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater,
            BPAttacksIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater,
            BPResistsIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater,
            BPAnimationsIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater
        )},
        {NewBPIndex::SatelliteLizardDesert, BPIndexAll(
            BPStatsIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert,
            BPAttacksIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert,
            BPResistsIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert,
            BPAnimationsIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert
        )},
        {NewBPIndex::YowieDesert, BPIndexAll(
            BPStatsIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert,
            BPAttacksIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert,
            BPResistsIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert,
            BPAnimationsIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert
        )},
        {NewBPIndex::Dorphon, BPIndexAll(
            BPStatsIndex::ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon,
            BPAttacksIndex::ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon,
            BPResistsIndex::ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon,
            BPAnimationsIndex::ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon
        )},
        {NewBPIndex::DorphonEclair, BPIndexAll(
            BPStatsIndex::ep1_DeRolLeShell__ep2_BarbaRayPart__ep4_DorphonEclair,
            BPAttacksIndex::ep1_DeRolLeShell__ep2_BarbaRayAttack1__ep4_DorphonEclair,
            BPResistsIndex::ep1_DeRolLeShell__ep2_BarbaRayPart__ep4_DorphonEclair,
            BPAnimationsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_DorphonEclair
        )},
        {NewBPIndex::Goran, BPIndexAll(
            BPStatsIndex::ep1_DeRolLeMine__ep2_None__ep4_Goran,
            BPAttacksIndex::ep1_DeRolLeMine__ep2_BarbaRayAttack2__ep4_Goran,
            BPResistsIndex::ep1_DeRolLeMine__ep2_None__ep4_Goran,
            BPAnimationsIndex::ep1_DeRolLeMine__ep2_None__ep4_Goran
        )},
        {NewBPIndex::GoranAttack1, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_None__ep4_GoranAttack1
        )},
        {NewBPIndex::PyroGoran, BPIndexAll(
            BPStatsIndex::ep1_Dragon__ep2_GolDragon__ep4_PyroGoran,
            BPAttacksIndex::ep1_Dragon__ep2_GolDragon__ep4_PyroGoran,
            BPResistsIndex::ep1_Dragon__ep2_GolDragon__ep4_PyroGoran,
            BPAnimationsIndex::ep1_Dragon__ep2_GolDragon__ep4_PyroGoran
        )},
        {NewBPIndex::PyroGoranAttack1, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_None__ep4_PyroGoranAttack1
        )},
        {NewBPIndex::GoranDetonator, BPIndexAll(
            BPStatsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator,
            BPAttacksIndex::ep1_BelraFist__ep2_SinowSpigell__ep4_GoranDetonator,
            BPResistsIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator,
            BPAnimationsIndex::ep1_None__ep2_None__ep4_GoranDetonator
        )},
        {NewBPIndex::GoranDetonatorAttack1, BPIndexAll(
            BPAttacksIndex::ep1_None__ep2_None__ep4_GoranDetonatorAttack1
        )},
        {NewBPIndex::MerissaA, BPIndexAll(
            BPStatsIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA,
            BPAttacksIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA,
            BPResistsIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA,
            BPAnimationsIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA
        )},
        {NewBPIndex::MerissaAA, BPIndexAll(
            BPStatsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPAttacksIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPResistsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA,
            BPAnimationsIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA
        )},
        {NewBPIndex::Girtablulu, BPIndexAll(
            BPStatsIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu,
            BPAttacksIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu,
            BPResistsIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu,
            BPAnimationsIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu
        )}
    };

    struct HardcodedBPIndexLocations {
        // size_t because it's more convenient for data entry.
        // vector because some enemies use multiple entries,
        // or their bp is fetched in multiple places.
        std::vector<size_t> stats;
        std::vector<size_t> attacks;
        std::vector<size_t> resists;
        std::vector<size_t> animations;

        HardcodedBPIndexLocations(std::vector<size_t> stats_, std::vector<size_t> attacks_, std::vector<size_t> resists_, std::vector<size_t> animations_) : 
            stats(stats_), attacks(attacks_), resists(resists_), animations(animations_) {}
    };

    std::map<NewBPIndex, HardcodedBPIndexLocations> hardcodedBPIndexLocations = {
        {NewBPIndex::Mothmant, HardcodedBPIndexLocations(
            {0x00518ea6},
            {0x00518eb2},
            {0x00518ebe},
            {0x00518eca}
        )},
        {NewBPIndex::Monest, HardcodedBPIndexLocations(
            {0x0051adfe},
            {0x0051ae0a},
            {0x0051ae16},
            {0x0051ae22}
        )},
        {NewBPIndex::SavageWolf, HardcodedBPIndexLocations(
            {0x00921170},
            {0x00921168},
            {0x00921160},
            {0x00921158}
        )},
        {NewBPIndex::BarbarousWolf, HardcodedBPIndexLocations(
            {0x00921174},
            {0x0092116c},
            {0x00921164},
            {0x0092115c}
        )},
        {NewBPIndex::SinowBeat, HardcodedBPIndexLocations(
            {0x00928b90},
            {0x00928b88},
            {0x00928b80},
            {0x00928b78}
        )},
        {NewBPIndex::Canadine, HardcodedBPIndexLocations(
            {0x00923584},
            {0x00923578},
            {0x0092356c},
            {0x00923560}
        )},
        {NewBPIndex::CanadineRing, HardcodedBPIndexLocations(
            {0x00923588},
            {0x0092357c},
            {0x00923570},
            {0x00923564}
        )},
        {NewBPIndex::Canane, HardcodedBPIndexLocations(
            {0x0092358c},
            {0x00923580},
            {0x00923574},
            {0x00923568}
        )},
        {NewBPIndex::ChaosSorcerer, HardcodedBPIndexLocations(
            {0x0059d724},
            {0x0059d730},
            {0x0059d73c},
            {0x0059d748}
        )},
        {NewBPIndex::BeeR, HardcodedBPIndexLocations(
            {0x00929824},
            {0x0092981c},
            {0x00929814},
            {0x0092980c}
        )},
        {NewBPIndex::BeeL, HardcodedBPIndexLocations(
            {0x00929828},
            {0x00929820},
            {0x00929818},
            {0x00929810}
        )},
        {NewBPIndex::ChaosBringer, HardcodedBPIndexLocations(
            // +1 because I'm just copying the instruction address and
            // too lazy to calculate the address for the operand
            {0x0053c98e + 1},
            {0x0053c99a + 1},
            {0x0053c9a6 + 1},
            {0x0053c9b2 + 1}
        )},
        {NewBPIndex::DarkBelra, HardcodedBPIndexLocations(
            {0x00539407 + 1},
            {0x00922e68},
            {0x00539432 + 1},
            {0x0053943e + 1}
        )},
        {NewBPIndex::BelraFist, HardcodedBPIndexLocations(
            {},
            {0x00922e6c},
            {},
            {}
        )},
        {NewBPIndex::SinowGold, HardcodedBPIndexLocations(
            {0x00928b94},
            {0x00928b8c},
            {0x00928b84},
            {0x00928b7c}
        )},
        {NewBPIndex::NanoDragon, HardcodedBPIndexLocations(
            {0x0057fb5c + 1},
            {0x0057fb68 + 1},
            {0x0057fb74 + 1},
            {0x0057fb80 + 1}
        )},
        {NewBPIndex::Dubchic, HardcodedBPIndexLocations(
            {0x00556d6c + 2},
            {0x00556d6c + 2},
            {0x00556d6c + 2},
            {0x00556d6c + 2}
        )},
        // Can't unhardcode Gillchich because its index is programmed as Dubchic + 1
        {NewBPIndex::Garanz, HardcodedBPIndexLocations(
            {0x0056fe25 + 1},
            {0x0056fe31 + 1},
            {0x0056fe3d + 1},
            {0x0056fe49 + 1}
        )},
        {NewBPIndex::DarkGunner, HardcodedBPIndexLocations(
            {0x005463ab + 1},
            {0x005463b7 + 1},
            {0x005463c3 + 1},
            {0x005463cf + 1}
        )},
        {NewBPIndex::Bulclaw, HardcodedBPIndexLocations(
            {0x0053194d + 1},
            {0x0053195b + 1},
            {0x00531969 + 1},
            {0x00531977 + 1}
        )},
        {NewBPIndex::Claw, HardcodedBPIndexLocations(
            {0x00531990 + 1},
            {0x0053199c + 1},
            {0x005319a8 + 1},
            {0x005319b4 + 1}
        )},
        {NewBPIndex::PofuillySlime, HardcodedBPIndexLocations(
            {0x00599905 + 1},
            {0x0059990e + 1},
            {0x00599917 + 1},
            {0x009bfbd8}
        )},
        {NewBPIndex::PanArms, HardcodedBPIndexLocations(
            {0x00583014 + 1},
            {0x0058307d + 1},
            {0x00583086 + 1},
            // Same entry fetched in multiple places for some reason
            {0x00583144 + 1, 0x0058314e + 1, 0x00583160 + 1}
        )},
        {NewBPIndex::Hidoom, HardcodedBPIndexLocations(
            {0x00586dcc + 1},
            {0x00586dd5 + 1},
            {0x00586dde + 1},
            {0x00586ea7 + 1, 0x00586eb1 + 1}
        )},
        {NewBPIndex::Migium, HardcodedBPIndexLocations(
            {0x00586dfe + 1},
            {0x00586e07 + 1},
            {0x00586e10 + 1},
            {0x00586ed4 + 1, 0x00586ede + 1, 0x00586f24 + 1,
                           0x00586205 + 1, 0x005861ce + 1, 0x00586968 + 1}
        )},
        {NewBPIndex::PouillySlime, HardcodedBPIndexLocations(
            {0x0059992e + 1},
            {0x00599937 + 1},
            {0x00599940 + 1},
            {0x009bfbdc}
        )},
        {NewBPIndex::Hildebear, HardcodedBPIndexLocations(
            {0x005162e6 + 1},
            {0x00516302 + 1},
            {0x00516361 + 1},
            {0x00516380 + 1}
        )},
        {NewBPIndex::HildebearAttack1, HardcodedBPIndexLocations(
            {},
            {0x00516312 + 1},
            {},
            {}
        )},
        {NewBPIndex::HildebearAttack2, HardcodedBPIndexLocations(
            {},
            {0x00516321 + 1},
            {},
            {}
        )},
        {NewBPIndex::Hildeblue, HardcodedBPIndexLocations(
            {0x005162f3 + 1},
            {0x00516331 + 1},
            {0x00516371 + 1},
            {0x00516390 + 1}
        )},
        {NewBPIndex::HildeblueAttack1, HardcodedBPIndexLocations(
            {},
            {0x00516341 + 1},
            {},
            {}
        )},
        {NewBPIndex::HildeblueAttack2, HardcodedBPIndexLocations(
            {},
            {0x00516351 + 1},
            {},
            {}
        )},
        {NewBPIndex::Booma, HardcodedBPIndexLocations(
            {0x00536526 + 1},
            {0x00536552 + 1},
            {0x00536581 + 1},
            {0x005365b0 + 1}
        )},
        {NewBPIndex::Gobooma, HardcodedBPIndexLocations(
            {0x00536533 + 1},
            {0x00536562 + 1},
            {0x00536591 + 1},
            {0x005365c0 + 1}
        )},
        {NewBPIndex::Gigobooma, HardcodedBPIndexLocations(
            {0x00536542 + 1},
            {0x00536571 + 1},
            {0x005365a0 + 1},
            {0x005365cf + 1}
        )},
        {NewBPIndex::GrassAssassin, HardcodedBPIndexLocations(
            {0x00525785 + 1},
            {0x00525791 + 1},
            {0x005257bd + 1},
            {0x005257cc + 1}
        )},
        {NewBPIndex::GrassAssassinAttack1, HardcodedBPIndexLocations(
            {},
            {0x0052579e + 1},
            {},
            {}
        )},
        {NewBPIndex::GrassAssassinAttack2, HardcodedBPIndexLocations(
            {},
            {0x005257ad + 1},
            {},
            {}
        )},
        {NewBPIndex::EvilShark, HardcodedBPIndexLocations(
            {0x00513212 + 1},
            {0x0051323e + 1},
            {0x0051326d + 1},
            {0x0051329c + 1}
        )},
        {NewBPIndex::PalShark, HardcodedBPIndexLocations(
            {0x0051321f + 1},
            {0x0051324e + 1},
            {0x0051327d + 1},
            {0x005132ac + 1}
        )},
        {NewBPIndex::GuilShark, HardcodedBPIndexLocations(
            {0x0051322e + 1},
            {0x0051325d + 1},
            {0x0051328c + 1},
            {0x005132bb + 1}
        )},
        {NewBPIndex::Delsaber, HardcodedBPIndexLocations(
            {0x0055142e + 1},
            {0x0055143a + 1},
            {0x00551466 + 1},
            {0x00551475 + 1}
        )},
        {NewBPIndex::DelsaberAttack1, HardcodedBPIndexLocations(
            {},
            {0x00551447 + 1},
            {},
            {}
        )},
        {NewBPIndex::DelsaberAttack2, HardcodedBPIndexLocations(
            {},
            {0x00551456 + 1},
            {},
            {}
        )},
        {NewBPIndex::Dimenian, HardcodedBPIndexLocations(
            {0x005523c8 + 1},
            {0x005523f4 + 1},
            {0x00552423 + 1},
            {0x00552452 + 1}
        )},
        {NewBPIndex::LaDimenian, HardcodedBPIndexLocations(
            {0x005523d5 + 1},
            {0x00552404 + 1},
            {0x00552433 + 1},
            {0x00552462 + 1}
        )},
        {NewBPIndex::SoDimenian, HardcodedBPIndexLocations(
            {0x005523e4 + 1},
            {0x00552413 + 1},
            {0x00552442 + 1},
            {0x00552471 + 1}
        )},
        {NewBPIndex::SinowBerill, HardcodedBPIndexLocations(
            {0x00929c18},
            {0x00929c10},
            {0x00929c08},
            {0x00929c00}
        )},
        {NewBPIndex::Gee, HardcodedBPIndexLocations(
            // These are actually all identical but I'll play nice just in case it actually matters
            {0x00925624, 0x00925628, 0x0092562c},
            {0x00925618, 0x0092561c, 0x00925620},
            {0x0092560c, 0x00925610, 0x00925614},
            {0x00925600, 0x00925604, 0x00925608}
        )},
        {NewBPIndex::Delbiter, HardcodedBPIndexLocations(
            {0x0054d774 + 1},
            {0x0054d780 + 1},
            {0x0054d78c + 1},
            {0x0054d798 + 1}
        )},
        {NewBPIndex::SinowSpigell, HardcodedBPIndexLocations(
            {0x00929c1c},
            {0x00929c14},
            {0x00929c0c},
            {0x00929c04}
        )},
        {NewBPIndex::GiGue, HardcodedBPIndexLocations(
            {0x0056c3bf + 1},
            {0x0056c3cb + 1},
            {0x0056c3d7 + 1},
            {0x0056c3e3 + 1}
        )},
        {NewBPIndex::Epsilon, HardcodedBPIndexLocations(
            {0x005580d2 + 1},
            {0x005580de + 1},
            {0x005580ea + 1},
            {0x005580f6 + 1}
        )},
        {NewBPIndex::Epsigard, HardcodedBPIndexLocations(
            {0x00558102 + 1},
            {0x0055810e + 1},
            {0x0055811a + 1},
            {0x00558126 + 1}
        )},
        {NewBPIndex::IllGill, HardcodedBPIndexLocations(
            {0x005312bc + 1},
            {0x005312c8 + 1},
            {0x005312f8 + 1},
            {0x00531304 + 1}
        )},
        {NewBPIndex::IllGillAttack1, HardcodedBPIndexLocations(
            {},
            {0x005312d4 + 1},
            {},
            {}
        )},
        {NewBPIndex::IllGillAttack2, HardcodedBPIndexLocations(
            {},
            {0x005312e0 + 1},
            {},
            {}
        )},
        {NewBPIndex::IllGillAttack3, HardcodedBPIndexLocations(
            {},
            {0x005312ec + 1},
            {},
            {}
        )},
        {NewBPIndex::Deldepth, HardcodedBPIndexLocations(
            {0x0054a040 + 1},
            {0x0054a06f + 1},
            {0x0054a078 + 1},
            {0x00549f39 + 1}
        )},
        {NewBPIndex::Mericarol, HardcodedBPIndexLocations(
            {0x00926b44},
            {0x00926b38},
            {0x00926b2c},
            {0x00926b20}
        )},
        {NewBPIndex::UlGibbon, HardcodedBPIndexLocations(
            {0x00925d80},
            {0x00925d78},
            {0x00925d70},
            {0x00925d68}
        )},
        {NewBPIndex::ZolGibbon, HardcodedBPIndexLocations(
            {0x00925d84},
            {0x00925d7c},
            {0x00925d74},
            {0x00925d6c}
        )},
        {NewBPIndex::Gibbles, HardcodedBPIndexLocations(
            {0x00564537 + 1},
            {0x00564543 + 1},
            {0x0056456f + 1},
            {0x0056457e + 1}
        )},
        {NewBPIndex::GibblesAttack1, HardcodedBPIndexLocations(
            {},
            {0x00564550 + 1},
            {},
            {}
        )},
        {NewBPIndex::GibblesAttack2, HardcodedBPIndexLocations(
            {},
            {0x0056455f + 1},
            {},
            {}
        )},
        {NewBPIndex::Morfos, HardcodedBPIndexLocations(
            {0x0057c8c0 + 1},
            {0x009275f8},
            {0x0057c8eb + 1},
            {0x0057c8f7 + 1}
        )},
        {NewBPIndex::MorfosAttack1, HardcodedBPIndexLocations(
            {},
            {0x009275fc},
            {},
            {}
        )},
        {NewBPIndex::Recobox, HardcodedBPIndexLocations(
            {0x00589d57 + 1},
            {0x00589d63 + 1},
            {0x00589d6f + 1},
            {0x00589d7b + 1}
        )},
        {NewBPIndex::Recon, HardcodedBPIndexLocations(
            {0x0058c55d + 1},
            {0x0058c575 + 1},
            {0x0058c581 + 1},
            {0x0058c58d + 1}
        )},
        {NewBPIndex::SinowZoa, HardcodedBPIndexLocations(
            {0x00928fb8},
            {0x00928fb0},
            {0x00928fa8},
            {0x00928fa0}
        )},
        {NewBPIndex::SinowZele, HardcodedBPIndexLocations(
            {0x00928fbc},
            {0x00928fb4},
            {0x00928fac},
            {0x00928fa4}
        )},
        {NewBPIndex::Merikle, HardcodedBPIndexLocations(
            {0x00926b48},
            {0x00926b3c},
            {0x00926b30},
            {0x00926b24}
        )},
        {NewBPIndex::Mericus, HardcodedBPIndexLocations(
            {0x00926b4c},
            {0x00926b40},
            {0x00926b34},
            {0x00926b28}
        )},
        {NewBPIndex::Merillia, HardcodedBPIndexLocations(
            {0x005763f9 + 1},
            {0x00576415 + 1},
            {0x00576434 + 1},
            {0x00576453 + 1}
        )},
        {NewBPIndex::Meriltas, HardcodedBPIndexLocations(
            {0x00576406 + 1},
            {0x00576425 + 1},
            {0x00576444 + 1},
            {0x00576463 + 1}
        )},
        {NewBPIndex::Dolmolm, HardcodedBPIndexLocations(
            {0x0055432f + 1},
            {0x0055434b + 1},
            {0x0055436a + 1},
            {0x00554389 + 1}
        )},
        {NewBPIndex::Dolmdarl, HardcodedBPIndexLocations(
            {0x0055433c + 1},
            {0x0055435b + 1},
            {0x0055437a + 1},
            {0x00554399 + 1}
        )},
        {NewBPIndex::Boota, HardcodedBPIndexLocations(
            {0x005a629e + 1},
            {0x005a62ca + 1},
            {0x005a6319 + 1},
            {0x005a6348 + 1}
        )},
        {NewBPIndex::ZeBoota, HardcodedBPIndexLocations(
            {0x005a62ab + 1},
            {0x005a62da + 1},
            {0x005a6329 + 1},
            {0x005a6358 + 1}
        )},
        {NewBPIndex::ZeBootaAttack1, HardcodedBPIndexLocations(
            {},
            {0x005a62f9 + 1},
            {},
            {}
        )},
        {NewBPIndex::BaBoota, HardcodedBPIndexLocations(
            {0x005a62ba + 1},
            {0x005a62e9 + 1},
            {0x005a6338 + 1},
            {0x005a6367 + 1}
        )},
        {NewBPIndex::BaBootaAttack1, HardcodedBPIndexLocations(
            {},
            {0x005a6309 + 1},
            {},
            {}
        )},
        {NewBPIndex::ZuCrater, HardcodedBPIndexLocations(
            {0x0092acbc},
            {0x0092acbc},
            {0x0092acbc},
            {0x0092acbc}
        )},
        {NewBPIndex::PazuzuCrater, HardcodedBPIndexLocations(
            {0x0092acc0},
            {0x0092acc0},
            {0x0092acc0},
            {0x0092acc0}
        )},
        {NewBPIndex::ZuDesert, HardcodedBPIndexLocations(
            {0x0092acc4},
            {0x0092acc4},
            {0x0092acc4},
            {0x0092acc4}
        )},
        {NewBPIndex::PazuzuDesert, HardcodedBPIndexLocations(
            {0x0092acc8},
            {0x0092acc8},
            {0x0092acc8},
            {0x0092acc8}
        )},
        {NewBPIndex::Astark, HardcodedBPIndexLocations(
            {0x005a4e28 + 1},
            {0x005a4e35 + 1},
            {0x005a4e63 + 1},
            {0x005a4e73 + 1}
        )},
        {NewBPIndex::AstarkAttack1, HardcodedBPIndexLocations(
            {},
            {0x005a4e44 + 1},
            {},
            {}
        )},
        {NewBPIndex::AstarkAttack2, HardcodedBPIndexLocations(
            {},
            {0x005a4e54 + 1},
            {},
            {}
        )},
        {NewBPIndex::SatelliteLizardCrater, HardcodedBPIndexLocations(
            {0x0092a968},
            {0x0092a968},
            {0x0092a968},
            {0x0092a968}
        )},
        {NewBPIndex::YowieCrater, HardcodedBPIndexLocations(
            {0x0092a96c},
            {0x0092a96c},
            {0x0092a96c},
            {0x0092a96c}
        )},
        {NewBPIndex::SatelliteLizardDesert, HardcodedBPIndexLocations(
            {0x0092a970},
            {0x0092a970},
            {0x0092a970},
            {0x0092a970}
        )},
        {NewBPIndex::YowieDesert, HardcodedBPIndexLocations(
            {0x0092a974},
            {0x0092a974},
            {0x0092a974},
            {0x0092a974}
        )},
        {NewBPIndex::Dorphon, HardcodedBPIndexLocations(
            {0x005a68bc + 1},
            {0x005a68c6 + 1},
            {0x005a68d6 + 1},
            // Same entry fetched in multiple places for some reason
            {0x005a9b0c + 1, 0x005a68e6 + 1}
        )},
        {NewBPIndex::DorphonEclair, HardcodedBPIndexLocations(
            {0x005a6877 + 1},
            {0x005a6881 + 1},
            {0x005a6891 + 1},
            // Same entry fetched in multiple places for some reason
            {0x005a9b2c + 1, 0x005a68a1 + 1}
        )},
        {NewBPIndex::Goran, HardcodedBPIndexLocations(
            {0x005ae211 + 1},
            {0x005ae23d + 1},
            {0x005ae29c + 1},
            {0x005ae2cb + 1}
        )},
        {NewBPIndex::GoranAttack1, HardcodedBPIndexLocations(
            {},
            {0x005ae26c + 1},
            {},
            {}
        )},
        {NewBPIndex::PyroGoran, HardcodedBPIndexLocations(
            {0x005ae21e + 1},
            {0x005ae24d + 1},
            {0x005ae2ac + 1},
            {0x005ae2db + 1}
        )},
        {NewBPIndex::PyroGoranAttack1, HardcodedBPIndexLocations(
            {},
            {0x005ae27c + 1},
            {},
            {}
        )},
        {NewBPIndex::GoranDetonator, HardcodedBPIndexLocations(
            {0x005ae22d + 1},
            {0x005ae25c + 1},
            {0x005ae2bb + 1},
            {0x005ae2ea + 1}
        )},
        {NewBPIndex::GoranDetonatorAttack1, HardcodedBPIndexLocations(
            {},
            {0x005ae28c + 1},
            {},
            {}
        )},
        {NewBPIndex::MerissaA, HardcodedBPIndexLocations(
            {0x009c1db8},
            {0x009c1db8},
            {0x009c1db8},
            // Same entry fetched in multiple places for some reason
            {0x005b7141 + 1, 0x009c1db8}
        )},
        {NewBPIndex::MerissaAA, HardcodedBPIndexLocations(
            {0x009c1dcc},
            {0x009c1dcc},
            {0x009c1dcc},
            // Same entry fetched in multiple places for some reason
            {0x005b714a + 1, 0x009c1dcc}
        )},
        {NewBPIndex::Girtablulu, HardcodedBPIndexLocations(
            {0x005ac745 + 1},
            {0x005ac75b + 1},
            {0x005ac767 + 1},
            {0x005ac773 + 1}
        )},
    };

    std::map<NewBPIndex, InitList::FunctionPair> enemyInitFuncPairs = {
        // {NewBPIndex::Mothmant             , InitList::FunctionPair(0x, 0x)}, // Inside Monest's function
        {NewBPIndex::Monest               , InitList::FunctionPair(0x0051ad94, 0x00519120)},
        {NewBPIndex::SavageWolf           , InitList::FunctionPair(0x0051ea94, 0x0051b24c)},
        //{NewBPIndex::BarbarousWolf        , InitList::FunctionPair(0x, 0x)}, // Same as Savage Wolf
        {NewBPIndex::PoisonLily           , InitList::FunctionPair(0x00529cf8, 0x0052930c)},
        // {NewBPIndex::NarLily              , InitList::FunctionPair(0x, 0x)}, // Same as Poison Lily
        {NewBPIndex::SinowBeat            , InitList::FunctionPair(0x0059051c, 0x0058d268)},
        {NewBPIndex::Canadine             , InitList::FunctionPair(0x005420f4, 0x0053e718)},
        // {NewBPIndex::CanadineRing         , InitList::FunctionPair(0x, 0x)}, // Same as Canadine
        // {NewBPIndex::Canane               , InitList::FunctionPair(0x, 0x)},
        /*
        {NewBPIndex::ChaosSorcerer        , InitList::FunctionPair(0x0059d6ac, 0x0059b164)},
        // {NewBPIndex::BeeR                 , InitList::FunctionPair(0x, 0x)},
        // {NewBPIndex::BeeL                 , InitList::FunctionPair(0x, 0x)}, // Inside Chaos Sorcerer's function
        {NewBPIndex::ChaosBringer         , InitList::FunctionPair(0x0053c914, 0x0053a378)},
        {NewBPIndex::DarkBelra            , InitList::FunctionPair(0x00538ef4, 0x00536b0c)},
        */
        // {NewBPIndex::SinowGold            , InitList::FunctionPair(0x, 0x)}, // Same as Sinow Beat
        {NewBPIndex::RagRappy             , InitList::FunctionPair(0x00527934, 0x00526c64)},
        // {NewBPIndex::AlRappy              , InitList::FunctionPair(0x, 0x)}, // Same as Rag Rappy
        {NewBPIndex::NanoDragon           , InitList::FunctionPair(0x0057fae0, 0x0057f5c4)},
        {NewBPIndex::Dubchic              , InitList::FunctionPair(0x005569bc, 0x00556528)},
        // {NewBPIndex::Gillchic             , InitList::FunctionPair(0x, 0x)}, // Same as Dubchic
        {NewBPIndex::Garanz               , InitList::FunctionPair(0x0056fc10, 0x0056f8b8)},
        {NewBPIndex::DarkGunner           , InitList::FunctionPair(0x005462b0, 0x00545e10)},
        {NewBPIndex::Bulclaw              , InitList::FunctionPair(0x00531808, 0x005317c8)},
        // {NewBPIndex::Claw                 , InitList::FunctionPair(0x, 0x)}, // Same as Bulclaw
        {NewBPIndex::PofuillySlime        , InitList::FunctionPair(0x00599a20, 0x00595ba8)},
        {NewBPIndex::PanArms              , InitList::FunctionPair(0x00583450, 0x0058335c)},
        // {NewBPIndex::Hidoom               , InitList::FunctionPair(0x, 0x)}, // Same as Pan Arms
        // {NewBPIndex::Migium               , InitList::FunctionPair(0x, 0x)}, // Same as Pan Arms
        // {NewBPIndex::PouillySlime         , InitList::FunctionPair(0x, 0x)}, // Same as Pofuilly Slime
        {NewBPIndex::Dubwitch             , InitList::FunctionPair(0x006194e4, 0x00618a48)},
        {NewBPIndex::Hildebear            , InitList::FunctionPair(0x00516124, 0x005152e4)},
        // {NewBPIndex::Hildeblue            , InitList::FunctionPair(0x, 0x)}, // Same as Hildebear
        {NewBPIndex::Booma                , InitList::FunctionPair(0x00536314, 0x00535d90)},
        // {NewBPIndex::Gobooma              , InitList::FunctionPair(0x, 0x)}, // Same as Booma
        // {NewBPIndex::Gigobooma            , InitList::FunctionPair(0x, 0x)}, // Same as Booma
        {NewBPIndex::GrassAssassin        , InitList::FunctionPair(0x005255b8, 0x00524888)},
        {NewBPIndex::EvilShark            , InitList::FunctionPair(0x0051303c, 0x00512a44)},
        // {NewBPIndex::PalShark             , InitList::FunctionPair(0x, 0x)}, // Same as Evil Shark
        // {NewBPIndex::GuilShark            , InitList::FunctionPair(0x, 0x)}, // Same as Evil Shark
        {NewBPIndex::Delsaber             , InitList::FunctionPair(0x005511a4, 0x00550030)},
        {NewBPIndex::Dimenian             , InitList::FunctionPair(0x00552198, 0x00551bb0)},
        // {NewBPIndex::LaDimenian           , InitList::FunctionPair(0x, 0x)}, // Same as Dimenian
        // {NewBPIndex::SoDimenian           , InitList::FunctionPair(0x, 0x)}, // Same as Dimenian
        {NewBPIndex::SinowBerill          , InitList::FunctionPair(0x005a155c, 0x0059dfa8)},
        {NewBPIndex::Gee                  , InitList::FunctionPair(0x0055f7dc, 0x0055c918)},
        {NewBPIndex::Delbiter             , InitList::FunctionPair(0x0054d724, 0x0054a680)},
        // {NewBPIndex::SinowSpigell         , InitList::FunctionPair(0x, 0x)}, // Same as Sinow Berill
        {NewBPIndex::GiGue                , InitList::FunctionPair(0x0056c2e0, 0x0056bda4)},
        {NewBPIndex::Epsilon              , InitList::FunctionPair(0x00557f90, 0x00557f18)},
        // {NewBPIndex::Epsigard             , InitList::FunctionPair(0x, 0x)}, // Same as Epsilon
        {NewBPIndex::IllGill              , InitList::FunctionPair(0x00531240, 0x0052e25c)},
        {NewBPIndex::Deldepth             , InitList::FunctionPair(0x00549db0, 0x00549a60)},
        {NewBPIndex::Mericarol            , InitList::FunctionPair(0x00573638, 0x00572b24)},
        {NewBPIndex::UlGibbon             , InitList::FunctionPair(0x00568fb0, 0x005649c8)},
        // {NewBPIndex::ZolGibbon            , InitList::FunctionPair(0x, 0x)}, // Same as Ul Gibbon
        {NewBPIndex::Gibbles              , InitList::FunctionPair(0x00564440, 0x005630fc)},
        {NewBPIndex::Morfos               , InitList::FunctionPair(0x0057c830, 0x0057a094)},
        {NewBPIndex::Recobox              , InitList::FunctionPair(0x00589d08, 0x005884fc)},
        // {NewBPIndex::Recon                , InitList::FunctionPair(0x, 0x)}, // Inside Recobox's function
        {NewBPIndex::SinowZoa             , InitList::FunctionPair(0x00594c30, 0x0059147c)},
        // {NewBPIndex::SinowZele            , InitList::FunctionPair(0x, 0x)}, // Same as Sinow Zoa
        // {NewBPIndex::Merikle              , InitList::FunctionPair(0x, 0x)}, // Same as Mericarol
        // {NewBPIndex::Mericus              , InitList::FunctionPair(0x, 0x)}, // Same as Mericarol
        {NewBPIndex::Merillia             , InitList::FunctionPair(0x005762cc, 0x00575d1c)},
        // {NewBPIndex::Meriltas             , InitList::FunctionPair(0x, 0x)}, // Same as Merillia
        {NewBPIndex::Dolmolm              , InitList::FunctionPair(0x00554114, 0x00553af0)},
        // {NewBPIndex::Dolmdarl             , InitList::FunctionPair(0x, 0x)}, // Same as Dolmolm
        {NewBPIndex::Boota                , InitList::FunctionPair(0x005a60cc, 0x005a5b48)},
        // {NewBPIndex::ZeBoota              , InitList::FunctionPair(0x, 0x)}, // Same as Boota
        // {NewBPIndex::BaBoota              , InitList::FunctionPair(0x, 0x)}, // Same as Boota
        {NewBPIndex::ZuCrater             , InitList::FunctionPair(0x005b4f5c, 0x005b4798)},
        // {NewBPIndex::PazuzuCrater         , InitList::FunctionPair(0x, 0x)}, // Same as Zu
        // {NewBPIndex::ZuDesert             , InitList::FunctionPair(0x, 0x)}, // Same as other Zu
        // {NewBPIndex::PazuzuDesert         , InitList::FunctionPair(0x, 0x)}, // Same as Zu
        {NewBPIndex::Astark               , InitList::FunctionPair(0x005a4c3c, 0x005a3cac)},
        {NewBPIndex::SatelliteLizardCrater, InitList::FunctionPair(0x005b21e4, 0x005ae7ac)},
        // {NewBPIndex::YowieCrater          , InitList::FunctionPair(0x, 0x)}, // Same as Satellite Lizard
        // {NewBPIndex::SatelliteLizardDesert, InitList::FunctionPair(0x, 0x)}, // Same as Satellite Lizard
        // {NewBPIndex::YowieDesert          , InitList::FunctionPair(0x, 0x)}, // Same as Satellite Lizard
        {NewBPIndex::Dorphon              , InitList::FunctionPair(0x005a9adc, 0x005a671c)},
        // {NewBPIndex::DorphonEclair        , InitList::FunctionPair(0x, 0x)}, // Same as Dorphon
        {NewBPIndex::Goran                , InitList::FunctionPair(0x005adfc8, 0x005ad9e8)},
        // {NewBPIndex::PyroGoran            , InitList::FunctionPair(0x, 0x)}, // Same as Goran
        // {NewBPIndex::GoranDetonator       , InitList::FunctionPair(0x, 0x)}, // Same as Goran
        {NewBPIndex::MerissaA             , InitList::FunctionPair(0x005b70ac, 0x005b6f0c)},
        // {NewBPIndex::MerissaAA            , InitList::FunctionPair(0x, 0x)}, // Same as Merissa
        {NewBPIndex::Girtablulu           , InitList::FunctionPair(0x005ac6e4, 0x005ab954)},
    };

    NpcType newEnemies[] = {
        NpcType::Hildebear,
        NpcType::Rag_Rappy_Sand_Rappy,
        NpcType::Monest,
        NpcType::Savage_Wolf,
        NpcType::Booma,
        NpcType::Grass_Assassin,
        NpcType::Poison_Lily_Del_Lily,
        NpcType::Nano_Dragon,
        NpcType::Evil_Shark,
        NpcType::Pofuilly_Slime,
        NpcType::Pan_Arms,
        NpcType::Gillchic,
        NpcType::Garanz,
        NpcType::Sinow_Beat,
        NpcType::Canadine,
        NpcType::Canane,
        NpcType::Dubchic_Switch,
        NpcType::Delsaber,
        NpcType::Chaos_Sorcerer,
        NpcType::Dark_Gunner,
        NpcType::Death_Gunner,
        NpcType::Chaos_Bringer,
        NpcType::Darth_Belra,
        NpcType::Dimenian,
        NpcType::Bulclaw,
        NpcType::Claw,
        NpcType::Sinow_Berill,
        NpcType::Merillias,
        NpcType::Mericarol,
        NpcType::Ul_Gibbon,
        NpcType::Gibbles,
        NpcType::Gee,
        NpcType::Gi_Gue,
        NpcType::Deldepth,
        NpcType::Delbiter,
        NpcType::Dolmdarl,
        NpcType::Morfos,
        NpcType::Recobox,
        NpcType::Epsilon_Sinow_Zoa,
        NpcType::Ill_Gill,
        NpcType::Astark,
        NpcType::Satellite_Lizard,
        NpcType::Merissa_A,
        NpcType::Girt,
        NpcType::Zu,
        NpcType::Boota,
        NpcType::Dorphon,
        NpcType::Goran,
    };

    /// Add enemies to the lists that the game uses to find the enemy constructor for each map
    void PatchMapEnemyLists()
    {
        // Find constructors
        std::map<NpcType, Enemy::TaggedEnemyConstructor*> enemyConstructors = {};
        for (NpcType enemy : newEnemies)
        {
            auto constructor = Enemy::FindEnemyConstructor(enemy);
            if (constructor == nullptr)
            {
                throw std::runtime_error("Failed to find constructor for enemy " + std::to_string((size_t) enemy));
            }
            enemyConstructors[enemy] = constructor;
        }

        // Ensure Epsilon is chosen over Sinow Zoa
        enemyConstructors[NpcType::Epsilon_Sinow_Zoa] = reinterpret_cast<Enemy::TaggedEnemyConstructor*>(0x009fae90);

        // Add new enemies to all maps
        for (size_t i = 0; i <= (size_t) MapType::MAX_INDEX; i++)
        {
            auto& mapEnemyList = Enemy::GetEnemyConstructorList((MapType) i);

            // Compute union of old list and new list
            for (NpcType newEnemy : newEnemies)
            {
                bool found = false;

                for (const Enemy::TaggedEnemyConstructor& oldEnemy : mapEnemyList)
                {
                    if (oldEnemy.enemyType == newEnemy)
                    {
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    mapEnemyList.push_back(*enemyConstructors[newEnemy]);
                }
            }
        }
    }

    /// Add enemy global init functions to each map's initlist
    void PatchMapInitLists()
    {
        for (size_t i = 0; i <= (size_t) MapType::MAX_INDEX; i++)
        {
            InitList& mapInitList = Map::GetMapInitList((MapType) i);

            for (const auto& entry : enemyInitFuncPairs)
            {
                mapInitList.AddFunctionPair(entry.second);
            }
        }
    }

    bool CurrentMapIsCrater()
    {
        switch (GetCurrentMap())
        {
            case MapType::Wilds1:
            case MapType::Wilds2:
            case MapType::Wilds3:
            case MapType::Wilds4:
            case MapType::Crater:
                return true;
        }

        return false;
    }

    bool CurrentMapIsDesert()
    {
        switch (GetCurrentMap())
        {
            case MapType::Desert1:
            case MapType::Desert2:
            case MapType::Desert3:
                return true;
        }

        return false;
    }

    /// Which episode an enemy is from
    Episode GetNewBPIndexEpisode(NewBPIndex i)
    {
        if (i >= NewBPIndex::EP4_START_INDEX && i <= NewBPIndex::EP4_END_INDEX)
        {
            return Episode::Episode4;
        }
        else
        {
            if (std::find(ep1AndEp2SharedEnemies.begin(), ep1AndEp2SharedEnemies.end(), i) != ep1AndEp2SharedEnemies.end())
            {
                Episode current = GetCurrentEpisode();

                if (current == Episode::Episode4)
                {
                    // Use Episode 1 stats for Crater and Episode 2 stats for Desert
                    if (CurrentMapIsCrater())
                    {
                        return Episode::Episode1;
                    }
                    else if (CurrentMapIsDesert())
                    {
                        return Episode::Episode2;
                    }
                }

                // Use current episode for enemies which appear in both Episode 1 and 2
                return current;
            }

            if (i >= NewBPIndex::EP2_START_INDEX && i <= NewBPIndex::EP2_END_INDEX)
            {
                return Episode::Episode2;
            }
            else if (i >= NewBPIndex::EP1_START_INDEX && i <= NewBPIndex::EP1_END_INDEX)
            {
                return Episode::Episode1;
            }
        }

        throw std::invalid_argument("Invalid NewBP index");
    }

    /// Transforms a NewBPIndex (new) to a BPIndex (old)
    uint8_t DecodeNewBpIndex(NewBPIndex i, BPEntryType entryType)
    {
        if (GetCurrentEpisode() == Episode::Episode2)
        {
            // Map Crater to Desert in Episode 2 (Desert is only requested when actually in Desert)
            switch (i)
            {
                // Rappies are not here because they are excluded from Omnispawn
                case NewBPIndex::ZuCrater:
                    i = NewBPIndex::ZuDesert;
                    break;
                case NewBPIndex::PazuzuCrater:
                    i = NewBPIndex::PazuzuDesert;
                    break;
                case NewBPIndex::SatelliteLizardCrater:
                    i = NewBPIndex::SatelliteLizardDesert;
                    break;
                case NewBPIndex::YowieCrater:
                    i = NewBPIndex::YowieDesert;
                    break;
            }
        }

        auto entry = newBpIndexMap.find(i);
        if (entry == newBpIndexMap.end())
        {
            throw std::invalid_argument("NewBPIndex not found in index map");
        }

        BPIndexAll origIdx = (*entry).second;

        switch (entryType)
        {
            case BPEntryType::Stats:
                return (uint8_t) origIdx.stats;
            case BPEntryType::Attacks:
                return (uint8_t) origIdx.attacks;
            case BPEntryType::Resists:
                return (uint8_t) origIdx.resists;
            case BPEntryType::Animations:
                return (uint8_t) origIdx.animations;
        }

        throw std::runtime_error("Unreachable");
    }

    /// Get a new BP entry
    void* GetNewBPEntry(NewBPIndex i, BPEntryType entryType)
    {
        Episode fromEp = GetNewBPIndexEpisode(i);
        uint8_t decoded = DecodeNewBpIndex(i, entryType);

        return BattleParam::GetBPEntry(fromEp, IsSoloMode(), decoded, entryType);
    }

    /// Get a new or old BP entry
    void* GetOmnispawnBPEntry(uint8_t i, BPEntryType entryType)
    {
        if (i <= (uint8_t) BattleParam::END_INDEX)
        {
            // Vanilla index
            return BattleParam::GetBPEntry(GetCurrentEpisode(), IsSoloMode(), i, entryType);
        }

        return GetNewBPEntry(static_cast<NewBPIndex>(i), entryType);
    }

    void* __cdecl GetOmnispawnBPStatsEntry(uint8_t i)
    {
        return GetOmnispawnBPEntry(i, BPEntryType::Stats);
    }

    void* __cdecl GetOmnispawnBPAttacksEntry(uint8_t i)
    {
        return GetOmnispawnBPEntry(i, BPEntryType::Attacks);
    }

    void* __cdecl GetOmnispawnBPResistsEntry(uint8_t i)
    {
        return GetOmnispawnBPEntry(i, BPEntryType::Resists);
    }

    void* __cdecl GetOmnispawnBPAnimationsEntry(uint8_t i)
    {
        return GetOmnispawnBPEntry(i, BPEntryType::Animations);
    }

    /// Patch BP getter functions to allow new BP indices
    void PatchBPGetters()
    {
        PatchJMP(0x0077a684, 0x0077a696, (int) GetOmnispawnBPStatsEntry);
        PatchJMP(0x0077a698, 0x0077a6b1, (int) GetOmnispawnBPAttacksEntry);
        PatchJMP(0x0077a6b4, 0x0077a6c9, (int) GetOmnispawnBPResistsEntry);
        PatchJMP(0x0077a6cc, 0x0077a6e5, (int) GetOmnispawnBPAnimationsEntry);
    }

    /// Replace all hardcoded arguments to BP getter functions
    void UnhardcodeBattleParamIndices()
    {
        for (const auto& entry : hardcodedBPIndexLocations)
        {
            uint8_t bpI = (uint8_t) entry.first;
            HardcodedBPIndexLocations codeLocs = entry.second;

            for (size_t loc : codeLocs.stats) {
                *(uint8_t*)loc = bpI;
            }

            for (size_t loc : codeLocs.resists) {
                *(uint8_t*)loc = bpI;
            }

            for (size_t loc : codeLocs.attacks) {
                *(uint8_t*)loc = bpI;
            }

            for (size_t loc : codeLocs.animations) {
                *(uint8_t*)loc = bpI;
            }
        }
    }

    /// Enables loading assets in Pioneer 2 and Lobby that are normally only loaded on Ragol
    void PatchRagolAssetLoading()
    {
        // jnz -> jmp
        *(uint8_t*)0x00782496 = 0xeb;
    }

    typedef uint32_t (__cdecl *LoadMapSoundDataFunction)(uint32_t);
    LoadMapSoundDataFunction LoadMapSoundData = reinterpret_cast<LoadMapSoundDataFunction>(0x00828d40);

    /// Loads all map-specific .pac files
    void __cdecl LoadSoundDataAllMaps()
    {
        for (uint32_t i = 0; i <= (uint32_t) MapType::MAX_INDEX; i++)
        {
            if (LoadMapSoundData(i) == 0)
            {
                return;
            }
        }

        return;
    }

    void PatchSoundEffects()
    {
        // This is an initlist that loads assets that should stay loaded 
        // for the entire lifetime of the program
        InitList& lst = InitList::GetInitList(0x009ff6e0, 0x009ff7c0);
        // No uninit function because at that point the game is exiting anyway
        lst.AddFunctionPair(InitList::FunctionPair(LoadSoundDataAllMaps, nullptr));
        lst.AddListReferenceAddress({0x007a666d + 1, 0x007a63a7 + 1});
        lst.AddSizeReferenceAddress({0x007a63a2 + 1, 0x007a6668 + 1});

        // Stub out load_map_sound_data_:00815584
        // Because verything should already be loaded
        StubOutFunction(0x00815584, 0x0081558f);

        // Stub out unload_map_sound_data:00828c38
        // Because we wan't to keep everything loaded permanently
        StubOutFunction(0x00828c38, 0x00828d24);
    }

    bool patchApplied = false;
    void ApplyOmnispawnPatch()
    {
        if (patchApplied) {
            return;
        }

        PatchMapEnemyLists();
        PatchMapInitLists();
        PatchBPGetters();
        UnhardcodeBattleParamIndices();
        PatchRagolAssetLoading();
        PatchSoundEffects();

        patchApplied = true;
    }
};
