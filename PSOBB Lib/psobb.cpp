#include "pch.h"
#include "globals.h"

void PSOBB()
{
    memset((void*)addrMainGameGuardCall, 0x90, 0x05);

    MessageBoxA(NULL, "Yeet, we got a DLL", "Hi", 0);
}
