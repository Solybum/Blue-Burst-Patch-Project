#include "psobb_functions.h"

void(__cdecl *pf_FogEnable_True)() = (void(__cdecl *)())0x82dab0;
void(__cdecl *pf_FogEnable_False)() = (void(__cdecl *)())0x82dbac;

extern uint32_t(__cdecl *pf_render_text)(float, float, float, uint32_t, wchar_t *) =
    (uint32_t(__cdecl *)(float, float, float, uint32_t, wchar_t *))0x789248;

extern void(__stdcall *pf_opcode_hud_hide)() = (void(__stdcall *)())0x6b375c;
extern void(__stdcall *pf_opcode_hud_show)() = (void(__stdcall *)())0x6b3770;

extern void(__cdecl *pf_nop)() = (void(__cdecl *)())0x61cdb0;
extern void *(__cdecl *pf_malloc)(size_t) = (void *(__cdecl *)(size_t))0x859c55;
extern void(__cdecl *pf_free)(void *) = (void(__cdecl *)(void *))0x858882;
