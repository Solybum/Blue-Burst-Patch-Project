#include "map_object.h"
#include "object_extension.h"
#include <cstdint>

namespace MapObject
{
    void** rootMapObject = reinterpret_cast<void**>(0x00aca350);

    MapObjectBase::MapObjectBase(void* parentObject)
    {
        Constructor(this, parentObject);

        vtable = new Vtable(*origVtable);

        OVERRIDE_METHOD(MapObjectBase, Destruct);
    }

    void MapObjectBase::Destruct(bool32 freeMemory)
    {
        // Superdestructors will reassign the vtable so it's fine to delete our vtable here
        delete vtable;
        vtable = nullptr;

        // Recursively call superdestructors, without letting them deallocate
        origVtable->Destruct(this, false);

        // We can deallocate our own memory if this object was not derived further
        if (freeMemory)
        {
            MainArenaDealloc(this);
        }
    }

    const MapObjectBase::ConstructorFunction MapObjectBase::Constructor = reinterpret_cast<MapObjectBase::ConstructorFunction>(0x00770748);
    const MapObjectBase::Vtable* MapObjectBase::origVtable = reinterpret_cast<Vtable*>(0x00b441c0);

    /// Each map has a list of objects that can be spawned in it. Index is map.
    static TaggedMapObjectConstructor** mapObjectTable = reinterpret_cast<TaggedMapObjectConstructor**>(0x00a06be0);

    TaggedMapObjectConstructor::TaggedMapObjectConstructor(uint16_t type, CreationFunction ctor) :
        objectType(type), createObject(ctor), unknown1(0), constructionDistance(250000.0), unknown2(0) {}

    TaggedMapObjectConstructor::TaggedMapObjectConstructor() :
        objectType(0), createObject(nullptr), unknown1(0), constructionDistance(250000.0), unknown2(0) {}

    /// Save used lists here
    static std::unordered_map<Map::MapType, std::vector<TaggedMapObjectConstructor>> objectConstructorListCache;

    std::vector<TaggedMapObjectConstructor>& GetMapObjectConstructorList(Map::MapType map)
    {
        // Get cached or read new
        auto found = objectConstructorListCache.find(map);

        if (found != objectConstructorListCache.end())
        {
            return (*found).second;
        }

        std::vector<TaggedMapObjectConstructor> objectList;
        auto entries = mapObjectTable[(size_t) map];

        while (entries->objectType != 0xffff)
        {
            objectList.push_back(*entries);
            entries++;
        }

        objectConstructorListCache[map] = objectList;

        return objectConstructorListCache[map];
    }

    static const TaggedMapObjectConstructor objectListTerminator = TaggedMapObjectConstructor(0xffff, nullptr);

    bool patchedConstructorLists = false;

    void PatchMapObjectConstructorLists()
    {
        if (patchedConstructorLists) return;

        patchedConstructorLists = true;

        // Write used lists
        for (const auto& entry : objectConstructorListCache)
        {
            auto map = entry.first;
            auto objectList = entry.second;

            // Add terminator
            objectList.push_back(objectListTerminator);

            // Copy to heap (deliberate leak) and write new list to table
            TaggedMapObjectConstructor* copy = new TaggedMapObjectConstructor[objectList.size()];
            std::copy(objectList.begin(), objectList.end(), copy);
            mapObjectTable[(size_t) map] = copy;
        }
    }
};
