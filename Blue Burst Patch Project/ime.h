#pragma once

static int addrImeCall = 0x00841386;
//static int valImePatch = 0x008F83A8; // Original
static int valImePatch = 0x008EC39C; // Patch

void PatchIme();
