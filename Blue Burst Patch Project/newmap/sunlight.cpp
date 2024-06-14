#include <cstdint>
#include <unordered_map>
#ifdef PATCH_NEWMAP

#include "../common.h"
#include "../helpers.h"
#include "sunlight.h"

#define LightEntryFields(X) \
    X(float, x1) \
    X(float, y1) \
    X(float, z1) \
    X(float, x2) \
    X(float, y2) \
    X(float, z2) \
    X(float, intensity_specular) \
    X(float, intensity_diffuse) \
    X(float, intensity_ambient) \
    X(float, diffuse_a) \
    X(float, diffuse_r) \
    X(float, diffuse_g) \
    X(float, diffuse_b) \
    X(float, ambient_a) \
    X(float, ambient_r) \
    X(float, ambient_g) \
    X(float, ambient_b)

#define AS_FIELD_DECL(tp, name) tp name;

#pragma pack(push, 1)
struct LightEntry
{
    LightEntryFields(AS_FIELD_DECL)
};
#pragma pack(pop)

#undef AS_FIELD_DECL

static LightEntry** lightEntries = reinterpret_cast<LightEntry**>(0x00a9d4e4);
static std::unordered_map<uint8_t, LightEntry> replacedLightEntries;

LightEntry ReadLightFile(const std::string& path)
{
    // Read sunlight settings from file
    auto lines = ReadCsvFile(path);
    auto& splitParts = lines[0];

#define AS_TYPE_ONLY(tp, name) ,tp
    auto light = ParseCsvLine<LightEntry LightEntryFields(AS_TYPE_ONLY)>(splitParts);
#undef AS_TYPE_ONLY
    
    return light;
}

#undef LightEntryFields

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
