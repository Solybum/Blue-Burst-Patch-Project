#include <stdint.h>

#include "pch.h"
#include "globals.h"

#ifdef PATCH_IME
#include "ime.h"
#endif

#ifdef PATCH_UNSELLABLE_RARES
#include "shop.h"
#endif

#ifdef PATCH_EARLY_WALK_FIX
#include "earlywalk.h"
#endif

#ifdef PATCH_KEYBOARD_ALTERNATE_PALETTE
#include "palette.h"
#endif

#ifdef PATCH_SLOW_GIBBLES_FIX
#include "slow_gibbles.h"
#endif

#ifdef PATCH_CUSTOMIZE_MENU
#include "customize_menu.h"
#endif

#ifdef PATCH_FASTWARP
#include "fastwarp.h"
#endif

#ifdef PATCH_OMNISPAWN
#include "omnispawn.h"
#endif

#ifdef PATCH_NEWENEMY
#include "newenemy.h"
#endif

#ifdef PATCH_LARGE_ASSETS
#include "large_assets.h"
#endif

#ifdef PATCH_ENEMY_CONSTRUCTOR_LISTS
#include "enemy.h"
#endif

#ifdef PATCH_INITLISTS
#include "initlist.h"
#endif

void PSOBB()
{
    // By default, keep the game guard patch enabled
#ifndef DO_NOT_PATCH_DISABLE_GAMEGUARD
    memset((void*)addrMainGameGuardCall, 0x90, 0x05);
#endif

#ifdef PATCH_EARLY_WALK_FIX
    ApplyEarlyWalkFix();
#endif

#ifdef PATCH_KEYBOARD_ALTERNATE_PALETTE
    PatchPalette();
#endif

#ifdef PATCH_SLOW_GIBBLES_FIX
    ApplySlowGibblesFix();
#endif

#ifdef PATCH_CUSTOMIZE_MENU
    CustomizeMenu::ApplyActionListPatch();
#endif

#ifdef PATCH_UNSELLABLE_RARES
    PatchShop();
#endif

#ifdef PATCH_IME
    PatchIme();
#endif

#ifdef PATCH_FASTWARP
    ApplyFastWarpPatch();
#endif

#ifdef PATCH_SKIP_INTRO_CREDITS
    *(uint8_t*)0x007a645e = 2;
#endif

#ifdef PATCH_OMNISPAWN
    Omnispawn::ApplyOmnispawnPatch();
#endif

#ifdef PATCH_NEWENEMY
    ApplyNewEnemyPatch();
#endif

#ifdef PATCH_LARGE_ASSETS
    ApplyLargeAssetsPatch();
#endif

#ifdef PATCH_ENEMY_CONSTRUCTOR_LISTS
    Enemy::PatchEnemyConstructorLists();
#endif

#ifdef PATCH_INITLISTS
    // Should be last so that other patches can apply their changes first
    InitList::PatchAllInitLists();
#endif
}
