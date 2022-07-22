#ifdef PATCH_IME

#include "pch.h"
#include "ime.h"

void PatchIme()
{
    // TODO obtain from registry/ini settings
    int reg_ImeEnabled = 0;

    if (reg_ImeEnabled != 1)
    {
        *(int*)addrImeCall = valImePatch;
    }
}

#endif // PATCH_IME
