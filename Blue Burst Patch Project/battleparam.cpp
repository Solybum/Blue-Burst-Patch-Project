#include <cstddef>
#include "battleparam.h"

namespace BattleParam
{
    /// The currently loaded BP file
    BPFile** loadedBP = reinterpret_cast<BPFile**>(0x00a9b1e0);
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

    void* GetBPEntry(Episode ep, bool solo_mode, BPIndex i, BPEntryType entryType)
    {
        // Use currently loaded BP if possible, because it's a bit faster
        BPFile* bp = *loadedBP;

        if (ep != GetCurrentEpisode() || solo_mode != IsSoloMode())
        {
            // Accessing a BP file that is not currently loaded
            bp = GetBPFile(ep, solo_mode);
        }

        size_t asNumber = static_cast<size_t>(i);

        switch (entryType)
        {
            case BPEntryType::Stats:
                return &bp->stats[asNumber];
            case BPEntryType::Attacks:
                return &bp->attacks[asNumber];
            case BPEntryType::Resists:
                return &bp->resists[asNumber];
            case BPEntryType::Animations:
                return &bp->animations[asNumber];
        }
    }
};
