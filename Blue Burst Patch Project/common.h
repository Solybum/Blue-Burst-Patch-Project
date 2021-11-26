#pragma once

#include "map.h"

enum class Episode
{
    Episode1 = 0,
    Episode2 = 1,
    Episode4 = 2,
};

Episode GetCurrentEpisode();
bool IsSoloMode();
Map::MapType GetCurrentMap();
