#pragma once

#include <stdint.h>

extern void(__cdecl *pf_FogEnable_True)();
extern void(__cdecl *pf_FogEnable_False)();

extern uint32_t(__cdecl *pf_render_text)(float, float, float, uint32_t, wchar_t *);

extern void(__stdcall *pf_opcode_hud_hide)();
extern void(__stdcall *pf_opcode_hud_show)();

extern void(__cdecl *pf_nop)();
extern void *(__cdecl *pf_malloc)(size_t);
extern void(__cdecl *pf_free)(void *);


extern int(__cdecl *was_key_pressed)(unsigned char);
extern int(__fastcall *is_ctrl_pressed)(void *);
extern int(__fastcall *is_alt_pressed)();
extern int(__cdecl *is_pso_keycode_pressed_with_only_alt)(uint8_t);
extern int(__fastcall *is_key_pressed)(unsigned char);
extern int(__cdecl *is_mouse_button_pressed)(unsigned int);
extern int(__cdecl *was_mouse_button_pressed)(unsigned int);
