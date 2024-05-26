#ifdef PATCH_NEWMAP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "slbgm.h"
#include "../helpers.h"

namespace Slbgm
{
    // Loading this struct from a text file
    SlbgmDef::SlbgmDef(const std::string& songFilename, const std::string& definitionFilePath)
    {
        std::strncpy(name, songFilename.c_str(), 32);
        name[31] = '\0';
        
        std::vector<SlbgmTransitionPart> track1Parts;
        std::vector<SlbgmTransitionPart> track2Parts;
        auto lines = ReadCsvFile(definitionFilePath);

        for (const auto& splitParts : lines)
        {
            // Add transition part
            // track_index, stream_index, transition_in, part_length, transition_out, next_part
            auto& transPart = std::strtoul(splitParts[0].c_str(), nullptr, 10) == 0 ? track1Parts.emplace_back() : track2Parts.emplace_back();
            transPart.startIndex = std::strtoul(splitParts[1].c_str(), nullptr, 10);
            transPart.transitionIn = std::strtoul(splitParts[2].c_str(), nullptr, 10);
            transPart.length = std::strtoul(splitParts[3].c_str(), nullptr, 10);
            transPart.transitionOut = std::strtoul(splitParts[4].c_str(), nullptr, 10);
            transPart.nextPart = std::strtoul(splitParts[5].c_str(), nullptr, 10);
        }
        
        // Put track parts on the heap
        // XXX: Is it fine to leak these?
        track1 = new SlbgmTransitionPart[track1Parts.size()];
        track2 = new SlbgmTransitionPart[track2Parts.size()];
        
        std::copy(track1Parts.begin(), track1Parts.end(), track1);
        std::copy(track2Parts.begin(), track2Parts.end(), track2);
        
        partCount = track1Parts.size();
        unk1 = 0;
        unk2 = 0;
    }
    
    // Copy of original array that can be modified
    static std::vector<SlbgmDef*> slbgmDefinitions = []() {
        auto originalDefinitions = reinterpret_cast<SlbgmDef**>(0x00a16088);
        size_t originalDefinitionsCount = 0;
        for (auto cursor = originalDefinitions; *cursor != nullptr; cursor++)
            originalDefinitionsCount++;
            
        return std::vector(originalDefinitions, originalDefinitions + originalDefinitionsCount);
    }();

    std::vector<SlbgmDef*>& GetAllTransitionData()
    {
        return slbgmDefinitions;
    }

    void ApplySlbgmPatch()
    {
        // Replace old reference with new array
        *reinterpret_cast<SlbgmDef***>(0x00814cb8 + 3) = slbgmDefinitions.data();
    }
    
    decltype(LoadSlbgm) LoadSlbgm = reinterpret_cast<decltype(LoadSlbgm)>(0x00814c7c);
};

#endif // PATCH_NEWMAP
