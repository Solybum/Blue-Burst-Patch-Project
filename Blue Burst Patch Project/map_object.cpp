#include "map_object.h"
#include "object_extension.h"
#include <cstdint>

namespace MapObject
{
    SpawnableDefinition::SpawnableDefinition(MapObjectType id, size_t loadAssetsAddr, size_t unloadAssetsAddr, size_t createAddr)
        : id((uint16_t) id),
          LoadAssets(reinterpret_cast<decltype(LoadAssets)>(loadAssetsAddr)),
          UnloadAssets(reinterpret_cast<decltype(UnloadAssets)>(unloadAssetsAddr)),
          Create(reinterpret_cast<decltype(Create)>(createAddr)) {}

    SpawnableDefinition::SpawnableDefinition(uint16_t id,
            decltype(LoadAssets) LoadAssets,
            decltype(UnloadAssets) UnloadAssets,
            decltype(Create) Create)
        : id(id),
          LoadAssets(LoadAssets),
          UnloadAssets(UnloadAssets),
          Create(Create) {}
    
    const std::unordered_map<MapObjectType, SpawnableDefinition>& GetMapObjectDefinitions()
    {
        static std::unordered_map<MapObjectType, SpawnableDefinition> mapObjectDefinitions =
        {
            {MapObjectType::PlayerSet1, SpawnableDefinition(MapObjectType::PlayerSet1, 0, 0, 0x00661728)},
            {MapObjectType::FloatingJelifish, SpawnableDefinition(MapObjectType::FloatingJelifish, 0x0062cfa8, 0x0062c190, 0x0062c1b0)}
        };
        return mapObjectDefinitions;
    }
    
    const SpawnableDefinition& GetMapObjectDefinition(MapObjectType tp)
    {
        return GetMapObjectDefinitions().at(tp);
    }

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

    static std::vector<TaggedMapObjectConstructor*> patchedLists;

    void PatchMapObjectConstructorLists()
    {
        // Write used lists
        for (auto& [map, objectList] : objectConstructorListCache)
        {
            // Add terminator
            if (objectList.empty() || objectList[objectList.size() - 1].objectType != objectListTerminator.objectType)
                objectList.push_back(objectListTerminator);

            // Write new list to table
            mapObjectTable[(size_t) map] = objectList.data();
        }
    }
};
