#include "pch.h"
#include "helpers.h"
#include "earlywalk.h"

/*
# Why does early walk happen?
There is a short random delay after an enemy has finished
its spawning animation before it starts searching for a target.
If the random delay happens to roll a 0 then the enemy will
immediately start chasing its target, skipping the battle cry animation.

# How does the patch work?
Conveniently there is already a flag on the enemy that gets set when
the entity has fully finished spawning (0x32e, set at 0x00535ac5).
We simply prevent the enemy from chasing any targets until that flag is set,
allowing it to do the battle cry animation every time.
*/

// applies to all "booma type" enemies like sharks, dimenians, gorans etc.
// TODO: figure out the complete list of enemies this applies to
void __declspec(naked) BoomaEarlyWalkFix()
{
    __asm
    {
        // check if fully finished spawning
        movsx eax, word ptr [ecx + 0x32e]
        test eax, eax
        // skip code that causes booma to start chasing its target
        je _skip
        // original code
        mov esi, ecx
        mov eax, [esi + 0x398]
        jmp addrBoomaEarlyWalkFixO

    _skip:
        jmp addrBoomaEarlyWalkFixJMP
    }
}

void ApplyEarlyWalkFix()
{
    // TODO: dorphon and delbiter early walk fix?
    PatchJMP(addrBoomaEarlyWalkFixI, addrBoomaEarlyWalkFixO, (int) &BoomaEarlyWalkFix);
}
