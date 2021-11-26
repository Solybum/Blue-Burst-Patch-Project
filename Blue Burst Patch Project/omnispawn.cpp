#include <cstddef>
#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>
#include "omnispawn.h"
#include "common.h"
#include "map.h"
#include "enemy.h"
#include "battleparam.h"
#include "helpers.h"

namespace Omnispawn
{
    using Enemy::NpcType;
    using Map::MapType;
    using BattleParam::BPIndex;
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

    /// Map new indices to old
    std::map<NewBPIndex, BPIndex> newBpIndexMap = {
        {NewBPIndex::Mothmant             , BPIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota                    },
        {NewBPIndex::Monest               , BPIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota                      },
        {NewBPIndex::SavageWolf           , BPIndex::ep1_SavageWolf__ep2_SavageWolf__ep4_None                 },
        {NewBPIndex::BarbarousWolf        , BPIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota        },
        {NewBPIndex::PoisonLily           , BPIndex::ep1_PoisonLily__ep2_PoisonLily__ep4_None                 },
        {NewBPIndex::NarLily              , BPIndex::ep1_NarLily__ep2_NarLily__ep4_SandRappyCrater            },
        {NewBPIndex::SinowBeat            , BPIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater       },
        {NewBPIndex::Canadine             , BPIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater                      },
        {NewBPIndex::CanadineRing         , BPIndex::ep1_CanadineRing__ep2_None__ep4_PazuzuCrater             },
        {NewBPIndex::Canane               , BPIndex::ep1_Canane__ep2_None__ep4_Astark                         },
        {NewBPIndex::ChaosSorcerer        , BPIndex::ep1_ChaosSorcerer__ep2_ChaosSorcerer__ep4_None           },
        {NewBPIndex::BeeR                 , BPIndex::ep1_BeeR__ep2_BeeR__ep4_None                             },
        {NewBPIndex::BeeL                 , BPIndex::ep1_BeeL__ep2_BeeL__ep4_None                             },
        {NewBPIndex::ChaosBringer         , BPIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater},
        {NewBPIndex::DarkBelra            , BPIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater            },
        {NewBPIndex::SinowGold            , BPIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator      },
        {NewBPIndex::RagRappy             , BPIndex::ep1_RagRappy__ep2_RagRappy__ep4_DelRappyDesert           },
        {NewBPIndex::AlRappy              , BPIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA                 },
        {NewBPIndex::NanoDragon           , BPIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA                 },
        {NewBPIndex::Dubchic              , BPIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert                   },
        {NewBPIndex::Gillchic             , BPIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert             },
        {NewBPIndex::Garanz               , BPIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert        },
        {NewBPIndex::DarkGunner           , BPIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert          },
        {NewBPIndex::Bulclaw              , BPIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu                    },
        {NewBPIndex::Claw                 , BPIndex::ep1_Claw__ep2_None__ep4_SaintMilionPhase1                },
        {NewBPIndex::PofuillySlime        , BPIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None                },
        {NewBPIndex::PanArms              , BPIndex::ep1_PanArms__ep2_PanArms__ep4_None                       },
        {NewBPIndex::Hidoom               , BPIndex::ep1_Hidoom__ep2_Hidoom__ep4_None                         },
        {NewBPIndex::Migium               , BPIndex::ep1_Migium__ep2_Migium__ep4_None                         },
        {NewBPIndex::PouillySlime         , BPIndex::ep1_PouillySlime__ep2_None__ep4_None                     },
        {NewBPIndex::Dubwitch             , BPIndex::ep1_Dubwitch__ep2_Dubwitch__ep4_None                     },
        {NewBPIndex::Hildebear            , BPIndex::ep1_Hildebear__ep2_Hildebear__ep4_None                   },
        {NewBPIndex::Hildeblue            , BPIndex::ep1_Hildeblue__ep2_Hildeblue__ep4_None                   },
        {NewBPIndex::Booma                , BPIndex::ep1_Booma__ep2_Merillia__ep4_None                        },
        {NewBPIndex::Gobooma              , BPIndex::ep1_Gobooma__ep2_Meriltas__ep4_None                      },
        {NewBPIndex::Gigobooma            , BPIndex::ep1_Gigobooma__ep2_None__ep4_None                        },
        {NewBPIndex::GrassAssassin        , BPIndex::ep1_GrassAssassin__ep2_GrassAssassin__ep4_None           },
        {NewBPIndex::EvilShark            , BPIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None                     },
        {NewBPIndex::PalShark             , BPIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None                     },
        {NewBPIndex::GuilShark            , BPIndex::ep1_GuilShark__ep2_None__ep4_None                        },
        {NewBPIndex::Delsaber             , BPIndex::ep1_Delsaber__ep2_Delsaber__ep4_None                     },
        {NewBPIndex::Dimenian             , BPIndex::ep1_Dimenian__ep2_Dimenian__ep4_None                     },
        {NewBPIndex::LaDimenian           , BPIndex::ep1_LaDimenian__ep2_LaDimenian__ep4_None                 },
        {NewBPIndex::SoDimenian           , BPIndex::ep1_SoDimenian__ep2_SoDimenian__ep4_None                 },

        {NewBPIndex::SinowBerill          , BPIndex::ep1_SinowBeat__ep2_SinowBerill__ep4_DelRappyCrater       },
        {NewBPIndex::Gee                  , BPIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater                      },
        {NewBPIndex::Delbiter             , BPIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater},
        {NewBPIndex::SinowSpigell         , BPIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator      },
        {NewBPIndex::GiGue                , BPIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA                 },
        {NewBPIndex::Epsilon              , BPIndex::ep1_VolOptMonitor__ep2_Epsilon__ep4_SpinnerSaintMilion2  },
        {NewBPIndex::Epsigard             , BPIndex::ep1_VolOptSpire__ep2_Epsigard__ep4_ShambertinPhase1      },
        {NewBPIndex::IllGill              , BPIndex::ep1_VolOptPrison__ep2_IllGill__ep4_ShambertinPhase2      },
        {NewBPIndex::Deldepth             , BPIndex::ep1_PofuillySlime__ep2_Deldepth__ep4_None                },
        {NewBPIndex::Mericarol            , BPIndex::ep1_None__ep2_Mericarol__ep4_None                        },
        {NewBPIndex::UlGibbon             , BPIndex::ep1_None__ep2_UlGibbon__ep4_None                         },
        {NewBPIndex::ZolGibbon            , BPIndex::ep1_None__ep2_ZolGibbon__ep4_None                        },
        {NewBPIndex::Gibbles              , BPIndex::ep1_None__ep2_Gibbles__ep4_None                          },
        {NewBPIndex::Morfos               , BPIndex::ep1_None__ep2_Morfos__ep4_None                           },
        {NewBPIndex::Recobox              , BPIndex::ep1_None__ep2_Recobox__ep4_None                          },
        {NewBPIndex::Recon                , BPIndex::ep1_None__ep2_Recon__ep4_None                            },
        {NewBPIndex::SinowZoa             , BPIndex::ep1_None__ep2_SinowZoa__ep4_None                         },
        {NewBPIndex::SinowZele            , BPIndex::ep1_None__ep2_SinowZele__ep4_None                        },
        {NewBPIndex::Merikle              , BPIndex::ep1_None__ep2_Merikle__ep4_None                          },
        {NewBPIndex::Mericus              , BPIndex::ep1_None__ep2_Mericus__ep4_None                          },
        {NewBPIndex::Merillia             , BPIndex::ep1_Booma__ep2_Merillia__ep4_None                        },
        {NewBPIndex::Meriltas             , BPIndex::ep1_Gobooma__ep2_Meriltas__ep4_None                      },
        {NewBPIndex::Dolmolm              , BPIndex::ep1_EvilShark__ep2_Dolmolm__ep4_None                     },
        {NewBPIndex::Dolmdarl             , BPIndex::ep1_PalShark__ep2_Dolmdarl__ep4_None                     },

        {NewBPIndex::Boota                , BPIndex::ep1_Mothmant__ep2_Mothmant__ep4_Boota                    },
        {NewBPIndex::ZeBoota              , BPIndex::ep1_Monest__ep2_Monest__ep4_ZeBoota                      },
        {NewBPIndex::BaBoota              , BPIndex::ep1_BarbarousWolf__ep2_BarbarousWolf__ep4_BaBoota        },
        {NewBPIndex::ZuCrater             , BPIndex::ep1_Canadine__ep2_Gee__ep4_ZuCrater                      },
        {NewBPIndex::PazuzuCrater         , BPIndex::ep1_CanadineRing__ep2_None__ep4_PazuzuCrater             },
        {NewBPIndex::ZuDesert             , BPIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert                   },
        {NewBPIndex::PazuzuDesert         , BPIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert             },
        {NewBPIndex::Astark               , BPIndex::ep1_Canane__ep2_None__ep4_Astark                         },
        {NewBPIndex::SatelliteLizardCrater, BPIndex::ep1_ChaosBringer__ep2_Delbiter__ep4_SatelliteLizardCrater},
        {NewBPIndex::YowieCrater          , BPIndex::ep1_DarkBelra__ep2_DarkBelra__ep4_YowieCrater            },
        {NewBPIndex::SatelliteLizardDesert, BPIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert        },
        {NewBPIndex::YowieDesert          , BPIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert          },
        {NewBPIndex::Dorphon              , BPIndex::ep1_DeRolLe__ep2_BarbaRay__ep4_Dorphon                   },
        {NewBPIndex::DorphonEclair        , BPIndex::ep1_DeRolLeShell__ep2_PigRay__ep4_DorphonEclair          },
        {NewBPIndex::Goran                , BPIndex::ep1_DeRolLeMine__ep2_UlRay__ep4_Goran                    },
        {NewBPIndex::PyroGoran            , BPIndex::ep1_Dragon__ep2_GolDragon__ep4_PyroGoran                 },
        {NewBPIndex::GoranDetonator       , BPIndex::ep1_SinowGold__ep2_SinowSpigell__ep4_GoranDetonator      },
        {NewBPIndex::MerissaA             , BPIndex::ep1_AlRappy__ep2_LoveRappy__ep4_MerissaA                 },
        {NewBPIndex::MerissaAA            , BPIndex::ep1_NanoDragon__ep2_GiGue__ep4_MerissaAA                 },
        {NewBPIndex::Girtablulu           , BPIndex::ep1_Bulclaw__ep2_None__ep4_Girtablulu                    },
    };

