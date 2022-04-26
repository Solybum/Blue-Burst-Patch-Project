#include <stdint.h>
#include <vector>
#include "large_assets.h"

void ApplyLargeAssetsPatch()
{
    uint32_t newsize = 100000000;

    std::vector<uint32_t> addrs = {
        0x00800c31 + 1,
        0x005b7cfa + 2,
        0x005b80f6 + 2,
        0x005b913e + 1,
        0x005b7214 + 1,
        0x005b7936 + 1,
        0x005b97c1 + 1,
        0x005ba612 + 1,
        0x005bb404 + 1,
        0x005b77e1 + 2,
        0x005c74bf + 2,
        0x0070eb59 + 2,
        0x00800a32 + 2,
        0x005b7cfa + 2,
        0x005b82ac + 1,
        0x005bb40b + 1,
        0x005e581d + 1,
        0x007a6572 + 1
    };

    for (auto addr : addrs)
    {
        *(uint32_t*) addr = newsize;
    }
}
