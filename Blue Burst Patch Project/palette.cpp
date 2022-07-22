#ifdef PATCH_KEYBOARD_ALTERNATE_PALETTE

#include "pch.h"
#include "helpers.h"
#include "palette.h"

int tmpCurrentPalette = 0;

void __declspec(naked) GetCurrentPalette()
{
    __asm
    {
        mov edx, [ebp - 0x14];
        mov edx, [edx + 0x2C];
        movzx edx, byte ptr[edx + 0x62];

        test edx, edx;
        jz _set_current_palette_main;
        jmp _set_current_palette_alt;

    _set_current_palette_main:
        mov byte ptr[tmpCurrentPalette], 0;
        jmp _after_set_current_palette;

    _set_current_palette_alt:
        mov byte ptr[tmpCurrentPalette], 1;
        jmp _after_set_current_palette;

    _after_set_current_palette:

        mov edx, edi;
        and edx, 0xFF;
        jmp addrGetCurrentPaletteO;
    }
}

void __declspec(naked) CheckHotkey1_1()
{
    __asm
    {
        cmp[tmpCurrentPalette], 0;

        jz _palette_main;
        jmp _palette_alt;

    _palette_main:
        movzx edx, byte ptr[eax + esi * 4 + 0x04];
        jmp addrCheckHotkey1_1O;

    _palette_alt:
        movzx edx, byte ptr[eax + esi * 4 + 0x3C];
        jmp addrCheckHotkey1_1O;
    }
}
void __declspec(naked) CheckHotkey1_2()
{
    __asm
    {
        cmp[tmpCurrentPalette], 0;
        jz _palette_main;
        jmp _palette_alt;

    _palette_main:
        movzx ecx, byte ptr[eax + ecx * 2 + 0x05];
        jmp addrCheckHotkey1_2O;

    _palette_alt:
        movzx ecx, byte ptr[eax + ecx * 2 + 0x3D];
        jmp addrCheckHotkey1_2O;
    }
}
void __declspec(naked) UnsetHotkey1()
{
    __asm
    {
        mov ecx, [ebp - 0x28];
        xor eax, eax;
        push eax;
        push eax;
        push esi;
        push[tmpCurrentPalette];
        push eax;
        call addrSetPaletteHotkey;

        jmp addrUnsetHotkey1O;
    }
}
void __declspec(naked) CheckHotkey2_1()
{
    __asm
    {
        cmp[tmpCurrentPalette], 0;

        jz _palette_main;
        jmp _palette_alt;

    _palette_main:
        movzx edx, byte ptr[edx + ebx * 4 + 0x04];
        jmp addrCheckHotkey2_1O;

    _palette_alt:
        movzx edx, byte ptr[edx + ebx * 4 + 0x3C];
        jmp addrCheckHotkey2_1O;
    }
}
void __declspec(naked) CheckHotkey2_2()
{
    __asm
    {
        cmp[tmpCurrentPalette], 0;

        jz _palette_main;
        jmp _palette_alt;

    _palette_main:
        movzx ecx, byte ptr[edx + eax * 2 + 0x05];
        jmp addrCheckHotkey2_2O;

    _palette_alt:
        movzx ecx, byte ptr[edx + eax * 2 + 0x3D];
        jmp addrCheckHotkey2_2O;
    }
}
void __declspec(naked) UnsetHotkey2()
{
    __asm
    {
        mov ecx, [ebp - 0x2C];
        xor eax, eax;
        push eax;
        push eax;
        push ebx;
        push[tmpCurrentPalette];
        push eax;
        call addrSetPaletteHotkey;

        jmp addrUnsetHotkey2O;
    }
}
void __declspec(naked) SetHotkey()
{
    __asm
    {
        mov eax, [ebp - 0x24];
        mov ecx, [ebp - 0x28];
        movzx ebx, word ptr[eax];
        movzx edx, word ptr[eax + 0x02];
        push edx;
        push ebx;
        push esi;
        push[tmpCurrentPalette];
        push 0;
        call addrSetPaletteHotkey;

        jmp addrSetHotkeyO;
    }
}
void __declspec(naked) CheckHotkey3_1()
{
    __asm
    {
        cmp[tmpCurrentPalette], 0;

        jz _palette_main;
        jmp _palette_alt;

    _palette_main:
        movzx ecx, byte ptr[eax + edx * 4 + 0x04];
        jmp addrCheckHotkey3_1O;

    _palette_alt:
        movzx ecx, byte ptr[eax + edx * 4 + 0x3C];
        jmp addrCheckHotkey3_1O;
    }
}
void __declspec(naked) CheckHotkey3_2()
{
    __asm
    {
        cmp[tmpCurrentPalette], 0;
        jz _palette_main;
        jmp _palette_alt;

    _palette_main:
        movzx ecx, byte ptr[eax + edx * 4 + 0x05];
        jmp addrCheckHotkey3_2O;

    _palette_alt:
        movzx ecx, byte ptr[eax + edx * 4 + 0x3D];
        jmp addrCheckHotkey3_2O;
    }
}

void PatchPalette()
{
    *(unsigned char*)addrSecondaryPaletteAttack1 = valSecondaryPaletteAttack1;
    *(unsigned char*)addrSecondaryPaletteAttack2 = valSecondaryPaletteAttack2;
    *(unsigned char*)addrSecondaryPaletteAttack3 = valSecondaryPaletteAttack3;

    PatchJMP(addrGetCurrentPaletteI, addrGetCurrentPaletteO, (int)&GetCurrentPalette);
    PatchJMP(addrCheckHotkey1_1I, addrCheckHotkey1_1O, (int)&CheckHotkey1_1);
    PatchJMP(addrCheckHotkey1_2I, addrCheckHotkey1_2O, (int)&CheckHotkey1_2);
    PatchJMP(addrUnsetHotkey1I, addrUnsetHotkey1O, (int)&UnsetHotkey1);

    PatchJMP(addrCheckHotkey2_1I, addrCheckHotkey2_1O, (int)&CheckHotkey2_1);
    PatchJMP(addrCheckHotkey2_2I, addrCheckHotkey2_2O, (int)&CheckHotkey2_2);
    PatchJMP(addrUnsetHotkey2I, addrUnsetHotkey2O, (int)&UnsetHotkey2);

    PatchJMP(addrSetHotkeyI, addrSetHotkeyO, (int)&SetHotkey);

    PatchJMP(addrCheckHotkey3_1I, addrCheckHotkey3_1O, (int)&CheckHotkey3_1);
    PatchJMP(addrCheckHotkey3_2I, addrCheckHotkey3_2O, (int)&CheckHotkey3_2);
}

#endif // PATCH_KEYBOARD_ALTERNATE_PALETTE