    /// Add enemies to the lists that the game uses to find the enemy constructor for each map
    void PatchMapEnemyLists()
    {
        // As a test, allow certain enemies to spawn in the falz shrine area
        auto falzEnemies = Enemy::ReadEntriesIntoEnemyConstructorList(Enemy::mapEnemyTable[(size_t) MapType::Boss_Darkfalz]);

        NpcType newEnemies[] = {
            NpcType::Booma,
            NpcType::Hildebear,
            NpcType::Rag_Rappy_Sand_Rappy,
            NpcType::Poison_Lily_Del_Lily,
            NpcType::Gillchic,
            NpcType::Evil_Shark,
            NpcType::Nano_Dragon,
            NpcType::Garanz,
            NpcType::Canadine,
            NpcType::Sinow_Beat,
            NpcType::Delsaber,
            NpcType::Chaos_Bringer,
            NpcType::Chaos_Sorcerer,
            NpcType::Dark_Gunner,
            NpcType::Darth_Belra,
            NpcType::Dimenian,
            NpcType::Delbiter
        };

        for (NpcType newType : newEnemies)
        {
            Enemy::TaggedEnemyConstructor* enemy = Enemy::FindEnemyConstructor(newType);

            if (enemy != nullptr)
            {
                falzEnemies.push_back(*enemy);
            }
        }

        falzEnemies.push_back(Enemy::enemyListTerminator);
        Enemy::mapEnemyTable[(size_t) Map::MapType::Boss_Darkfalz] = Enemy::CopyEnemyConstructorListToHeap(falzEnemies);
    }

