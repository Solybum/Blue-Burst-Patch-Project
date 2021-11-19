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
};
