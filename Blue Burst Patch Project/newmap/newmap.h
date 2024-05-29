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
        const char* variantNames[];
    }* prefixes;
    uint32_t variantCount;
};

struct SetDataTable
{
    struct Inner1 {
        struct Inner2 {
            const char* variantName1;
            const char* variantName2;
            const char* baseName;
        }* objectSetVariants;
        uint32_t count;
    }* mapVariants;
    uint32_t count;
};

extern SetDataTable** setDataTable;

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
