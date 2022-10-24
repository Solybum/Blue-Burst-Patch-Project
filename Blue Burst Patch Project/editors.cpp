#ifdef PATCH_EDITORS
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "editors.h"
#include "helpers.h"
#include "keyboard.h"
#include "psobb_functions.h"

// If set, a list is maintained of the editor strings and they will be
// rendered later by the render methods. This requires replacing the render
// methods inside the vtables.
// This is just a hack for prototyping. Proper implementation should
// create a new surface, load a fixed width debug font, render the text
// on the surface, and then display the surface just before d3d8_device->Present().
// Current implementation has small placement issues because the font isn't
// fixed width.
#define DELAYED_RENDERING 1

// Flag passed to the EditorPrint* functions indicating rendering should be
// done now because caller is already inside rendering.
#define DISPLAY_NOW 0x1

// Singleton of each editor currently active
static byte *activeEditors[TEditorType::Editor_Max];

// Array of pointers to the create functions
static byte *(*Create_Functions[TEditorType::Editor_Max])();

int editorColorDefault = 0xFFFFFFFF;
int editorColorSelected = 0xFFFF0000;
static editorTextNode_t *editorTextList = NULL;
static uint16_t editorFontSize = 0xE;
static uint32_t editorFontColor = 0xFFFFFFFF;

// Freeze enemies.
static void PauseEnemyAI() { *(uint8_t *)0xa7276c = 1; }

// Allow enemies to continue.
static void ResumeEnemyAI() { *(uint8_t *)0xa7276c = 0; }

void __cdecl EditorFontSizeSet(uint16_t size) { editorFontSize = size; }

void __cdecl EditorColorSet(uint32_t col) { editorFontColor = col; }

uint32_t __cdecl EditorColorGet() { return editorFontColor; }

void __cdecl EditorColorReset() { editorFontColor = editorColorDefault; }

byte *GetLastEditor()
{
    byte **savedEditorArray = (byte **)0xa72720;
    uint32_t numSaved = *(uint32_t *)0xa72764;
    if (numSaved >= 1)
        return savedEditorArray[numSaved-1];
    
    return NULL;
}

void __cdecl EditorErrorWindow(const char *fmt, ...)
{
    char buf[128];
    wchar_t wbuf[128];
    size_t outSize = 0;
    va_list args;
    va_start(args, fmt);

    vsprintf_s(buf, _countof(buf), fmt, args);
    mbstowcs_s(&outSize, wbuf, _countof(wbuf), buf, -1);

    MessageBoxW(NULL, wbuf, L"Error!", 0);
    va_end(args);
}

void __cdecl EditorPrintAtConstantString(int display_flags, uint32_t loc, const char *str)
{
#if DELAYED_RENDERING==1
    uint32_t x = LOC_X(loc);
    uint32_t y = LOC_Y(loc); 
    wchar_t wbuf[128];
    size_t outSize = 0;
    float pos[2] = { (float)x * editorFontSize, (float)y * editorFontSize };

    mbstowcs_s(&outSize, wbuf, _countof(wbuf), str, -1);

    if (DISPLAY_NOW & display_flags)
    {
        // DISPLAY_NOW is intended for calls from render method
        (*pf_FogEnable_False)();
        (*pf_render_text)(pos[0], pos[1], 0.9999f, editorFontColor, wbuf);
    }
    else
    {
        // Prepend it to the list which is rendered later
        editorTextNode_t *newNode = (editorTextNode_t *)malloc(sizeof(editorTextNode_t));
        if (NULL == newNode)
            return;

        memset(newNode, 0, sizeof(editorTextNode_t));
        newNode->pos[0] = pos[0];
        newNode->pos[1] = pos[1];
        wcscpy_s(newNode->wbuf, wbuf);
        newNode->color = editorFontColor;
        newNode->next = editorTextList;

        editorTextList = newNode;
    }
#else
    // TODO: Draw onto a separate surface and display it before Present().
#endif
}

void __cdecl vEditorPrintAt(int display_flags, uint32_t loc, const char *fmt, va_list *args)
{
    char buf[128];
    
    if (args)
        vsprintf_s(buf, fmt, *args);
    else
        sprintf_s(buf, "%s", fmt);

    EditorPrintAtConstantString(display_flags, loc, buf);
}

void __cdecl EditorPrintAt(uint32_t loc, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vEditorPrintAt(0, loc, fmt, &args);	
    va_end(args);
}

void __cdecl EditorPrintAtNow(uint32_t loc, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vEditorPrintAt(DISPLAY_NOW, loc, fmt, &args);
    va_end(args);

}

void EditorPrint(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vEditorPrintAt(0, LOC(20, 40), fmt, &args);
    va_end(args);
}

void EditorPrintNow(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vEditorPrintAt(DISPLAY_NOW, LOC(20, 40), fmt, &args);
    va_end(args);
}

static void __fastcall RenderEditorDrawList(uint8_t *unused)
{
#if DELAYED_RENDERING==1
    (*pf_FogEnable_False)();

    editorTextNode_t *tmp = editorTextList;
    while (tmp)
    {
        editorTextNode_t *old = tmp;

        (*pf_render_text)(tmp->pos[0], tmp->pos[1], 0.9999f, tmp->color, tmp->wbuf);

        tmp = tmp->next;
        free(old);
    }

    editorTextList = NULL;
    (*pf_FogEnable_True)();
#endif
}


void PatchErrorWindows(uint32_t *addrs, uint32_t num_addrs)
{
    for (uint32_t i = 0; i < num_addrs; ++i)
        PatchCALL(addrs[i], addrs[i] + 5, (int)&EditorErrorWindow);
}

