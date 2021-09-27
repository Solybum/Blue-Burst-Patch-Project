#include <cstdint>
#include "pch.h"
#include "helpers.h"
#include "slow_gibbles.h"

const size_t gibblesBpId = 0x3d;
const size_t bpAnimSpeedSz = 0x30;
// Actually a pointer to a struct of size 0x30 but let's just call it a float pointer
float** bpAnimSpeeds = (float**) 0x00a9b1d0;
float* gibblesJumpAnimSpeed = (float*) 0x009be1e8;

float* getBpAnimationSpeeds(size_t id)
{
    return (*bpAnimSpeeds) + id * bpAnimSpeedSz / sizeof(float);
}

void __cdecl SlowGibblesFix()
{
    // BP should be loaded at this point but just in case
    if (*bpAnimSpeeds == nullptr) return;

    // Restore jump speed
    *gibblesJumpAnimSpeed = *getBpAnimationSpeeds(gibblesBpId);
}

void ApplySlowGibblesFix()
{
    // Append to a function table that is called on party join/create
    *((void**) 0x00a0f630) = &SlowGibblesFix;
    // Adjust size of function table
    *((uint8_t*) 0x007bd159) = 0x58;
}
