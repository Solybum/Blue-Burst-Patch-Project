#pragma once

#include <cstdint>
#include <string>

#pragma pack(push, 1)
struct MapLoader
{
    const char* name;
    bool (__cdecl *Load)();
    void (__cdecl *Unload)();
};

struct MapAssetPrefixes
{
    struct Prefixes {
        const char* basename;
        const char* variant_names[];
    }* prefixes;
    uint32_t variant_count;
};

struct CustomMapDefinition
{
    MapAssetPrefixes assetPrefixes;
    MapLoader mapLoader;
    std::string songFilename;
    std::string slbgmFilePath;
    size_t slbgmIndex; // Assigned by patch
};
#pragma pack(pop)

void ApplyNewMapPatch();