void PatchNJPrintAtCalls(uint32_t *addrs, uint32_t num_addrs)
{
    for (uint32_t i = 0; i < num_addrs; ++i)
        PatchCALL(addrs[i], addrs[i] + 5, (int)&EditorPrintAt);
}

void PatchNJPrintAtNowCalls(uint32_t *addrs, uint32_t num_addrs)
{
    for (uint32_t i = 0; i < num_addrs; ++i)
        PatchCALL(addrs[i], addrs[i] + 5, (int)&EditorPrintAtNow);
}

void PatchNJPrintCalls(uint32_t *addrs, uint32_t num_addrs)
{
    for (uint32_t i = 0; i < num_addrs; ++i)
        PatchCALL(addrs[i], addrs[i] + 5, (int)&EditorPrint);
}

void ReplaceTEditorRenderMethods(uint32_t *addrs, uint32_t num_addrs)
{
#if DELAYED_RENDERING==1
    for (uint32_t i = 0; i < num_addrs; ++i)
        *(uint32_t *)addrs[i] = (uint32_t)&RenderEditorDrawList;
#endif
}

static BOOL __cdecl DisableMovementIfEditorActive()
{
    auto editorCount = *reinterpret_cast<uint16_t*>(0xA72764);
    auto movementInputEnabled = *reinterpret_cast<BOOL*>(0xA0F6F0);

    return editorCount > 0 || !movementInputEnabled;
}

static void __thiscall ClearTEditorEx(void* editor)
{
    uint32_t i;
    for (uint32_t i = 0; i < _countof(activeEditors); ++i)
    {
        if (activeEditors[i] == editor)
            activeEditors[i] = NULL;
    }
    
    *(uint32_t *)0xA0F6F0 = 1; // movement_input_enabled
    
    // Remove it from the array in client
    for (i = 0; i < 16; ++i)
    {
        byte **tmp = (byte **)(0xa72720 + 4 * i);
        if (*tmp == editor)
        {
            // Found the editor. Remove it and shift the rest.
            *(uint32_t *)0xA72764 -= 1;

            *tmp = NULL;
            if (i < 15)
                memmove(tmp, tmp + 4 * (i + 1), 0xa72760 - (size_t)tmp);
        }
    }

    (*pf_opcode_hud_show)();
    ResumeEnemyAI();
}

static void __thiscall ClearTEditor(void* ptr)
{
    ClearTEditorEx(ptr);

    // Original code
    reinterpret_cast<void (__thiscall *)(void*)>(0x8171e8)(ptr);
}

static void ApplyTEditorPatches()
{
    // Cleanup memory when the TEditor goes away. Can happen due to scene change
    // or the keyboard shortcut.
    PatchCALL(0x4f70b2, 0x4f70b7, (int)&ClearTEditor);

    // Don't set flags in the object that prevent calling the render method.
    PatchNOP(0x4f740b, 4);

    // Remove movement input enabled check when instance is deconstructed
    PatchCALL(0x7bece2, 0x7bece7, (int)&DisableMovementIfEditorActive);
}

void ApplyEditorPatch()
{
    Create_Functions[TEditorType::Editor_TSetEvtScriptTest] = Create_TSetEvtScriptTest; 
    Create_Functions[TEditorType::Editor_TGroupSet] = Create_TGroupSetEditor;
    Create_Functions[TEditorType::Editor_TGroupEnemySet] = Create_TGroupEnemySetEditor;
    Create_Functions[TEditorType::Editor_TCamera] = Create_TCameraEditor;
    Create_Functions[TEditorType::Editor_TFreeCamera] = Create_TFreeCamera;
    Create_Functions[TEditorType::Editor_TParticle] = Create_TParticleEditor;
    Create_Functions[TEditorType::Editor_TQuestScriptChecker] = Create_TQuestScriptChecker;

    // Patches for the various classes
    ApplyTEditorPatches();
    ApplyTSetEvtScriptTestPatches();
    ApplyTGroupSetEditorPatches();
    ApplyTGroupEnemySetEditorPatches(); 
    ApplyTParticleEditorPatches();
    ApplyTCameraEditorPatches();
    ApplyTFreeCameraPatches();
    ApplyTQuestScriptCheckerPatches();

    Keyboard::onKeyDown({Keyboard::Keycode::Ctrl, Keyboard::Keycode::T}, []() {
        ToggleEditorShortcut();
    });
}

static void ToggleEditor(TEditorType toggleType)
{
    if (toggleType < Editor_EMPTY || toggleType >= Editor_Max)
        return;

    if (activeEditors[toggleType])
    {
        *(uint32_t *)(activeEditors[toggleType] + 0x8) |= 0x1; // destruct
        activeEditors[toggleType] = NULL;
    }
    else
    {
        // Disable the hud to avoid visibility issues and weird interactions
        // with symbol chat and word select menus.
        (*pf_opcode_hud_hide)();
        PauseEnemyAI();

        activeEditors[toggleType] = Create_Functions[toggleType]();
    }
}

void ToggleEditorShortcut()
{
    // Working editors (some functionality missing but still usable).
    // Editor_TGroupSet
    // Editor_TGroupEnemySet
    // Editor_TSetEvtScriptTest

    // Somewhat working but not ideal. Missing a key piece of functionality.
    // Editor_TCamera

    // Doesn't do anything in BB and some other versions.
    // Editor_TFreeCamera

    // Not working or has major issues.
    // Editor_TParticle

    // Replace the argument and then call this function.
    //ToggleEditor(TEditorType::Editor_TParticle);
    ToggleEditor(TEditorType::Editor_TQuestScriptChecker);
}

#endif
