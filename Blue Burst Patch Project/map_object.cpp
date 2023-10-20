#include "map_object.h"
#include "object_extension.h"

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
};
