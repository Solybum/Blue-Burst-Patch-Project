// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "globals.h"
#include "psobb.h"

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        {
            // TODO move client name somewhere else
            FILE* fp;
            _wfopen_s(&fp, clientName, L"rb");
            if (fp != NULL)
            {
                fclose(fp);

                PSOBB();
            }
            else
            {
                MessageBoxA(NULL, "Client executable not found, please check that your client executable is named psobb.exe and has not been quarantined by your antivirus", "Error", 0);
                ExitProcess(1);
            }
        }
        break;
        case DLL_PROCESS_DETACH:
        {
        }
        break;
    }
    return TRUE;
}

