#pragma once

#include <string>

#pragma pack(push, 1)
struct LightEntry
{
    float x1;
    float y1;
    float z1;
    float x2;
    float y2;
    float z2;
    float intensity_specular;
    float intensity_diffuse;
    float intensity_ambient;
    float diffuse_a;
    float diffuse_r;
    float diffuse_g;
    float diffuse_b;
    float ambient_a;
    float ambient_r;
    float ambient_g;
    float ambient_b;
};
#pragma pack(pop)

extern LightEntry** lightEntries;

LightEntry ReadLightFile(const std::string& path);
