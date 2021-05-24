#pragma once

#define GIBBLES_ANIMATION_SPEEDS 0x00a73ce4 // float*
#define GIBBLES_JUMP_ANIMATION_SPEED 0x009be1e8 // float

static int addrSlowGibblesFixI = 0x007bcdb9;
static int addrSlowGibblesFixO = 0x007bcdbe;
static int addrSlowGibblesFixJMP = 0x007bcdbe;

void ApplySlowGibblesFix();
