#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct BmlMetadata
{
    const char* bmlFilename;
    const char** modelFilenames;
    const char** animationFilenames;
    const uint16_t modelCount;
    const uint16_t animationCount;
};
#pragma pack(pop)

extern void (__thiscall *UseTextureArchive)(void* tex);
extern void (__fastcall *RenderXj)(void* model);
extern void* (__thiscall *LoadBml)(void* result, uint32_t unk1, BmlMetadata* bmlMeta, void* unk2, uint32_t unk3, void* optionalDstBuf);
extern void (__thiscall *FreeBml)(void* bml, uint32_t doFree);
extern void* (__cdecl *LoadXvm)(const char* filename);
extern uint32_t (__fastcall *FreeXvm)(void* xvm);
extern void (__thiscall *PrepareBmlMeshes)(void* bml);
extern void* (__thiscall *GetModel)(void* bml, uint32_t index);
