#include "pch.h"
#include "helpers.h"
#include "shop.h"

void __declspec(naked) NoSellRareTool()
{
    __asm
    {
        mov edi, [eax + 0x14];
        and edi, 0x80;

        je _not_rare;

        //_rare:
        mov edi, 0;
        jmp _out;

    _not_rare:
        mov edi, [eax + 0x10];

    _out:
        jmp addrNoSellRareToolJMP;
    }
}

void PatchShop()
{
    // TODO obtain from registry/ini settings
    int reg_noRareSale = 1;

    if (reg_noRareSale == 1)
    {
        *(int*)addrNoSellUntekkedWeapon = 0;
        *(int*)addrNoSellRareWeapon = 0;
        *(int*)addrNoSellRareArmor = 0;

        PatchJMP(addrNoSellRareToolI, addrNoSellRareToolO, (int)&NoSellRareTool);
    }
}
