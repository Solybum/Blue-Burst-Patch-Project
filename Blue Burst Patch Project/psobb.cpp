#include "pch.h"
#include "globals.h"
#include "shop.h"
#include "earlywalk.h"
#include "slow_gibbles.h"

void PSOBB()
{
#ifdef PATCH_DISABLE_GAMEGUARD
    memset((void*)addrMainGameGuardCall, 0x90, 0x05);
#endif

#ifdef PATCH_EARLY_WALK_FIX
    ApplyEarlyWalkFix();
#endif

    PatchShop();

#ifdef PATCH_SLOW_GIBBLES_FIX
    ApplySlowGibblesFix();
#endif
}
