#include "pch.h"
#include "globals.h"
#include "palette.h"
#include "shop.h"
#include "earlywalk.h"
#include "slow_gibbles.h"
#include "customize_menu.h"
#include "ime.h"
#include "fastwarp.h"
#include "omnispawn.h"
#include "initlist.h"
#include "enemy.h"

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

    PatchShop();

    PatchIme();

#ifdef PATCH_FASTWARP
    ApplyFastWarpPatch();
#endif

#ifdef PATCH_OMNISPAWN
    Omnispawn::ApplyOmnispawnPatch();
#endif

#ifdef PATCH_ENEMY_CONSTRUCTOR_LISTS
    Enemy::PatchEnemyConstructorLists();
#endif

#ifdef PATCH_INITLISTS
    // Should be last so that other patches can apply their changes first
    InitList::PatchAllInitLists();
#endif
}