    /// Add enemy global init functions to each map's initlist
    void PatchMapInitLists()
    {
        // As a test, append all previous map init function lists to the falz area init list
        auto falzFuncs = Map::ReadEntriesIntoMapInitList(Map::mapInitFunctionTable[(size_t) MapType::Boss_Darkfalz]);

        for (size_t i = 0; i < (size_t) MapType::Boss_Darkfalz; i++) {
            auto list = Map::ReadEntriesIntoMapInitList(Map::mapInitFunctionTable[i]);
            falzFuncs.insert(falzFuncs.end(), list.begin(), list.end());
        }

        falzFuncs.push_back(Map::mapInitListTerminator);
        Map::mapInitFunctionTable[(size_t) MapType::Boss_Darkfalz] = Map::CopyMapInitFunctionListToHeap(falzFuncs);
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
    BPIndex DecodeNewBpIndex(NewBPIndex i)
    {
        if (GetCurrentEpisode() == Episode::Episode2)
        {
            // Map Crater to Desert in Episode 2 (Desert is only requested when actually in Desert)
            switch (i)
            {
                // Rappies are excluded from Omnispawn
                case NewBPIndex::ZuCrater:
                    return BPIndex::ep1_Dubchic__ep2_Dubchic__ep4_ZuDesert;
                case NewBPIndex::PazuzuCrater:
                    return BPIndex::ep1_Gillchic__ep2_Gillchic__ep4_PazuzuDesert;
                case NewBPIndex::SatelliteLizardCrater:
                    return BPIndex::ep1_Garanz__ep2_Garanz__ep4_SatelliteLizardDesert;
                case NewBPIndex::YowieCrater:
                    return BPIndex::ep1_DarkGunner__ep2_GalGryphon__ep4_YowieDesert;
            }
        }

        auto entry = newBpIndexMap.find(i);
        if (entry == newBpIndexMap.end())
        {
            throw std::invalid_argument("NewBPIndex not found in index map");
        }

        return (*entry).second;
    }

    /// Get a vanilla BP entry
    void* GetOldBPEntry(BPIndex i, BPEntryType entryType)
    {
        return BattleParam::GetBPEntry(GetCurrentEpisode(), IsSoloMode(), i, entryType);
    }

    /// Get a new BP entry
    void* GetNewBPEntry(NewBPIndex i, BPEntryType entryType)
    {
        Episode fromEp = GetNewBPIndexEpisode(i);
        BPIndex decoded = DecodeNewBpIndex(i);

        return BattleParam::GetBPEntry(fromEp, IsSoloMode(), decoded, entryType);
    }

    /// Get a new or old BP entry
    void* GetOmnispawnBPEntry(uint8_t i, BPEntryType entryType)
    {
        if (i <= (uint8_t) BPIndex::END_INDEX)
        {
            // Vanilla index
            return GetOldBPEntry(static_cast<BPIndex>(i), entryType);
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

        patchApplied = true;
    }
};
