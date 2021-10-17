#include "helpers.h"
#include "fastwarp.h"

/*
This patch works by skipping the sleep portion of the render function
during loading screens to allow the game to load as fast as possible.
*/

#define LOGIN_CONNECTION_MANAGER 0x00a3ad08
#define QUEST_STARTED 0x00a95624

int sleepSkipAddr = 0x0083ae4c;
int renderSkipInAddr = 0x0083ae11;
int renderSkipOutAddr = 0x0083ae16;

int skipNextFrameAlso = 0;
int skipSleep = 0;

void __declspec(naked) CheckRenderSkip()
{
    __asm
    {
        mov eax, dword ptr [skipSleep]
        test eax, eax
        jne goto_skip

        // To skip the login loading screen, check if the login connection manager exists
        mov eax, ds:[LOGIN_CONNECTION_MANAGER]
        test eax, eax
        // Also skip the next frame because the loading screen is up for one frame after login finishes
        // and showing only one frame of it looks ugly
        jne skip_next_frame_also

        mov eax, dword ptr [skipNextFrameAlso]
        test eax, eax
        // Don't skip the next frame anymore
        jne dont_skip_next_frame

    dont_skip:
        jmp renderSkipOutAddr

    skip_next_frame_also:
        mov dword ptr [skipNextFrameAlso], 1
        jmp goto_skip

    dont_skip_next_frame:
        mov dword ptr [skipNextFrameAlso], 0
        jmp goto_skip

    goto_skip:
        jmp sleepSkipAddr
    }
}

int renderFuncAddr = 0x0080030c;

int assetLoadPatchInAddr = 0x005b92d9;
int assetLoadPatchOutAddr = 0x005b92de;

int questNotLoadingFuncAddr = 0x007a6154;
int questDoneLoadingInAddr = 0x007bcff4;
int questDoneLoadingOutAddr = 0x007bcff9;

int questDoneLoading = 0;

void __declspec(naked) PatchAllAssetLoading()
{
    __asm
    {
#ifdef FASTWARP_NO_QUEST
        // Skip if quest not started
        mov eax, ds:[QUEST_STARTED]
        test eax, eax
        je do_skip

        // Don't skip if quest has finished loading
        mov eax, dword ptr [questDoneLoading]
        test eax, eax
        jne dont_skip
#endif

    do_skip:
        // Quest has not finished loading (or hasn't started loading)
        mov dword ptr [questDoneLoading], 0
        // To skip most other loading screens, check if the render function is being called from a certain asset loading function
        mov dword ptr [skipSleep], 1
    dont_skip:
        call renderFuncAddr
        mov dword ptr [skipSleep], 0
        jmp assetLoadPatchOutAddr
    }
}

void __declspec(naked) PatchInQuest()
{
    __asm
    {
        mov dword ptr [questDoneLoading], 1
        call questNotLoadingFuncAddr
        jmp questDoneLoadingOutAddr
    }
}

#undef LOGIN_CONNECTION_MANAGER
#undef QUEST_STARTED

void ApplyFastWarpPatch()
{
    // Replaces a useless call to a nop function
    PatchJMP(renderSkipInAddr, renderSkipOutAddr, (int) &CheckRenderSkip);

    // Makes asset loading fast
    PatchJMP(assetLoadPatchInAddr, assetLoadPatchOutAddr, (int) &PatchAllAssetLoading);

#ifdef FASTWARP_NO_QUEST
    // Sets a variable when quest loading has finished
    PatchJMP(questDoneLoadingInAddr, questDoneLoadingOutAddr, (int) &PatchInQuest);
#endif
}
