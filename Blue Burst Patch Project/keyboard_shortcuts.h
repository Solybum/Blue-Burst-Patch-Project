#pragma once

extern void *keyboard_state;
extern BOOL(__cdecl *was_key_pressed)(unsigned char);
extern int(__fastcall *is_ctrl_pressed)(void *);

void ApplyKeyboardShortcuts();
