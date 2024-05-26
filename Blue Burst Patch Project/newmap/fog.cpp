#ifdef PATCH_NEWMAP

#include "fog.h"
#include "../helpers.h"

FogEntry** fogEntries = reinterpret_cast<FogEntry**>(0x00a8d770);

FogEntry ReadFogFile(const std::string& path)
{
    // Read fog settings from file
    auto lines = ReadCsvFile(path);
    auto& splitParts = lines[0];
    
    auto parseFloat = [&](size_t idx) {
        return std::strtof(splitParts[idx].c_str(), nullptr);
    };
    auto parseUint = [&](size_t idx) {
        return std::strtoul(splitParts[idx].c_str(), nullptr, 10);
    };
    
    FogEntry fog = {
        .type = parseUint(0),
        .color = {
            .b = (uint8_t) parseUint(3),
            .g = (uint8_t) parseUint(2),
            .r = (uint8_t) parseUint(1),
            .a = (uint8_t) parseUint(4)
        },
        .end = parseFloat(5),
        .start = parseFloat(6),
        .density = parseFloat(7),
        .unk1 = parseUint(8),
        .unk2 = parseFloat(9),
        .unk3 = parseFloat(10),
        .end_pulse_distance = parseFloat(11),
        .unk4 = parseUint(12),
        .start_pulse_distance = parseFloat(13),
        .unk5 = parseUint(14),
        .transition = parseFloat(15),
        .unk6 = parseUint(16),
        .unk7 = (uint8_t) parseUint(17),
        .unk8 = (uint8_t) parseUint(18),
        .lerped_field6 = (uint8_t) parseUint(19),
        .unk9 = (uint8_t) parseUint(20),
        .lerped_field7 = (uint8_t) parseUint(21),
        .unk10 = (uint8_t) parseUint(22),
        .lerped_field8 = (uint8_t) parseUint(23),
        .unk11 = (uint8_t) parseUint(24),
    };
    
    return fog;
}

#endif // PATCH_NEWMAP
