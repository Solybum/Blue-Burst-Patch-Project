#include <cstdint>
#include <unordered_map>
#ifdef PATCH_NEWMAP

#include "../common.h"
#include "../helpers.h"
#include "sunlight.h"

#pragma pack(push, 1)
struct LightEntry
{
    float x1;
    float y1;
    float z1;
    float x2;
    float y2;
    float z2;
    float intensity_specular;
    float intensity_diffuse;
    float intensity_ambient;
    float diffuse_a;
    float diffuse_r;
    float diffuse_g;
    float diffuse_b;
    float ambient_a;
    float ambient_r;
    float ambient_g;
    float ambient_b;
};
#pragma pack(pop)

static LightEntry** lightEntries = reinterpret_cast<LightEntry**>(0x00a9d4e4);
static std::unordered_map<uint8_t, LightEntry> replacedLightEntries;

LightEntry ReadLightFile(const std::string& path)
{
    // Read sunlight settings from file
    auto lines = ReadCsvFile(path);
    auto& splitParts = lines[0];
    
    auto parseFloat = [&](size_t idx) {
        return std::strtof(splitParts[idx].c_str(), nullptr);
    };
    
    LightEntry light = {
        .x1 = parseFloat(0),
        .y1 = parseFloat(1),
        .z1 = parseFloat(2),
        .x2 = parseFloat(3),
        .y2 = parseFloat(4),
        .z2 = parseFloat(5),
        .intensity_specular = parseFloat(6),
        .intensity_diffuse = parseFloat(7),
        .intensity_ambient = parseFloat(8),
        .diffuse_a = parseFloat(9),
        .diffuse_r = parseFloat(10),
        .diffuse_g = parseFloat(11),
        .diffuse_b = parseFloat(12),
        .ambient_a = parseFloat(13),
        .ambient_r = parseFloat(14),
        .ambient_g = parseFloat(15),
        .ambient_b = parseFloat(16)
    };
    
    return light;
}

void ReplaceMapSunlight(uint8_t origMap, const std::string& sunlightFilePath)
{
    auto lightEntryIndex = origMap;
    if (IsUltEp1()) lightEntryIndex += 48;
    replacedLightEntries[origMap] = (*lightEntries)[lightEntryIndex];
    (*lightEntries)[lightEntryIndex] = ReadLightFile(sunlightFilePath);
}

void RestoreMapSunlight(uint8_t origMap)
{
    auto lightEntryIndex = origMap;
    if (IsUltEp1()) lightEntryIndex += 48;
    (*lightEntries)[lightEntryIndex] = replacedLightEntries[lightEntryIndex];
    replacedLightEntries.erase(lightEntryIndex);
}

#endif // PATCH_NEWMAP
