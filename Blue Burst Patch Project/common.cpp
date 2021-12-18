#include <stdexcept>
#include "common.h"

Episode GetCurrentEpisode()
{
    return *reinterpret_cast<Episode*>(0x00a9b1c8);
}

bool IsSoloMode()
{
    return *reinterpret_cast<bool*>(0x00a9b1c4);
}

Map::MapType GetCurrentMap()
{
    return *reinterpret_cast<Map::MapType*>(0x00aafc9c);
}

Difficulty GetCurrentDifficulty()
{
    return *reinterpret_cast<Difficulty*>(0x00a9b1cc);
}
