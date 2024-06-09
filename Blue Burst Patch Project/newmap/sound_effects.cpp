#include "common.h"
#ifdef PATCH_NEWMAP

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "helpers.h"
#include "sound_effects.h"

#pragma pack(push, 1)
struct SoundEffectMetadata
{
    uint8_t unused1 = 0;
    uint8_t unused2 = 0;
    uint8_t loopFlags = 0;
    uint8_t unused3 = 0;
    uint32_t soundFileOffset = 0;
    uint32_t loopStart = 0;
    uint32_t loopEnd = 0;
};

// WAVEFORMATEX from dsound8
struct WAVEFORMATEX
{
    uint16_t wFormatTag;
    uint16_t nChannels;
    uint32_t nSamplesPerSec;
    uint32_t nAvgBytesPerSec;
    uint16_t nBlockAlign;
    uint16_t wBitsPerSample;
    // uint16_t cbSize; // Field ignored by WAVE_FORMAT_PCM
};

struct PacItemHeader
{
    WAVEFORMATEX waveFormatEx;
    uint8_t magic[4];
    uint32_t size1;
    uint16_t unk6;
    uint16_t unk7;
    uint32_t size2; // next item at aligned to 0x20
};
#pragma pack(pop)

struct ReplacedPac
{
    char* pacFilename;
    uint32_t soundCount;
    SoundEffectMetadata* metadata;
    uint32_t* soundIds;
    
    ReplacedPac() {}
    ReplacedPac(
        char* pacFilename,
        uint32_t soundCount,
        SoundEffectMetadata* metadata,
        uint32_t* soundIds)
            : pacFilename(pacFilename),
              soundCount(soundCount),
              metadata(metadata),
              soundIds(soundIds) {}
};

static const auto vanillaSoundEffectLimit = 0x700;
static const auto newSoundEffectLimit = 0x1000;

static std::unordered_map<uint8_t, ReplacedPac> replacedPacs;
static uint32_t* pacIndexMap = reinterpret_cast<uint32_t*>(0x00989e60);
static char** pacFilenames = reinterpret_cast<char**>(0x00a21560);
static uint32_t* pacFileSoundCount = reinterpret_cast<uint32_t*>(0x00a214e0);
static SoundEffectMetadata** pacFileMetadata = reinterpret_cast<SoundEffectMetadata**>(0x00a215e0);
static uint32_t** pacFileSoundIds = reinterpret_cast<uint32_t**>(0x00a21460);

static auto newSfxFlagArray = std::vector<uint32_t>();

static void PatchSfxFlagArray()
{
    // Replace references to array with new array
    for (auto addr : {0x008142be + 2, 0x0081442e + 3, 0x008144d6 + 2, 0x00814656 + 3, 0x008290fb + 3, 0x00829103 + 1})
        *(uint32_t**) addr = newSfxFlagArray.data();
}

static void ReplaceMapPac(uint8_t origMap,
    const std::string& pacFilename,
    uint32_t soundCount,
    const std::vector<SoundEffectMetadata>& metadata,
    const std::vector<uint32_t>& soundIds,
    const std::vector<uint32_t>& soundFlags)
{
    // Save original data
    auto pacIndex = pacIndexMap[origMap];
    replacedPacs.try_emplace(
        origMap,
        pacFilenames[pacIndex],
        pacFileSoundCount[pacIndex],
        pacFileMetadata[pacIndex],
        pacFileSoundIds[pacIndex]);

    // Write new data
    auto filenameBuf = pacFilenames[pacIndex] = new char[pacFilename.size() + 1];
    std::copy(pacFilename.begin(), pacFilename.end(), filenameBuf);
    filenameBuf[pacFilename.size()] = '\0';

    pacFileSoundCount[pacIndex] = soundCount;

    auto metadataBuf = pacFileMetadata[pacIndex] = new SoundEffectMetadata[metadata.size()];
    std::copy(metadata.begin(), metadata.end(), metadataBuf);

    auto idBuf = pacFileSoundIds[pacIndex] = new uint32_t[soundIds.size()];
    std::copy(soundIds.begin(), soundIds.end(), idBuf);
    
    // Add padding to the flags array to ensure it's indexable by sfx id
    size_t lowestId = 0xffffffff;
    for (auto id : soundIds)
    {
        if (id < vanillaSoundEffectLimit)
            throw std::runtime_error("Sound effect id must be above vanilla limit of " + std::to_string(vanillaSoundEffectLimit));
        if (id < lowestId)
            lowestId = id;
    }

    auto idDiff = lowestId - vanillaSoundEffectLimit;
    for (size_t i = 0; i < idDiff; i++)
        newSfxFlagArray.push_back(0);
    
    // Append flags and repatch array
    newSfxFlagArray.insert(newSfxFlagArray.end(), soundFlags.begin(), soundFlags.end());
    PatchSfxFlagArray();
}

