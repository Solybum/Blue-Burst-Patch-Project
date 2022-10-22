#pragma once
#include "helpers.h"

// Get X from encoded location
#define LOC_X(l) ((uint32_t)(l) >> 16)
// Get Y from encoded location
#define LOC_Y(l) ((uint32_t)(l) & 0xFFFF)
// Encode (x,y) as a location on the screen in logical char units.
#define LOC(x,y) (((x) << 16) | (y))

typedef enum : signed int {
	Editor_EMPTY = 0,
	Editor_TGroupSet,
	Editor_TGroupEnemySet,
	Editor_TSetEvtScriptTest,
	Editor_TCamera,
	Editor_TFreeCamera,
	Editor_TParticle,
	Editor_TQuestScriptChecker,
	Editor_Max,
} TEditorType;

typedef struct editorTextNode {
	float pos[2];
	wchar_t wbuf[128];
	uint32_t color; // from debugFontColor at time of call
	struct editorTextNode *next;
} editorTextNode_t;

// Default color
extern int editorColorDefault;

// Color of a selected element
extern int editorColorSelected;

// Returns address of most recently created editor.
byte *GetLastEditor();

// Not used yet
void __cdecl EditorFontSizeSet(uint16_t size);

// Sets the color of the text.
void __cdecl EditorColorSet(uint32_t col);

// Get the current color in case of temporary changes.
uint32_t __cdecl EditorColorGet();

// Print text at a logical location on the screen.
void __cdecl EditorPrintAt(uint32_t loc, const char *fmt, ...);

// Print text on the screen. Uses a hardcoded location.
void __cdecl EditorPrint(const char *fmt, ...);

// Immediately print text on the screen. Uses a hardcoded location.
void __cdecl EditorPrintNow(const char *fmt, ...);

// Error routine to display a MessageBox. 
void __cdecl EditorErrorWindow(const char *fmt, ...);

// Print text at a logical location. The string was already formatted.
void __cdecl EditorPrintAtConstantString(int display_flags, uint32_t loc, const char *str);

// va_list version of EditorPrintAt
void __cdecl vEditorPrintAt(int display_flags, uint32_t loc, const char *fmt, va_list *args);

// Set color back to the default.
void __cdecl EditorColorReset();

// Replace calls for errors with EditorErrorWindow().
void PatchErrorWindows(uint32_t *addrs, uint32_t num_addrs);

// Replace calls with EditorPrintAt().
void PatchNJPrintAtCalls(uint32_t *addrs, uint32_t num_addrs);

// Replace calls with EditorPrintAtNow().
void PatchNJPrintAtNowCalls(uint32_t *addrs, uint32_t num_addrs);

// Replace calls with EditorPrint().
void PatchNJPrintCalls(uint32_t *addrs, uint32_t num_addrs);

// Replace addresses in vtables with the delayed rendering method.
void ReplaceTEditorRenderMethods(uint32_t *addrs, uint32_t num_addrs);

void ApplyTSetEvtScriptTestPatches();
byte *Create_TSetEvtScriptTest();

void ApplyTQuestScriptCheckerPatches();
byte *Create_TQuestScriptChecker();

void ApplyTGroupSetEditorPatches();
byte *Create_TGroupSetEditor();

void ApplyTGroupEnemySetEditorPatches();
byte *Create_TGroupEnemySetEditor();

void ApplyTCameraEditorPatches();
byte *Create_TCameraEditor();

void ApplyTFreeCameraPatches();
byte *Create_TFreeCamera();

void ApplyTParticleEditorPatches();
byte *Create_TParticleEditor();

