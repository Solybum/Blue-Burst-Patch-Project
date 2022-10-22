#include "pch.h"
#include <stdlib.h>
#include "helpers.h"
#include "editors.h"
#include "psobb_functions.h"

static byte *TGroupSetEditor_instance = NULL;

static const uint32_t bufferSize = 0x8800; // 0x200 object inits

uint8_t *Create_TGroupSetEditor()
{
    void(__cdecl * pf_Create)() = (void(__cdecl *)())0x4fbce4;
    void(__cdecl * pf_Buffer_Allocate)() = (void(__cdecl *)())0x4ff310;

    (*pf_Buffer_Allocate)();
    
    (*pf_Create)();

    TGroupSetEditor_instance = GetLastEditor();
    if (TGroupSetEditor_instance)
    {
        // Freed by destructor
        *(byte **)(TGroupSetEditor_instance + 0x100) = (byte *)(*pf_malloc)(bufferSize);
    }

    return TGroupSetEditor_instance;
}

// Free the object init buffer when this instance goes away
static void __declspec(naked) FreeBuffer()
{
    __asm {
        pushad;
        mov eax, 0x4fb6f4;
        call eax;
        popad;
        mov TGroupSetEditor_instance, 0;

        mov dword ptr[ebp - 0x4], 0xFFFFFFFF;
        push 0x4fbc24;
        ret;
    }
}

static void __stdcall SetMenuColorEx(uint32_t objIdx, uint32_t idx)
{
    if (objIdx == idx)
        EditorColorSet(editorColorSelected);
    else
        EditorColorSet(editorColorDefault);
}

static void __declspec(naked) SetMenuColor1()
{
    __asm {
        pushad;
        push ebx; 
        mov eax, [ebp - 0x14];
        push dword ptr[eax + 0xf8];
        call SetMenuColorEx;
        popad;

        push dword ptr ds : [0xA0F688] ;
        push 0x4f9134;
        ret;
    }
}
static void __declspec(naked) SetMenuColor2()
{
    __asm {
        pushad;
        push ebx;
        mov eax, [ebp - 0x14];
        push dword ptr[eax + 0xec];
        call SetMenuColorEx;
        popad;

        push dword ptr ds : [0xA0F688] ;
        push 0x4fcb08;
        ret;
    }
}

static void __declspec(naked) ResetColor1()
{
    __asm {
        pushad;
        call EditorColorReset;
        popad;

        push 0x4f9da1;
        ret;
    }
}

static void __declspec(naked) ResetColor2()
{
    __asm {
        pushad;
        call EditorColorReset;
        popad;

        push 0x4fd8fa;
        ret;
    }
}

// Wrapper around the usual print location function. Sets the color.
// Specific to this object.
static void NJPrintAtWithColor(uint32_t loc, const char *fmt, ...)
{
    uint16_t yLoc = LOC_Y(loc);
    uint16_t xLocStart = LOC_X(loc);
    uint32_t saveColor;
    va_list args;
    va_start(args, fmt);

    saveColor = EditorColorGet();

    EditorColorSet(editorColorDefault);
    if (TGroupSetEditor_instance)
    {
        uint32_t currentIndex = *(uint32_t *)(TGroupSetEditor_instance + 0x9c);

        // Menu listing is 0 to 11 inclusive.
        // Pair up hardcoded y locations with the menu listing to color
        // the current selection properly.
        if ((0 == currentIndex && 0x14 == yLoc) ||
            (1 == currentIndex && 0x15 == yLoc) ||
            (2 == currentIndex && 0x16 == yLoc) ||
            (3 == currentIndex && 0x18 == yLoc) ||
            (4 == currentIndex && 0x19 == yLoc) ||
            (5 == currentIndex && 0x1a == yLoc) ||
            (6 == currentIndex && 0x1c == yLoc) ||
            (7 == currentIndex && 0x1d == yLoc) ||
            (8 == currentIndex && 0x1e == yLoc) ||
            (9 == currentIndex && 0x20 == yLoc) ||
            (10 == currentIndex && 0x21 == yLoc) ||
            (11 == currentIndex && 0x22 == yLoc))
        {
            // TODO: For current selection, should also show which digit is selected.
            EditorColorSet(editorColorSelected);
        }
    }

    vEditorPrintAt(0, loc, fmt, &args);
    EditorColorSet(saveColor);

    va_end(args);
}

