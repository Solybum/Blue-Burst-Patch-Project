#pragma once

#include <cstdint>
#include "initlist.h"

namespace Map
{
    enum class MapType : uint32_t {
        Pioneer2_Ep1 = 0,
        Forest1 = 1,
        Forest2 = 2,
        Cave1 = 3,
        Cave2 = 4,
        Cave3 = 5,
        Mines1 = 6,
        Mines2 = 7,
        Ruins1 = 8,
        Ruins2 = 9,
        Ruins3 = 10,
        Boss_Dragon = 11,
        Boss_Derolle = 12,
        Boss_Volopt = 13,
        Boss_Darkfalz = 14,
        Lobby = 15,
        Battle_Spaceship = 16,
        Battle_Ruins = 17,
        Pioneer2_Ep2 = 18,
        Temple_A = 19,
        Temple_B = 20,
        Spaceship_A = 21,
        Spaceship_B = 22,
        CCA = 23,
        Jungle_East = 24,
        Jungle_North = 25,
        Mountain = 26,
        Seaside = 27,
        Seabed_Upper = 28,
        Seabed_Lower = 29,
        Boss_Galgryphon = 30,
        Boss_Olgaflow = 31,
        Boss_Barbaray = 32,
        Boss_Goldragon = 33,
        Seaside_Night = 34,
        Tower = 35,
        Wilds1 = 36,
        Wilds2 = 37,
        Wilds3 = 38,
        Wilds4 = 39,
        Crater = 40,
        Desert1 = 41,
        Desert2 = 42,
        Desert3 = 43,
        Boss_Saintmilion = 44,
        Pioneer2_Ep4 = 45,
        Test_Area = 46,

        MAX_INDEX = Test_Area,

        Invalid  =  0xffffffff
    };

    InitList& GetMapInitList(MapType);
};
