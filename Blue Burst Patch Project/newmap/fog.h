#pragma once

#include <cstdint>
#include <string>

void ReplaceMapFog(uint8_t origMap, const std::string& fogFilePath);
void RestoreMapFog(uint8_t origMap);
