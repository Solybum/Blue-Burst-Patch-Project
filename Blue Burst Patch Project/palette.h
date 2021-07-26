#pragma once

static int addrSecondaryPaletteAttack1 = 0x0068A739;
static unsigned char valSecondaryPaletteAttack1 = 0xEB;

static int addrSecondaryPaletteAttack2 = 0x006A114F;
static unsigned char valSecondaryPaletteAttack2 = 0x00;

static int addrSecondaryPaletteAttack3 = 0x006A0C4F;
static unsigned char valSecondaryPaletteAttack3 = 0x00;

static int addrSetPaletteHotkey = 0x0068CDE0;

static int addrGetCurrentPaletteI = 0x00748944;
static int addrGetCurrentPaletteO = 0x0074894C;

static int addrCheckHotkey1_1I = 0x00748992;
static int addrCheckHotkey1_1O = 0x00748997;
static int addrCheckHotkey1_2I = 0x007489a1;
static int addrCheckHotkey1_2O = 0x007489a6;
static int addrUnsetHotkey1I = 0x007489b1;
static int addrUnsetHotkey1O = 0x007489d0;

static int addrCheckHotkey2_1I = 0x00748A3C;
static int addrCheckHotkey2_1O = 0x00748A41;
static int addrCheckHotkey2_2I = 0x00748A4B;
static int addrCheckHotkey2_2O = 0x00748A50;
static int addrUnsetHotkey2I = 0x00748A57;
static int addrUnsetHotkey2O = 0x00748A76;

static int addrSetHotkeyI = 0x00748ABE;
static int addrSetHotkeyO = 0x00748AEC;

static int addrCheckHotkey3_1I = 0x007103B7;
static int addrCheckHotkey3_1O = 0x007103BC;
static int addrCheckHotkey3_2I = 0x007103C0;
static int addrCheckHotkey3_2O = 0x007103C5;

void GetCurrentPalette();
void CheckHotkey1_1();
void CheckHotkey1_2();
void UnsetHotkey1();
void CheckHotkey2_1();
void CheckHotkey2_2();
void UnsetHotkey2();
void SetHotkey();
void CheckHotkey3_1();
void CheckHotkey3_2();

void PatchPalette();
