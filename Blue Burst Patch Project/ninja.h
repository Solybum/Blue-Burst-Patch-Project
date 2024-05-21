#pragma once

#include <cstdint>
#include "object_extension.h"

#pragma pack(push, 1)
struct BmlMetadata
{
    const char* bmlFilename;
    const char** modelFilenames;
    const char** animationFilenames;
    const uint16_t modelCount;
    const uint16_t animationCount;
};

struct TBinModel
{
    union
    {
        DEFINE_FIELD(0x42c, void* njcmChunks);
        DEFINE_FIELD(0x430, void* njmChunks);
        DEFINE_FIELD(0x434, uint32_t njcmCount);
        DEFINE_FIELD(0x438, uint32_t njmCount);

        uint8_t _padding[0x440];
    };
};

struct NjmMotion
{
    void* nodesKeyframes;
    uint32_t frameCount;
    uint16_t motionFlags;
    uint16_t factorCount;
};
#pragma pack(pop)

extern void (__thiscall *UseTextureArchive)(void* tex);
extern void (__fastcall *RenderXj)(void* model);
extern void (__fastcall *RenderAnimatedXjUp)(void* model, void* motion, float elapsed);
extern TBinModel* (__thiscall *LoadBml)(TBinModel* result, uint32_t unk1, BmlMetadata* bmlMeta, void* unk2, uint32_t unk3, void* optionalDstBuf);
extern void (__thiscall *FreeBml)(TBinModel* bml, uint32_t doFree);
extern void* (__cdecl *LoadXvm)(const char* filename);
extern uint32_t (__fastcall *FreeXvm)(void* xvm);
extern void (__thiscall *PrepareBmlMeshes)(TBinModel* bml);
extern void* (__thiscall *GetModel)(TBinModel* bml, uint32_t index);
extern NjmMotion* (__thiscall *GetMotion)(TBinModel* bml, uint32_t index);
extern void (*DisableFog)();
extern void (*EnableFog)();
