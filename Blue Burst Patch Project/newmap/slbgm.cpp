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

namespace Slbgm
{
    // Loading this struct from a text file
    SlbgmDef::SlbgmDef(const std::string& songFilename, const std::string& definitionFilePath)
    {
        std::strncpy(name, songFilename.c_str(), 32);
        name[31] = '\0';
        
        std::ifstream file;
        file.open(definitionFilePath);
        
        if (file.fail())
            throw std::runtime_error("Failed to open " + definitionFilePath);
        
        std::vector<SlbgmTransitionPart> track1Parts;
        std::vector<SlbgmTransitionPart> track2Parts;
        
        // Read lines, each line is one transition part.
        // A line is a list of comma separated values, the values are as follows:
        // track_index, stream_index, transition_in, part_length, transition_out, next_part
        std::string line;
        while (std::getline(file, line))
        {
            // Split line and collect into vector of ints
            std::istringstream lineStream(line);
            std::string splitPart;
            std::vector<uint16_t> splitValues;
            splitValues.resize(6);
            size_t i = 0;
            while (std::getline(lineStream, splitPart, ','))
            {
                uint16_t value = std::strtoul(splitPart.data(), nullptr, 10);
                splitValues[i++] = value;
            }
            
            // Add transition part
            auto& transPart = splitValues[0] == 0 ? track1Parts.emplace_back() : track2Parts.emplace_back();
            transPart.startIndex = splitValues[1];
            transPart.transitionIn = splitValues[2];
            transPart.length = splitValues[3];
            transPart.transitionOut = splitValues[4];
            transPart.nextPart = splitValues[5];
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
