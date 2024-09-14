#include "psobb_functions.h"

void(__cdecl *pf_FogEnable_True)() = (void(__cdecl *)())0x82dab0;
void(__cdecl *pf_FogEnable_False)() = (void(__cdecl *)())0x82dbac;

uint32_t(__cdecl *pf_render_text)(float, float, float, uint32_t, wchar_t *) =
    (uint32_t(__cdecl *)(float, float, float, uint32_t, wchar_t *))0x789248;

void(__stdcall *pf_opcode_hud_hide)() = (void(__stdcall *)())0x6b375c;
void(__stdcall *pf_opcode_hud_show)() = (void(__stdcall *)())0x6b3770;

void(__cdecl *pf_nop)() = (void(__cdecl *)())0x61cdb0;
void *(__cdecl *pf_malloc)(size_t) = (void *(__cdecl *)(size_t))0x859c55;
void(__cdecl *pf_free)(void *) = (void(__cdecl *)(void *))0x858882;

int(__cdecl *was_key_pressed)(unsigned char) = (int(__cdecl *)(unsigned char))0x7bfff8;
int(__fastcall *is_ctrl_pressed)(void *) = (int(__fastcall *)(void *))0x78f484;
int(__fastcall *is_alt_pressed)() = (int(__fastcall *)())0x7c00d0;
int(__cdecl *is_pso_keycode_pressed_with_only_alt)(uint8_t) = (int(__cdecl *)(uint8_t))0x7c0044;
int(__fastcall *is_key_pressed)(unsigned char) = (int(__fastcall *)(unsigned char))0x7bfce4;
int(__cdecl *is_mouse_button_pressed)(unsigned int) = (int(__cdecl *)(unsigned int))0x7c00e8;
int(__cdecl *was_mouse_button_pressed)(unsigned int) = (int(__cdecl *)(unsigned int))0x7c0108;
