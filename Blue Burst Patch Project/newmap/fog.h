#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

#pragma pack(push, 1)
struct FogEntry
{
    uint32_t type;
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    } color;
    float end;
    float start;
    float density;
    uint32_t unk1;
    float unk2;
    float unk3;
    float end_pulse_distance;
    uint32_t unk4;
    float start_pulse_distance;
    uint32_t unk5;
    float transition;
    uint32_t unk6;
    uint8_t unk7;
    uint8_t unk8;
    uint8_t lerped_field6;
    uint8_t unk9;
    uint8_t lerped_field7;
    uint8_t unk10;
    uint8_t lerped_field8;
    uint8_t unk11;
};
#pragma pack(pop)

extern FogEntry** fogEntries;

FogEntry ReadFogFile(const std::string& path);
