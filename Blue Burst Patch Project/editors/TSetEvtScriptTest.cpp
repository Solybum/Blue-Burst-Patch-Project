#ifdef PATCH_EDITORS

#include <stdlib.h>

#include "editors.h"
#include "helpers.h"
#include "patching.h"

byte *Create_TSetEvtScriptTest()
{
    byte *(__cdecl * pf_Create)(void) = (byte * (__cdecl *)())0x4f7a78;

    (*pf_Create)();

    return GetLastEditor();
}

void __naked TSetEvtScriptTestMenuListingColor()
{
    XASM(pushad);
    XASM(shr esi, 2);
    XASM(cmp dword ptr [ebp + 0x40], esi);
    XASM(jne notSelected);
    XASM(push dword ptr editorColorSelected);
    XASM(jmp getOut);

XASM(notSelected:);
    XASM(push dword ptr editorColorDefault);

XASM(getOut:);
    // Set the debug color
    XASM(call EditorColorSet);
    XASM(pop ecx);
    XASM(popad);

    // original code and get out
    XASM(push dword ptr ds : [esi + 0xA11E30] );
    XASM(push 0x80da57);
    XASM(ret);
}

void __naked TSetEvtScriptTestRestoreColor()
{
    XASM(call EditorColorReset);

    XASM(push 0x980724);
    XASM(push 0x80da78);
    XASM(ret);
}

void __cdecl TSetEvtScriptTestFileName(char **param_1, char *file_name, char param_3, char param_4, char file_name_buf_size)
{
#if 1
    // Not sure where this name is supposed to come from, but the pink disc prototype doesn't set anything.
    // This file must be located in the directory specified by string registry entry "SET_DIR".
    // TODO: Registry setting
    strcpy_s(file_name, file_name_buf_size, "events.evt");
#else
    param_1[0] = file_name;
    param_1[1] = param_3; // ?
    param_1[2] = param_4;
    param_1[3] = file_name_buf_size;
    param_1[4] = (char *)0x0;
    param_1[5] = (char *)0x0;
#endif
}

void ApplyTSetEvtScriptTestPatches()
{
    PatchCALL(0x80d829, 0x80d82e, (int)&TSetEvtScriptTestFileName);
    PatchJMP(0x80da51, 0x80da57, (int)&TSetEvtScriptTestMenuListingColor);
    PatchJMP(0x80da73, 0x80da78, (int)&TSetEvtScriptTestRestoreColor);

    uint32_t njPrintAtCalls[] = {
        0x80da60,
        0x80da7d,
        0x80da8f,
    };

    uint32_t vtableRenderAddrs[] = {
        0xb47750,
    };

    PatchNJPrintAtCalls(njPrintAtCalls, _countof(njPrintAtCalls));
    ReplaceTEditorRenderMethods(vtableRenderAddrs, _countof(vtableRenderAddrs));
}

#endif // PATCH_EDITORS
