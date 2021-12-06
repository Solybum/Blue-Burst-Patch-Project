#pragma once

#include "map.h"

enum class Episode
{
    Episode1 = 0,
    Episode2 = 1,
    Episode4 = 2,
};

enum class Difficulty
{
    Normal = 0,
    Hard = 1,
    VeryHard = 2,
    Ultimate = 3
};

Episode GetCurrentEpisode();
bool IsSoloMode();
Map::MapType GetCurrentMap();
Difficulty GetCurrentDifficulty();
