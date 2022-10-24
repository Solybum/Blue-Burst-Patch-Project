#ifdef PATCH_EDITORS

#include <stdlib.h>

#include "editors.h"
#include "helpers.h"
#include "patching.h"
#include "psobb_functions.h"

static byte *ASM_VAR(TParticleEditor_instance) = NULL;

byte *Create_TParticleEditor()
{
    byte *(__cdecl * pf_Create)() = (byte * (__cdecl *)())0x50d5dc;

    (*pf_Create)();

    TParticleEditor_instance = GetLastEditor();

    return TParticleEditor_instance;
}

static void __naked CreateDebugParticle()
{
    XASM(pushad);
    XASM(mov ecx, edi);
    XASM(mov eax, 0x50d530);
    XASM(call eax);
    XASM(popad);
    XASM(push 0x50d8b9);
    XASM(ret);
}

static void __naked FixSharedInputState()
{
    XASM(push eax);
    XASM(push edx);
    XASM(mov eax, dword ptr[edi + 0x30]);
    XASM(lea edx, [eax + eax * 0x4]);
    XASM(add edx, edx);
    XASM(add edx, edx);
    XASM(add edx, edx);
    XASM(lea ecx, [0xaae75c + edx]);
    XASM(pop edx);
    XASM(pop eax);
    XASM(ret);
}

void ApplyTParticleEditorPatches()
{
    PatchJMP(0x50d8b4, 0x50d8b9, (int)&CreateDebugParticle);

    uint32_t sharedInputArgs[] = {
        0x50cda6,
        0x50cdba,
        0x50cdf2,
        0x50ce1e,
        0x50ce50,
        0x50ced0,
        0x50ceff,
        0x50cf5e,
        0x50cf72,
        0x50cf9f,
        0x50cfb7,
        0x50d348,
        0x50d381,
        0x50d3e4,
        0x50d3f8,
        0x50d457,
    };

    for (auto addr : sharedInputArgs)
        PatchCALL(addr, addr + 5, (int)&FixSharedInputState);

    uint32_t njPrintAtCalls[] = {
        0x50cf32,
        0x50cf41,
        0x50cf50,
        0x50d008,
        0x50d012,
        0x50d026,
    };

    uint32_t renderMethods[] = {
        0xaff3c0,
    };

    PatchNJPrintAtCalls(njPrintAtCalls, _countof(njPrintAtCalls));
    ReplaceTEditorRenderMethods(renderMethods, _countof(renderMethods));
}

#endif // PATCH_EDITORS
