#pragma once

#include <cstdint>
#include <string>
#include <vector>

#pragma pack(push, 1)
struct SlbgmTransitionPart
{
    uint16_t startIndex;
    uint16_t length;
    uint16_t transitionIn;
    uint16_t transitionOut;
    uint16_t nextPart;
};

struct SlbgmDef
{
    char name[32];
    uint32_t partCount;
    uint32_t unk1;
    uint32_t unk2;
    SlbgmTransitionPart* track1;
    SlbgmTransitionPart* track2;
    
    SlbgmDef(const std::string& songFilename, const std::string& definitionFilePath);
    ~SlbgmDef();
};
#pragma pack(pop)

std::vector<SlbgmDef*>& GetSlbgmMetadata();
void ApplySlbgmPatch();
extern void (__cdecl *LoadSlbgm)(size_t idx);
