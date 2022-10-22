#include <windows.h>
#include <cstddef>
#include <stdexcept>
#include "battleparam.h"

namespace BattleParam
{
    /// The currently loaded BP file
    BPDifficultyFile** loadedBP = reinterpret_cast<BPDifficultyFile**>(0x00a9b1e0);
    char** bpFilenames = reinterpret_cast<char**>(0x009f7dec);

    void* paramFileStore = reinterpret_cast<void*>(0x00a8d4e0);

    // Load some functions from the game instead of reimplementing them
    // TODO: Move these to a separate file
    typedef uint8_t* (__thiscall *GetParamFileDataFunction)(void* paramFileStore, char* filename);
    GetParamFileDataFunction GetParamFileData = reinterpret_cast<GetParamFileDataFunction>(0x005bc0b0);

    typedef size_t (__thiscall *GetParamFileSizeFunction)(void* paramFileStore, char* filename);
    GetParamFileSizeFunction GetParamFileSize = reinterpret_cast<GetParamFileSizeFunction>(0x005bc050);

    char* GetBPFilename(Episode ep, bool solo_mode)
    {
        size_t i = static_cast<size_t>(ep) * 2;

        if (solo_mode)
        {
            i++;
        }

        return bpFilenames[i];
    }

    // TODO: Memoize? The file does not change after login, I think.
    BPFile* GetBPFile(Episode ep, bool solo_mode)
    {
        char* filename = GetBPFilename(ep, solo_mode);

        size_t size = GetParamFileSize(paramFileStore, filename);

        if (size != sizeof(BPFile))
        {
            return nullptr;
        }

        uint8_t* file = GetParamFileData(paramFileStore, filename);

        if (file == nullptr)
        {
            return nullptr;
        }

        return reinterpret_cast<BPFile*>(file);
    }

    void* GetBPEntry(Episode ep, bool solo_mode, uint8_t i, BPEntryType entryType)
    {
        // Use currently loaded BP if possible, because it's a bit faster
        BPDifficultyFile* bpDiff = *loadedBP;

        if (bpDiff == nullptr || ep != GetCurrentEpisode() || solo_mode != IsSoloMode())
        {
            // Accessing a BP file that is not currently loaded
            BPFile* bpFull = GetBPFile(ep, solo_mode);
            size_t difficulty = (size_t) GetCurrentDifficulty();

            switch (entryType)
            {
                case BPEntryType::Stats:
                    return &bpFull->stats[difficulty][i];
                case BPEntryType::Attacks:
                    return &bpFull->attacks[difficulty][i];
                case BPEntryType::Resists:
                    return &bpFull->resists[difficulty][i];
                case BPEntryType::Animations:
                    return &bpFull->animations[difficulty][i];
            }

            throw std::runtime_error("Unreachable");
        }

        switch (entryType)
        {
            case BPEntryType::Stats:
                return &bpDiff->stats[i];
            case BPEntryType::Attacks:
                return &bpDiff->attacks[i];
            case BPEntryType::Resists:
                return &bpDiff->resists[i];
            case BPEntryType::Animations:
                return &bpDiff->animations[i];
        }

        // Appease the compiler
        throw std::runtime_error("Unreachable");
    }
};
