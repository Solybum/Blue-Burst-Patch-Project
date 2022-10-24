#ifdef PATCH_EDITORS
#include "../helpers.h"
#include "editors.h"
#include "psobb_functions.h"

static byte *TCameraEditor_instance = NULL;

uint8_t *Create_TFreeCamera()
{
    void (__cdecl * pf_Create)() = (void (__cdecl *)())0x78afe4;

    (*pf_Create)();

    TCameraEditor_instance = GetLastEditor();

    return TCameraEditor_instance;
}

// Not implemented yet, has input issues and there's no UI in any version of the game.
void ApplyTFreeCameraPatches()
{

}

#endif
