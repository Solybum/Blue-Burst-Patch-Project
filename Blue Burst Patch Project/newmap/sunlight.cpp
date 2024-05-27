#ifdef PATCH_NEWMAP

#include "sunlight.h"
#include "../helpers.h"

LightEntry** lightEntries = reinterpret_cast<LightEntry**>(0x00a9d4e4);

LightEntry ReadLightFile(const std::string& path)
{
    // Read sunlight settings from file
    auto lines = ReadCsvFile(path);
    auto& splitParts = lines[0];
    
    auto parseFloat = [&](size_t idx) {
        return std::strtof(splitParts[idx].c_str(), nullptr);
    };
    
    LightEntry light = {
        .x1 = parseFloat(0),
        .y1 = parseFloat(1),
        .z1 = parseFloat(2),
        .x2 = parseFloat(3),
        .y2 = parseFloat(4),
        .z2 = parseFloat(5),
        .intensity_specular = parseFloat(6),
        .intensity_diffuse = parseFloat(7),
        .intensity_ambient = parseFloat(8),
        .diffuse_a = parseFloat(9),
        .diffuse_r = parseFloat(10),
        .diffuse_g = parseFloat(11),
        .diffuse_b = parseFloat(12),
        .ambient_a = parseFloat(13),
        .ambient_r = parseFloat(14),
        .ambient_g = parseFloat(15),
        .ambient_b = parseFloat(16)
    };
    
    return light;
}

#endif // PATCH_NEWMAP