void ReplaceMapPac(uint8_t origMap, const std::string& pacFilename, const std::string& configFilename)
{
    const std::string soundDirPath = "data/sound/";

    auto parseInt = [](const std::string& s) {
        auto radix = s.size() > 1 && s[1] == 'x'
            ? 16 : 10;
        return std::strtoul(s.c_str(), nullptr, radix);
    };

    auto soundCount = 0;
    auto metadata = std::vector<SoundEffectMetadata>();
    auto soundIds = std::vector<uint32_t>();
    auto soundFlags = std::vector<uint32_t>();
    for (const auto& line : ReadCsvFile(soundDirPath + configFilename))
    {
        soundCount++;

        auto id = parseInt(line[0]);
        auto loopFlags = (uint8_t) parseInt(line[1]);
        auto loopStart = parseInt(line[2]);
        auto loopEnd = parseInt(line[3]);
        auto flags = parseInt(line[4]);
    
        soundIds.push_back(id);
        metadata.push_back({
            .loopFlags = loopFlags,
            .loopStart = loopStart,
            .loopEnd = loopEnd
        });
        soundFlags.push_back(flags);
    }
    
    // Vanilla hardcodes the offset of each wav,
    // but we can be a little less stupid and actually read the offsets from the pac
    std::ifstream file;
    file.open(soundDirPath + pacFilename, std::ios::binary);

    if (file.fail())
        throw std::runtime_error("Failed to open '" + pacFilename + "'");

    std::vector<uint8_t> pacFileContents;
    std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), std::back_inserter(pacFileContents));
    file.close();
    
    // Get the offset of each wav inside the pac
    const size_t pacItemAlign = 0x20;
    size_t pacCursor = 0;
    for (size_t i = 0; i < soundCount; i++)
    {
        metadata[i].soundFileOffset = pacCursor;
        Log(ToWideString(std::to_string(pacCursor)).c_str());
        
        // Read header and skip to next header
        auto itemHeader = reinterpret_cast<PacItemHeader*>(pacFileContents.data() + pacCursor);
        pacCursor += sizeof(PacItemHeader);
        if (itemHeader->magic[0] == 'J' && itemHeader->magic[1] == 'U')
            pacCursor += 2;
        pacCursor += itemHeader->size2;
        pacCursor = NextAligned(pacCursor, pacItemAlign);
    }
    
    ReplaceMapPac(origMap, pacFilename, soundCount, metadata, soundIds, soundFlags);
}

void RestoreMapPac(uint8_t origMap)
{
    auto pacIndex = pacIndexMap[origMap];
    
    // Remove all added flags
    newSfxFlagArray.resize(vanillaSoundEffectLimit);
    PatchSfxFlagArray();

    // Free new data
    if (pacFilenames[pacIndex])
    {
        delete pacFilenames[pacIndex];
        pacFilenames[pacIndex] = nullptr;
    }
    if (pacFileMetadata[pacIndex])
    {
        delete pacFileMetadata[pacIndex];
        pacFileMetadata[pacIndex] = nullptr;
    }
    if (pacFileSoundIds[pacIndex])
    {
        delete pacFileSoundIds[pacIndex];
        pacFileSoundIds[pacIndex] = nullptr;
    }
    pacFileSoundCount[pacIndex] = 0;
    
    if (replacedPacs.count(origMap))
    {
        // Restore original data
        auto& origData = replacedPacs[origMap];
        pacFilenames[pacIndex] = origData.pacFilename;
        pacFileSoundCount[pacIndex] = origData.soundCount;
        pacFileMetadata[pacIndex] = origData.metadata;
        pacFileSoundIds[pacIndex] = origData.soundIds;
        
        // Delete save
        replacedPacs.erase(origMap);
    }
}

__attribute__((regparm(1)))
static uint32_t __cdecl BeforeSoundEffectIdSubstitutions(uint32_t id)
{
    // Don't call original function if id is higher than vanilla limit
    if (id >= vanillaSoundEffectLimit) return id;

    // Call original function
    return reinterpret_cast<decltype(BeforeSoundEffectIdSubstitutions)*>(0x0081443c)(id);
}

void PatchSoundEffectLimit()
{
    // Increase buffer size
    *(uint32_t*) (0x00829b88 + 1) = newSoundEffectLimit * 0x20;
    *(uint32_t*) (0x00829b92 + 1) = newSoundEffectLimit * 0x20;

    // Increase limits in conditionals
    *(uint32_t*) (0x008142b3 + 1) = newSoundEffectLimit;
    *(uint32_t*) (0x008144cb + 1) = newSoundEffectLimit;
    *(uint32_t*) (0x008290f4 + 1) = newSoundEffectLimit;

    // Copy original flag array
    auto origSfxFlagArray = (uint32_t*) 0x00a13b20;
    std::copy(origSfxFlagArray, origSfxFlagArray + vanillaSoundEffectLimit, std::back_inserter(newSfxFlagArray));

    PatchCALL(0x00814427, 0x00814427 + 5, (int) BeforeSoundEffectIdSubstitutions);
    PatchCALL(0x0081464f, 0x0081464f + 5, (int) BeforeSoundEffectIdSubstitutions);
}

#endif
