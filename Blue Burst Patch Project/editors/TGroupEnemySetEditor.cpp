#include "pch.h"
#include <stdlib.h>
#include "helpers.h"
#include "editors.h"
#include "psobb_functions.h"

static byte *TGroupEnemySetEditor_instance = NULL;

static const uint32_t bufferSize = 0x4800; // 0x100 monster inits

uint8_t *Create_TGroupEnemySetEditor()
{
    void(__cdecl * pf_Create)() = (void(__cdecl *)())0x4fb61c;
    void(__cdecl * pf_Buffer_Allocate)() = (void(__cdecl *)())0x4fb740;

    (*pf_Buffer_Allocate)();

    (*pf_Create)();

    TGroupEnemySetEditor_instance = GetLastEditor();
    if (TGroupEnemySetEditor_instance)
    {
        // This buffer is automatically freed by destructor
        *(byte **)(TGroupEnemySetEditor_instance + 0x10c) = (byte *)(*pf_malloc)(bufferSize);
    }

    return TGroupEnemySetEditor_instance;
}

// Free the object init buffer when this instance goes away
static void __declspec(naked) FreeBuffer()
{
    __asm {
        pushad;
        mov eax, 0x4fb6f4;
        call eax;
        popad;
        mov TGroupEnemySetEditor_instance, 0;

        mov dword ptr[ebp - 0x4], 0xFFFFFFFF;
        push 0x4f84a3;
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
    if (TGroupEnemySetEditor_instance)
    {
        uint32_t currentIndex = *(uint32_t *)(TGroupEnemySetEditor_instance + 0x9c);

        // Menu listing is 0 to 13 inclusive.
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
            (11 == currentIndex && 0x22 == yLoc) ||
            (12 == currentIndex && 0x23 == yLoc) ||
            (13 == currentIndex && 0x24 == yLoc))
        {
            // TODO: For current selection, should also show which digit is selected.
            EditorColorSet(editorColorSelected);
        }
    }

    vEditorPrintAt(0, loc, fmt, &args);
    EditorColorSet(saveColor);

    va_end(args);
}

static void __declspec(naked) FixFile1()
{
    __asm {
        add ebx, 0x11c;
        push ebx;
        push 0x91ec80;
        push 0x4fa0f1;
        ret;
    }
}

void ApplyTGroupEnemySetEditorPatches()
{
    // Patches for the right menu are already done through TGroupSetEditor patches
    PatchJMP(0x4f849c, 0x4f84a3, (int)&FreeBuffer);

    // Fix crashes when doing CLEAR or CLEAR(ALL) options in the menu...
    *(uint32_t *)(0x4fb74a + 1) = bufferSize;
    *(uint32_t *)(0x4fb75f + 1) = bufferSize;
    *(uint32_t *)(0x4fb7ac + 1) = bufferSize;

    // Allow our messagebox to run for errors
    *(uint8_t *)(0x4f9de3 + 1) = 0x8E;
    *(uint8_t *)(0x4f9dff + 1) = 0x8E;

    // Fix the error file names
    PatchJMP(0x04fa0e8, 0x4fa0f1, (int)&FixFile1);

    // The filename used for saving and loading should be "enemyentry.dat" but
    // it's overwritten during processing.
    // Looks like there may have been some kind of file name prompt?
    PatchNOP(0x4f8f29, 4);
    PatchNOP(0x4f8f5d, 4);

    // Don't break the file name
    PatchNOP(0x4f8fa7, 0x4f8faa - 0x4f8fa7);
    PatchNOP(0x4f8fb3, 0x4f8fbc - 0x4f8fb3);

    uint32_t njPrintAtCalls[] = {
        // func 4f84b8
        0x4f84d6,
        0x4f84e8,
        0x4f84fc,
        0x4f851d,
        0x4f853b,
        0x4f8559,
        0x4f856e,
        0x4f8583,
        0x4f8598,
        0x4f85ad,
        0x4f85c2,
        0x4f88ac,
        0x4f86cd,
        0x4f86ef,
        0x4f8704,

        // func 4f8a2c
        0x4f9124,
        0x4f91db,
        0x4f9179,
        0x4f919d,
        0x4f91bc,
    };

    uint32_t setColorNjPrintAtCalls[] = {
        // func 4fb15c
        0x4fb17b,
        0x4fb199,
        0x4fb1b7,
        // func 4fb23c
        0x4fb255,
        0x4fb26a,
        0x4fb27f,
        // func 4fb324
        0x4fb34a,
        0x4fb368,
        0x4fb386,
        0x4fb3a4,
        0x4fb3c2,
        // func 4fb4a0
        0x4fb4b9,
        // func 4fb4f4
        0x4fb50d,
        // func 4fb548
        0x4fb561,
    };

    uint32_t renderMethods[] = {
        0xafe1e0,
    };

    uint32_t errorWindows[] = {
        0x4fbb05,
        0x4f9bc5,
        0x4f9958,
        0x4f9968,

        // func 4f9db8
        0x4fa17d,
        0x4fa0f1,
        0x4fa165,
    };

    PatchNJPrintAtCalls(njPrintAtCalls, _countof(njPrintAtCalls));
    ReplaceTEditorRenderMethods(renderMethods, _countof(renderMethods));
    PatchErrorWindows(errorWindows, _countof(errorWindows));
    for (uint32_t addr : setColorNjPrintAtCalls)
        PatchCALL(addr, addr + 5, (int)&NJPrintAtWithColor);
}

