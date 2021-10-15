#include <vector>
#include "map.h"

namespace Map
{
    MapInitFunctionPair** mapInitFunctionTable = reinterpret_cast<MapInitFunctionPair**>(0x009fcae0);

    std::vector<MapInitFunctionPair> ReadEntriesIntoMapInitList(MapInitFunctionPair* entries)
    {
        std::vector<MapInitFunctionPair> initList;

        while (*entries->init != nullptr && *entries->uninit != nullptr)
        {
            initList.push_back(*entries);
            entries++;
        }

        return initList;
    }

    MapInitFunctionPair* CopyMapInitFunctionListToHeap(const std::vector<MapInitFunctionPair>& initList)
    {
        MapInitFunctionPair* copy = new MapInitFunctionPair[initList.size()];
        std::copy(initList.begin(), initList.end(), copy);
        return copy;
    }

    bool initListPatchApplied = false;

    void ApplyMapInitListPatch()
    {
        if (initListPatchApplied) {
            return;
        }

        // As a test, append all previous map init function lists to the falz area init list
        auto falzFuncs = ReadEntriesIntoMapInitList(mapInitFunctionTable[(size_t) MapType::Boss_Darkfalz]);

        for (size_t i = 0; i < (size_t) MapType::Boss_Darkfalz; i++) {
            auto list = ReadEntriesIntoMapInitList(mapInitFunctionTable[i]);
            falzFuncs.insert(falzFuncs.end(), list.begin(), list.end());
        }

        falzFuncs.push_back(mapInitListTerminator);
        mapInitFunctionTable[(size_t) MapType::Boss_Darkfalz] = CopyMapInitFunctionListToHeap(falzFuncs);

        initListPatchApplied = true;
    }
};
