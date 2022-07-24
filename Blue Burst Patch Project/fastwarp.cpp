#ifdef PATCH_FASTWARP

#include "helpers.h"
#include "fastwarp.h"

/*
This patch works by skipping the sleep portion of the render function
during loading screens to allow the game to load as fast as possible.
*/

auto InnerRenderFunction = reinterpret_cast<void (__fastcall *)(BOOL)>(0x0083ae04);
auto OuterRenderFunction = reinterpret_cast<void (__cdecl *)()>(0x0080030c);
auto SleepLoopFunction = reinterpret_cast<void (__cdecl *)()>(0x0082c43c);

auto skipFrame = reinterpret_cast<BOOL*>(0x00aca200);
auto loginManager = reinterpret_cast<void**>(0x00a3ad08);
auto isInQuest = reinterpret_cast<BOOL*>(0x00a95624);
auto isQuestLoading = reinterpret_cast<BOOL*>(0x00aab378);
bool skipNextFrameAlso = false;

bool IsLoginLoadingScreenActive()
{
    return *loginManager != nullptr;
}

void __fastcall BeforeInnerRenderCall(BOOL shouldPresent)
{
    if (IsLoginLoadingScreenActive())
    {
        *skipFrame = true;
        shouldPresent = false;
        // Also skip the next frame because the loading screen is up for one frame after login finishes
        // and showing only one frame of it looks ugly
        skipNextFrameAlso = true;
    }
    else if (skipNextFrameAlso)
    {
        *skipFrame = true;
        shouldPresent = false;
        skipNextFrameAlso = false;
    }

    // Call the original function
    InnerRenderFunction(shouldPresent);
}

void __cdecl BeforeAssetLoadingRenderCall()
{
    // Setting skipFrame here will also cause the shouldPresent parameter for the inner render function to be false
#ifdef FASTWARP_NO_QUEST
    // During a quest, the quest loading screen will be skipped but not any other loading screens
    *skipFrame = !*isInQuest || *isQuestLoading;
#else
    *skipFrame = true;
#endif

    // Call the original function
    OuterRenderFunction();
}

void __cdecl BeforeSleepLoopCall()
{
    if (*skipFrame)
    {
        *skipFrame = false;
        return;
    }

    // Call the original function
    SleepLoopFunction();
}

void ApplyFastWarpPatch()
{
    // Handles skipping the login loading screen
    PatchCALL(0x00800347, 0x0080034c, (int) BeforeInnerRenderCall);

    // Handles skipping most other loading screens such as warping between areas and loading a quest
    PatchCALL(0x005b92d9, 0x005b92de, (int) BeforeAssetLoadingRenderCall);

    // Handles skipping the sleep loop within the render function
    PatchCALL(0x0083ae47, 0x0083ae4c, (int) BeforeSleepLoopCall);
}

#endif // PATCH_FASTWARP
