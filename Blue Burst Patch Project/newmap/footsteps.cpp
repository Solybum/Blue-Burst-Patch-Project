#ifdef PATCH_NEWMAP

#include <cstdint>
#include <string>
#include <unordered_map>

#include "footsteps.h"
#include "helpers.h"

#pragma pack(push, 1)
struct ClassFootsteps
{
    uint32_t normal;
    uint32_t grass;
    uint32_t water;
    uint32_t deepWater;
    uint32_t footprints;
};

struct MapFootsteps
{
    ClassFootsteps humans;
    ClassFootsteps androids;
    ClassFootsteps racast;
};

struct AllFootsteps
{
    ClassFootsteps* humans;
    ClassFootsteps* androids;
    ClassFootsteps* racast;
};
#pragma pack(pop)

static auto allFootstepSfx = reinterpret_cast<AllFootsteps*>(0x009cc480);
static std::unordered_map<uint8_t, MapFootsteps> replacedFootstepSfx;

MapFootsteps ReadFootstepFile(const std::string& footstepSfxFilePath)
{
    MapFootsteps footsteps;
    
    auto lines = ReadCsvFile(footstepSfxFilePath);
    footsteps.humans = ParseCsvLine<ClassFootsteps, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t>(lines[0]);
    footsteps.androids = ParseCsvLine<ClassFootsteps, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t>(lines[1]);
    footsteps.racast = ParseCsvLine<ClassFootsteps, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t>(lines[2]);
    
    return footsteps;
}

void ReplaceMapFootsteps(uint8_t origMap, const std::string& footstepSfxFilePath)
{
    auto footsteps = ReadFootstepFile(footstepSfxFilePath);
    replacedFootstepSfx[origMap] =
    {
        .humans = allFootstepSfx->humans[origMap],
        .androids = allFootstepSfx->androids[origMap],
        .racast = allFootstepSfx->racast[origMap]
    };
    allFootstepSfx->humans[origMap] = footsteps.humans;
    allFootstepSfx->androids[origMap] = footsteps.androids;
    allFootstepSfx->racast[origMap] = footsteps.racast;
}

void RestoreMapFootsteps(uint8_t origMap)
{
    auto origFootsteps = replacedFootstepSfx[origMap];
    allFootstepSfx->humans[origMap] = origFootsteps.humans;
    allFootstepSfx->androids[origMap] = origFootsteps.androids;
    allFootstepSfx->racast[origMap] = origFootsteps.racast;
    replacedFootstepSfx.erase(origMap);
}

#endif
