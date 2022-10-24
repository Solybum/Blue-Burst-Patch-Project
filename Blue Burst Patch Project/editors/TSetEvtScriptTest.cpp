#ifdef PATCH_EDITORS
#include <stdlib.h>
#include "helpers.h"
#include "editors.h"

byte *Create_TSetEvtScriptTest()
{
    byte *(__cdecl * pf_Create)(void) = (byte * (__cdecl *)())0x4f7a78;

    (*pf_Create)();

    return GetLastEditor();
}

void __declspec(naked) TSetEvtScriptTestMenuListingColor()
{
    __asm {
        pushad;
        shr esi, 2;
        cmp[ebp + 0x40], esi;
        jne notSelected;
        push editorColorSelected;
        jmp getOut;

    notSelected:
        push editorColorDefault;

    getOut:
        // Set the debug color
        call EditorColorSet;
        pop ecx;
        popad;

        // original code and get out
        push dword ptr ds : [esi + 0xA11E30] ;
        push 0x80da57;
        ret;
    }
}

void __declspec(naked) TSetEvtScriptTestRestoreColor()
{
    __asm {
        call EditorColorReset;

        push 0x980724;
        push 0x80da78;
        ret;
    }
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
#endif
