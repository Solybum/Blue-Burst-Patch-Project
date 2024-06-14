#ifdef PATCH_NEWMAP

#include <algorithm>
#include <array>
#include <cstdint>
#include <unordered_map>

#include "../helpers.h"
#include "particle_effects.h"

#define COMMA ,

#define ParticleEntryFields(X) \
    X(std::array<char COMMA 16>, name) \
    X(uint32_t, motionType) \
    X(uint32_t, textureId) \
    X(float, spawnAreaSizeX) \
    X(float, spawnAreaSizeY) \
    X(float, spawnAreaSizeZ) \
    X(float, size) \
    X(float, sizeVariation) \
    X(float, sizeGrowth) \
    X(float, vacuumSpeed) \
    X(float, gravity) \
    X(float, horizSpeed) \
    X(float, gravityVariation) \
    X(float, creat) \
    X(float, particleAmount) \
    X(uint32_t, durationFrames) \
    X(uint32_t, durationVariation) \
    X(uint32_t, horizAccel) \
    X(float, friction) \
    X(float, fadeInDurationRatio) \
    X(float, fadeOutDurationRatio) \
    X(float, rotateSpeed) \
    X(float, opt1) \
    X(float, opt2) \
    X(float, opt3) \
    X(float, opt4) \
    X(float, opt5) \
    X(float, colorA) \
    X(float, colorR) \
    X(float, colorG) \
    X(float, colorB) \
    X(uint32_t, textureFlags) \
    X(uint32_t, nor) \
    X(uint32_t, jum) \
    X(uint32_t, rever)

#define AS_FIELD_DECL(tp, name) tp name;

#pragma pack(push, 1)
struct ParticleEntry
{
    ParticleEntryFields(AS_FIELD_DECL)
};
#pragma pack(pop)

#undef AS_FIELD_DECL

static auto mapParticleEntries = reinterpret_cast<ParticleEntry**>(0x00a72ccc);
static std::unordered_map<uint8_t, ParticleEntry*> replacedParticleEffects;

std::vector<ParticleEntry> ReadParticleFile(const std::string& particleFilePath)
{
    std::vector<ParticleEntry> entries;
    auto lines = ReadCsvFile(particleFilePath);
    
#define AS_TYPE_ONLY(tp, name) ,tp

    for (const auto& line : lines)
    {
        auto particleEntry = ParseCsvLine<ParticleEntry ParticleEntryFields(AS_TYPE_ONLY)>(line);
        entries.push_back(particleEntry);
    }

#undef AS_TYPE_ONLY

    return entries;
}

#undef COMMA
#undef ParticleEntryFields

void ReplaceMapParticleEffects(uint8_t origMap, const std::string& particleFilePath)
{
    replacedParticleEffects[origMap] = *mapParticleEntries;
    auto entries = ReadParticleFile(particleFilePath);
    auto entriesBuf = *mapParticleEntries = new ParticleEntry[entries.size()];
    std::copy(entries.begin(), entries.end(), entriesBuf);
}

void RestoreMapParticleEffects(uint8_t origMap)
{
    if (*mapParticleEntries)
        delete *mapParticleEntries;
    
    *mapParticleEntries = replacedParticleEffects[origMap];
    replacedParticleEffects.erase(origMap);
}

#endif
