#include "pch.h"
#ifdef PATCH_EDITORS
#include <stdlib.h>
#include "helpers.h"
#include "editors.h"

static byte *TQuestScriptChecker_instance = NULL;

byte *Create_TQuestScriptChecker()
{
    byte *(__cdecl * pf_Create)(void) = (byte * (__cdecl *)())0x4f7aec;

    (*pf_Create)();

    TQuestScriptChecker_instance = GetLastEditor();

    return TQuestScriptChecker_instance;
}

static const char *label = "LABEL: %i";
static void __declspec(naked) FixFunctionDisplay()
{
    __asm {
        push dword ptr[ebp + 0x38]; 
        push label;
        push 0x6bad6f;
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
    if (TQuestScriptChecker_instance)
    {
        bool selected = false;
        uint32_t currentMenu = *(uint32_t *)(TQuestScriptChecker_instance + 0x84);
        switch (currentMenu)
        {
            case 0: // start thread
            case 1: // gd load
            case 2: // ev load
                // only 1 for these menus
                selected = true;
                break;

            case 3: // registers
            {
                uint32_t selectedRegister = *(uint32_t *)(TQuestScriptChecker_instance + 0x90);
                uint32_t registerPage = selectedRegister / 0xF;
                uint32_t selectedRegisterOnPage = selectedRegister % 0xF;
                if ((yLoc - 0xA) == selectedRegisterOnPage)
                    selected = true;
                break;
            }

            case 4: // breakpoints
            {
                uint32_t currentBreakpointIdx = *(uint32_t *)0xa95650;
                if ((yLoc - 0xC) == currentBreakpointIdx)
                    selected = true;
                break;
            }

            default:
                break;
        }
        
        if (selected)
        {
            // TODO: For current selection, should also show which digit is selected.
            EditorColorSet(editorColorSelected);
        }
    }

    vEditorPrintAt(0, loc, fmt, &args);
    EditorColorSet(saveColor);

    va_end(args);
}

static const char *change = "CHANGE: %u";

static void __declspec(naked) ShowChangeEx()
{
    __asm {
        push esi;
        push edi; 
        mov esi, dword ptr[esp + 0x10]; // field offset
        mov edi, dword ptr[esp + 0xC]; // y offset

        // save to restore these values
        push eax;
        push ebx;

        mov ecx, TQuestScriptChecker_instance;
        add ecx, esi;
        mov eax, dword ptr[ecx];
        mov ecx, dword ptr[eax + 0x08];
        mov eax, 1;

    _loop:
        cmp ecx, 0;
        jle _done;
        mov ebx, 10;
        mul ebx;
        sub ecx, 1;
        jmp _loop;

    _done:
        push eax; // value
        push change; // fmt string
        lea ebx, [0x40000 + edi];
        push ebx; // loc
        call EditorPrintAt;
        add esp, 0xC;

        pop ebx;
        pop eax;
        pop edi;
        pop esi;
        ret;
    }
}

static void __declspec(naked) ShowChange1() 
{
    __asm {
        push 0x44;
        push 0xC;
        call ShowChangeEx;
        add esp, 0x8;

        push 0xff00ffff;
        push 0x6bad81;
        ret;
    }
}

static void __declspec(naked) ShowChange2() 
{
    __asm {
        push 0x44;
        push 0x1B;
        call ShowChangeEx;
        add esp, 0x8;

        mov esi, dword ptr[esp + 0x08];
        mov edi, dword ptr[esp + 0x04];
        push 0x6bb0a4;
        ret;
    }
}

static void __declspec(naked) ShowChange3()
{
    __asm {
        push 0x48;
        push 0x13;
        call ShowChangeEx;
        add esp, 0x8;

        mov edi, dword ptr[esp];
        mov ebp, dword ptr[esp + 0x04];
        push 0x6ba9a4;
        ret;
    }
}

void ApplyTQuestScriptCheckerPatches()
{
    // Add label number which was missing. Also change the text to be about labels
    // and not events.
    PatchJMP(0x6bad6a, 0x6bad6f, (int)&FixFunctionDisplay);
    *(uint8_t *)(0x6bad79 + 2) = 0xC; // added an extra argument so fix stack

    // Show how much the label input will increase by
    PatchJMP(0x6bad7c, 0x6bad81, (int)&ShowChange1);
    // And registers
    PatchJMP(0x6bb09c, 0x6bb0a4, (int)&ShowChange2);
    // And breakpoints
    PatchJMP(0x6ba99d, 0x6ba9a4, (int)&ShowChange3);

    uint32_t njPrintAtCalls[] = {
        // func 6ba72c
        0x6ba780,
        0x6ba946,

        // func 6bb0ac
        0x6bb0b6,
    };
    PatchNJPrintAtCalls(njPrintAtCalls, _countof(njPrintAtCalls));

    uint32_t setColorNjPrintAtCalls[] = {
        // func 6ba72c
        0x6ba98a,

        // func 6bac60
        0x6bad74,

        // func 6bab34
        0x6bab68,

        // func 6baa1c
        0x6baa50,

        // func 6badb4
        0x6baff3,
        0x6bb087,
    };

    uint32_t vtableRenderAddrs[] = {
        0xb3ab40,
    };

    //PatchNJPrintAtCalls(njPrintAtCalls, _countof(njPrintAtCalls));
    for (uint32_t addr : setColorNjPrintAtCalls)
        PatchCALL(addr, addr + 5, (int)&NJPrintAtWithColor);
    ReplaceTEditorRenderMethods(vtableRenderAddrs, _countof(vtableRenderAddrs));
}
#endif
