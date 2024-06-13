#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct MapLoader
{
    const char* name;
    bool (__cdecl *Load)();
    void (__cdecl *Unload)();
};
#pragma pack(pop)

bool __cdecl DefaultMapLoad();
void __cdecl DefaultMapUnload();

void PatchMapLoaders();
void ReplaceMapLoader(uint8_t origMap, const MapLoader& loader);
void RestoreMapLoader(uint8_t origMap);