void ApplyTGroupSetEditorPatches()
{
    PatchJMP(0x4f912e, 0x4f9134, (int)&SetMenuColor1);
    PatchJMP(0x4fcb02, 0x4fcb08, (int)&SetMenuColor2);
    PatchJMP(0x4f91ef, 0x4f91f4, (int)&ResetColor1);
    PatchJMP(0x4fcbc3, 0x4fcbc8, (int)&ResetColor2);
    PatchJMP(0x4fbc1d, 0x4fbc24, (int)&FreeBuffer);

    // Fix message boxes
    *(uint8_t *)(0x4fd93f + 1) = 0x8E;
    *(uint8_t *)(0x4fd95b + 1) = 0x8E;

    // Patches to fix the filename selection. Looks like there was a way
    // to enter filename for saving/loading, but it doesn't exist in DC
    // and doesn't seem to work in BB even if you ignore the missing UI.
    // Don't change the filename used for saving/loading
    PatchNOP(0x4fc903, 4);
    PatchNOP(0x4fc937, 4);

    // Don't break file name
    PatchNOP(0x4fc981, 0x4fc984 - 0x4fc981);
    PatchNOP(0x4fc98d, 0x4fc996 - 0x4fc98d);

    uint32_t njPrintAtCalls[] = {
        // func 4fbf3c
        0x4fbf5a, 
        0x4fbf6c, 
        0x4fbf80, 
        0x4fbfa1,
        0x4fbfbf,
        0x4fbfdd,
        0x4fbff2,
        0x4fc007,
        0x4fc01c,
        0x4fc031,
        0x4fc046,
        0x4fc128,
        0x4fc14a,
        0x4fc15f,
        0x4fc2e5,
        0x4fc2f9,
        // func 4fc370
        0x4fd6aa,
        0x4fd65f,
        //0x4fd67f,
        //0x4fd692,
        //0x4fd6aa,
        0x4fcaf8,
        0x4fcbaf,
        0x4fcb4d,
        0x4fcb71,
        0x4fcb90,
        // func 4f7db0
        0x4f7e74,
        0x4f7e83,
        0x4f7ea3,
        0x4f7ec0,
        0x4f7edd,

#ifdef DISPLAY_HARDCODED_OBJ_PARAMS
        // There aren't many of these and it would be better to parse
        // itemsname.ini from a Qedit install. Would be a lot nicer and 
        // more flexible.
        
        // TObjRoomId
        0x4fbf31,
        // Unknown (obj 903), doesn't seem to be available in the editor
        0x612250,
        0x612271,
        0x612292,
        0x6122a1,
        0x6122b0,
        0x6122d1,
        0x6122ef,
        // Object 204 (floating jellyfish)
        0x62cf7f,
        0x62cf8e,
        0x62cf9d,
#endif
    };

    uint32_t setColorNjPrintAtCalls[] = {
        // func 4fed54
        0x4fed73,
        0x4fed91,
        0x4fedaf,
        // func 4fee34
        0x4fee4d,
        0x4fee62,
        0x4fee77,
        // func 4fef1c
        0x4fef3b,
        0x4fef59,
        0x4fef77,
        // func 4feffc
        0x4ff016,
        0x4ff02b,
        0x4ff040,
    };

    uint32_t renderMethods[] = {
        0xafe2c0,
    };

    uint32_t errorWindows[] = {
        0x4ff2f5,
        0x4fdc9e,
        0x4fdc55,
        0x4fd3fb,
    };

    PatchNJPrintAtCalls(njPrintAtCalls, _countof(njPrintAtCalls));
    ReplaceTEditorRenderMethods(renderMethods, _countof(renderMethods));
    PatchErrorWindows(errorWindows, _countof(errorWindows));
    for (uint32_t addr : setColorNjPrintAtCalls)
        PatchCALL(addr, addr + 5, (int)&NJPrintAtWithColor);

    
}