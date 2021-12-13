#include <cstdint>
#include "pch.h"
#include "helpers.h"
#include "slow_gibbles.h"

float* gibblesJumpAnimSpeed = (float*) 0x009be1e8;

void __cdecl SlowGibblesFix()
{
    // Restore jump speed
    *gibblesJumpAnimSpeed = 0.7;
}

void ApplySlowGibblesFix()
{
    // Append to a function table that is called on party join/create
    *((void**) 0x00a0f630) = &SlowGibblesFix;
    // Adjust size of function table
    *((uint8_t*) 0x007bd159) = 0x58;
}
