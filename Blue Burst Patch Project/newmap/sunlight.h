#pragma once

#include <cstdint>
#include <string>

void ReplaceMapSunlight(uint8_t origMap, const std::string& sunlightFilePath);
void RestoreMapSunlight(uint8_t origMap);
