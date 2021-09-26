
#include "pch.h"
#include "helpers.h"
#include "slow_gibbles.h"

void __declspec(naked) SlowGibblesFix()
{
    __asm
    {
        // If this is null then gibbles was not initialized yet and cannot be slow
        mov eax, ds:[GIBBLES_INIT_SENTINEL]
        test eax, eax
        je _out

        // Get the original value
        mov eax, ds:[GIBBLES_ANIMATION_SPEEDS]
        fld [eax]
        // and restore the jump speed
        fstp ds:[GIBBLES_JUMP_ANIMATION_SPEED]

    _out:
        // Original code
        push 0
        lea ecx, [ebp - 0x60]

        jmp addrSlowGibblesFixJMP
    }
}

void ApplySlowGibblesFix()
{
    // Adding this to the "creating or joining a party" part of the main update function.
    // In other words slow gibbles will be fixed when a new party is created or joined.
    PatchJMP(addrSlowGibblesFixI, addrSlowGibblesFixO, (int) &SlowGibblesFix);
}
