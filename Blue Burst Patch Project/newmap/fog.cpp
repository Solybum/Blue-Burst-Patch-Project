#ifdef PATCH_NEWMAP

#include <cstdint>
#include <unordered_map>

#include "../helpers.h"
#include "fog.h"

#define FogEntryFields(X) \
    X(uint32_t, type) \
    X(uint8_t, color_b) \
    X(uint8_t, color_g) \
    X(uint8_t, color_r) \
    X(uint8_t, color_a) \
    X(float, end) \
    X(float, start) \
    X(float, density) \
    X(uint32_t, unk1) \
    X(float, unk2) \
    X(float, unk3) \
    X(float, end_pulse_distance) \
    X(uint32_t, unk4) \
    X(float, start_pulse_distance) \
    X(uint32_t, unk5) \
    X(float, transition) \
    X(uint32_t, unk6) \
    X(uint8_t, unk7) \
    X(uint8_t, unk8) \
    X(uint8_t, lerped_field6) \
    X(uint8_t, unk9) \
    X(uint8_t, lerped_field7) \
    X(uint8_t, unk10) \
    X(uint8_t, lerped_field8) \
    X(uint8_t, unk11)

#define AS_FIELD_DECL(tp, name) tp name;

#pragma pack(push, 1)
struct FogEntry
{
    FogEntryFields(AS_FIELD_DECL)
};
#pragma pack(pop)

#undef AS_FIELD_DECL

static FogEntry** fogEntries = reinterpret_cast<FogEntry**>(0x00a8d770);
static std::unordered_map<uint8_t, FogEntry> replacedFogEntries;

FogEntry ReadFogFile(const std::string& path)
{
    // Read fog settings from file
    auto lines = ReadCsvFile(path);
    auto& splitParts = lines[0];
    
#define AS_TYPE_ONLY(tp, name) ,tp
    auto fog = ParseCsvLine<FogEntry FogEntryFields(AS_TYPE_ONLY)>(splitParts);
#undef AS_TYPE_ONLY
    
    return fog;
}

#undef FogEntryFields

void ReplaceMapFog(uint8_t origMap, const std::string &fogFilePath)
{
    replacedFogEntries[origMap] = (*fogEntries)[origMap];
    (*fogEntries)[origMap] = ReadFogFile(fogFilePath);
}

void RestoreMapFog(uint8_t origMap)
{
    (*fogEntries)[origMap] = replacedFogEntries[origMap];
    replacedFogEntries.erase(origMap);
}

#endif // PATCH_NEWMAP
