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
