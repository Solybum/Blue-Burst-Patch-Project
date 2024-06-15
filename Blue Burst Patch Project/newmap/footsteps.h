#pragma once

#include <cstdint>
#include <string>

void ReplaceMapFootsteps(uint8_t origMap, const std::string& footstepSfxFilePath);
void RestoreMapFootsteps(uint8_t origMap);
