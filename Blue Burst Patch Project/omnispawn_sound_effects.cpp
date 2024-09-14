#include "helpers.h"
#include "omnispawn.h"
#include <mathutil.h>
#include "map.h"

// TODO: Cleanup this file.

namespace Omnispawn
{
    using Map::MapType;

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

    /// Returns 3 to fix the missing dark enemy spawn sound effect in CCA.
    uint32_t __cdecl HandleSfxDarkSpawn(void)
    {
        return 3;
    }

    /// Returns map number for Wilds1 to fix sound effects for Wilds/Crater enemies.
    int __cdecl GetWildsMapNumber()
    {
        return (int)MapType::Wilds1;
    }

    /// Returns map number for Sub Desert 1 to fix sound effects for Desert enemies.
    int __cdecl GetDesertMapNumber()
    {
        return (int)MapType::Desert1;
    }

    void PatchSoundEffectMapReferences()
    {
        PatchCALL(0x578a6f, 0x578a74, (int)&HandleSfxDarkSpawn);

        uint32_t addrsGetMapCrater[] =
        {
            0x5a592b, // Boota family - War cry
            0x5a5957, // Boota family - Arm attack
            0x5a56ff, // Boota family - Ze Boota rush attack
            0x5a5983, // Boota family - Flinched
            0x5a59af, // Boota family - Death sound

            0x5b1417, // Lizard family - Spawn sound 1
            0x5b144d, // Lizard family - Spawn sound 2
            0x5aff07, // Lizard family - Movement
            0x5b0104, // Lizard family - Attack step 1
            0x5aff74, // Lizard family - Attack step 2
            0x5aefbe, // Lizard family - Death sound
            0x5af2c1, // Lizard family - unknown
            0x5af701, // Lizard family - unknown
            0x5af783, // Lizard family - unknown

            0x5a3875, // Astark - Spawn landing
            0x5a2cb8, // Astark - Punch attack
            0x5a2884, // Astark - Breath/particle attack
            0x5a3276, // Astark - Moving
            0x5a254c, // Astark - Jump attack initiated
            0x5a23d3, // Astark - Jump attack landing
            0x5a2133, // Astark - Flinch
            0x5a1fa6, // Astark - Death sound 1
            0x5a2035, // Astark - Death sound 2
            0x5a2f5a, // Astark - Taunt?
            0x5a343b, // Astark - Another spawn landing?

            0x5b46d2, // Zu family - Wing flap
            0x5b2c25, // Zu family - Dive attack
            0x5b332d, // Zu family - Laser attack
            0x5b4132, // Zu family - Dead (start fall)
            0x5b40ad, // Zu family - Dead (hit ground)
            0x5b43ec, // Zu family - unknown

            0x5a9655, // Dorphon - Spawn
            0x5a953d, // Dorphon - Spawn 2
            0x5a8b1d, // Dorphon - Spawn 3
            0x5a7b39, // Dorphon - Laser
            0x5a7dd7, // Dorphon - Some roar
            0x5a7fdd, // Dorphon - Stomp/punch
            0x5a7fa9, // Dorphon - Right punch?
            0x5a8847, // Dorphon - Charge windup
            0x5a85e2, // Dorphon - Charge start
            0x5a865a, // Dorphon - Charge stop
            0x5a8fdd, // Dorphon - Turning sound
            0x5a745d, // Dorphon - Death
            0x5a7337, // Dorphon - Death 2
        };
        for (auto addr : addrsGetMapCrater)
        {
            PatchCALL(addr, addr + 5, (int)&GetWildsMapNumber);
        }

        // Goran family
        uint32_t adrrsGetMapDesert[] = {
            0x5ad62f, // Goran family - War cry 1
            0x5ad609, // Goran family - War cry 2
            0x5ad663, // Goran family - Attack
            0x5ad697, // Goran family - Hit by attack
            0x5ad6cb, // Goran family - Dead
            0x5ad5ce, // Goran family - Dead 2

            0x5b51b3, // Merissa family - Spawn
            0x5b562f, // Merissa family - Hop 1
            0x5b5666, // Merissa family - Hop 2
            0x5b6540, // Merissa family - Jump attack start
            0x5b6632, // Merissa family - Jump attack landing
            0x5b5f53, // Merissa family - Jump over player
            0x5b5f8a, // Merissa family - Land behind player
            0x5b6391, // Merissa family - Stab attack
            0x5b6465, // Merissa family - Faster stab attack
            0x5b5263, // Merissa family - Hit by attack
            0x5b53a5, // Merissa family - Dead 1
            0x5b53be, // Merissa family - Dead 2

            0x5aa719, // Girtablulu - Turn 1?
            0x5aa6cc, // Girtablulu - Melee 1
            0x5ab01b, // Girtablulu - Melee 2
            0x5aa4a5, // Girtablulu - Hit by attack
            0x5aa5d2, // Girtablulu - Center hit by attack
            0x5aa4d3, // Girtablulu - Unknown
            0x5aa3d8, // Girtablulu - Unknown
        };
        for (auto addr : adrrsGetMapDesert)
        {
            PatchCALL(addr, addr + 5, (int)&GetDesertMapNumber);
        }
    }


    void PatchOmnispawnSoundEffects()
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

        // And fix enemies that look up sound effect ID based on the current map.
        // Because all map sound effect files are loaded, this simply returns a map
        // number where they already exist.
        PatchSoundEffectMapReferences();
    }
}
