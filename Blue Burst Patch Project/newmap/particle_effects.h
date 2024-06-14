#pragma once

#include <cstdint>
#include <string>

void ReplaceMapParticleEffects(uint8_t origMap, const std::string& particleFilePath);
void RestoreMapParticleEffects(uint8_t origMap);
