#include <cstdint>
#include "pch.h"
#include "helpers.h"
#include "slow_gibbles.h"
#include "initlist.h"

float* gibblesJumpAnimSpeed = (float*) 0x009be1e8;

void __cdecl SlowGibblesFix()
{
    // Restore jump speed
    *gibblesJumpAnimSpeed = 0.7;
}

void ApplySlowGibblesFix()
{
    // Append to a function table that is called on party join/create
    InitList& lst = InitList::GetInitList(0x00a0f5e0, 0x00a0f630);
    lst.AddFunctionPair(InitList::FunctionPair(SlowGibblesFix, nullptr));
    lst.AddSizeReferenceAddress({0x007bcb80 + 1, 0x007bd158 + 1, 0x007bd558 + 1});
    lst.AddListReferenceAddress({0x007bcb82 + 1, 0x007bd15a + 1, 0x007bd55a + 1});
}
