#include "pch.h"
#include "map.h"

namespace Map
{
#ifdef PATCH_INITLISTS
    InitList& GetMapInitList(MapType map)
    {
        InitList::FunctionPair** mapInitLists = reinterpret_cast<InitList::FunctionPair**>(0x009fcae0);
        InitList& lst = InitList::GetNullTerminatedInitList(mapInitLists[(size_t) map]);
        lst.AddListReferenceAddress(&mapInitLists[(size_t) map]);
        return lst;
    }
#endif // PATCH_INITLISTS
};
