#ifdef PATCH_NEWMAP

#include <cstdint>
#include <unordered_map>

#include "setdata.h"

static SetDataTable** setDataTable = reinterpret_cast<SetDataTable**>(0x00aafdd0);
static std::unordered_map<uint8_t, SetDataTable> replacedSetData;

void ReplaceSetData(uint8_t origMap, const SetDataTable& setData)
{
    replacedSetData[origMap] = (*setDataTable)[origMap];
    (*setDataTable)[origMap] = setData;
}

void RestoreSetData(uint8_t origMap)
{
    (*setDataTable)[origMap] = replacedSetData[origMap];
    replacedSetData.erase(origMap);
}

#endif
