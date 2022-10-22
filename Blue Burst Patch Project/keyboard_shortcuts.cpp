#include "pch.h"
#ifdef PATCH_KEYBOARD_SHORTCUTS
#include "helpers.h"
#include "keyboard_shortcuts.h"
#include "editors.h"

void *keyboard_state = (void *)0xa9caa0;
BOOL(__cdecl *was_key_pressed)(unsigned char) = (BOOL(__cdecl *)(unsigned char))0x7bfff8;
int(__fastcall *is_ctrl_pressed)(void *) = (int(__fastcall *)(void *))0x78f484;

static void __stdcall CheckForShortcutsEx()
{
	int ctrl_pressed = (*is_ctrl_pressed)(keyboard_state);
	if (ctrl_pressed)
	{
		if ((*was_key_pressed)(0x23)) // t
		{
#ifdef PATCH_EDITORS
			ToggleEditorShortcut();
#endif
		}
	}
}

static void __declspec(naked) CheckForShortcuts()
{
	__asm
	{
		call CheckForShortcutsEx;
		// original code
		add dword ptr ds : [0xa9c4ec] , 0x1;
		push 0x7a5f2a;
		ret;
	}
}

void ApplyKeyboardShortcuts()
{
    PatchJMP(0x7a5f23, 0x7a5f2a, (int)&CheckForShortcuts);
}
#endif
