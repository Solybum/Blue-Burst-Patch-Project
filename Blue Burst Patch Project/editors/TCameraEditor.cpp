#ifdef PATCH_EDITORS

#include <stdlib.h>

#include "editors.h"
#include "helpers.h"
#include "patching.h"
#include "psobb_functions.h"

static byte *TCameraEditor_instance = NULL;

uint8_t *Create_TCameraEditor()
{
    byte *(__cdecl * pf_Create)() = (byte * (__cdecl *)())0x4d56d8;

#if 0
    // source points + type
    *(byte **)0xa48a6c = (byte *)(*pf_malloc)(0x80 * 0x10);
    // target points + type
    *(byte **)0xa48a70 = (byte *)(*pf_malloc)(0x80 * 0x10);
    // camera source + target points + type
    *(byte **)0xa48a74 = (byte *)(*pf_malloc)(0x80 * 0x1C);
#else
    // Not sure why the allocations above aren't sufficient.
    *(byte **)0xa48a6c = (byte *)(*pf_malloc)(100000);
    *(byte **)0xa48a70 = (byte *)(*pf_malloc)(100000);
    *(byte **)0xa48a74 = (byte *)(*pf_malloc)(100000);
#endif

    (*pf_Create)();

    TCameraEditor_instance = GetLastEditor();

    return TCameraEditor_instance;
}

static void __stdcall FreeBuffersEx() 
{
    (*pf_free)(*(void **)0xa48a6c);
    (*pf_free)(*(void **)0xa48a70);
    (*pf_free)(*(void **)0xa48a74);
}

static void __thiscall FreeBuffers(void* ptr)
{
    // Original code
    reinterpret_cast<void (__thiscall *)(void* ptr)>(0x004f7078)(ptr);

    FreeBuffersEx();
}

static void __naked FixSharedInputState()
{
    XASM(push eax);
    XASM(push ebx);
    XASM(push edx);
    XASM(mov ebx, dword ptr[ebp - 0x14]);
    XASM(mov eax, dword ptr[ebx + 0x30]);
    XASM(lea edx, [eax + eax * 0x4]);
    XASM(add edx, edx);
    XASM(add edx, edx);
    XASM(add edx, edx);
    XASM(lea ecx, [0xaae75c + edx]);
    XASM(pop edx);
    XASM(pop ebx);
    XASM(pop eax);
    XASM(ret);
}

// TODO: Inputs seem broken. 
void ApplyTCameraEditorPatches()
{
    uint32_t sharedInputArgs[] = {
        0x4d4a25,
        0x4d4a39,
        0x4d4a59,
        0x4d4a6d,
        0x4d4a8d,
        0x4d4aa1,
        0x4d4acf,
        0x4d4ae3,
        0x4d4af7,
        0x4d4b32,
        0x4d4b46,
        0x4d4b5a,
        0x4d4b93,
        0x4d4ba7,
        0x4d4bca,
        0x4d4bef,
        0x4d4f5a,
        0x4d4f6e,
        0x4d4fa8,
        0x4d4fbc,
        0x4d4ff6,
        0x4d500a,
        0x4d5022,
        0x4d5036,
    };

    for (auto addr : sharedInputArgs)
        PatchCALL(addr, addr + 5, (int)&FixSharedInputState);

    uint32_t njPrintAtNowCalls[] = {
        // func 4d5340
        0x4d5373, 
        0x4d53a8,
        0x4d53da,
        0x4d540c,
        0x4d543e,
        0x4d5470,
        0x4d54a2,
        0x4d54c8,
    };

    PatchNJPrintAtNowCalls(njPrintAtNowCalls, _countof(njPrintAtNowCalls));

    PatchCALL(0x004d4926, 0x004d492b, (int)&FreeBuffers);
}

#endif // PATCH_EDITORS
