#include "pch.h"
#include "globals.h"
#include "shop.h"

void PSOBB()
{
    memset((void*)addrMainGameGuardCall, 0x90, 0x05);

    PatchShop();

    MessageBoxA(NULL, "Yeet, we got a DLL", "Hi", 0);
}
