#pragma once

#include <cstdint>
#include "object.h"

/// Defines a method that returns a reference to an offset in the object
#define DECLARE_WRAPPED_MEMBER(offset, type, name) \
    volatile type& name() { return *reinterpret_cast<volatile type*>(reinterpret_cast<uint8_t*>(obj) + offset); }

/// Defines a method that calls a function in the object's vtable at the specified index
#define DECLARE_WRAPPED_METHOD(index, ret_type, name, ...) \
    template<typename... Args> \
    ret_type name(Args... args) { \
        typedef ret_type (__thiscall *method)(BaseObject*, __VA_ARGS__); \
        return reinterpret_cast<method>(obj->vtable[index])(obj, args...); \
    }

/// Inherit this class to enable interacting with existing game objects.
/// Use DECLARE_WRAPPER_MEMBER and DECLARE_WRAPPED_METHOD to declare fields and methods in the object.
class ObjectWrapper
{
protected:
    BaseObject* obj;

public:
    ObjectWrapper(void* obj);

    DECLARE_WRAPPED_MEMBER(0x4, void*, typeId);
    DECLARE_WRAPPED_MEMBER(0x8, ObjectFlag, flags);
    DECLARE_WRAPPED_MEMBER(0xc, BaseObject*, relative1);
    DECLARE_WRAPPED_MEMBER(0x10, BaseObject*, relative2);
    DECLARE_WRAPPED_MEMBER(0x14, BaseObject*, relative3);
    DECLARE_WRAPPED_MEMBER(0x18, BaseObject*, relative4);

    DECLARE_WRAPPED_METHOD(0, void, Destruct, bool32 freeMemory);
    DECLARE_WRAPPED_METHOD(1, void, Update);
    DECLARE_WRAPPED_METHOD(2, void, Render);
    DECLARE_WRAPPED_METHOD(3, void, RenderShadow);
};
