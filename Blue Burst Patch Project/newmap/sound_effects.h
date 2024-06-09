#pragma once

#include <cstdint>
#include <string>

void ReplaceMapPac(uint8_t origMap, const std::string& pacFilename, const std::string& configFilename);
void RestoreMapPac(uint8_t origMap);
void PatchSoundEffectLimit();
