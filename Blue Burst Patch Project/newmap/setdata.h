#pragma once

#include <cstdint>

#pragma pack(push, 1)
struct SetDataTable
{
    struct Inner1 {
        struct Inner2 {
            const char* variantName1;
            const char* variantName2;
            const char* baseName;
        }* objectSetVariants;
        uint32_t count;
    }* mapVariants;
    uint32_t count;
};
#pragma pack(pop)

void ReplaceSetData(uint8_t origMap, const SetDataTable& setData);
void RestoreSetData(uint8_t origMap);
