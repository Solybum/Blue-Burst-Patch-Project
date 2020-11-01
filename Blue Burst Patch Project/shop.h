#pragma once

static int addrNoSellUntekkedWeapon = 0x005D26F1;
static int addrNoSellRareWeapon = 0x005D2706;
static int addrNoSellRareArmor = 0x005D25AF;

static int addrNoSellRareToolI = 0x005D2528;
static int addrNoSellRareToolO = 0x005D252D;
static int addrNoSellRareToolJMP = 0x005D2576;

void NoSellRareTool();

void PatchShop();
