#pragma once

#include <algorithm>

#include "common.h"

#define MAKE_PADDING_FIELD(size) CONCAT(_pad, __COUNTER__)[size]

/// Define a field at a certain offset (from the current offset).
/// It is recommended to put all such fields inside a union to ensure their offset base stays the same.
#define DEFINE_FIELD(offset, field) struct { uint8_t MAKE_PADDING_FIELD(offset); volatile field; }

/// Override a method in the derived vtable
#define OVERRIDE_METHOD(clazz, method) \
    vtable->method = reinterpret_cast<decltype(Vtable::method)>(VoidCastMemberFunction(&clazz::method));

/// Probably UB
template<typename Ret, typename Class, typename... Args>
void* VoidCastMemberFunction(Ret (Class::*member)(Args...))
{
    union
    {
        Ret (Class::*m)(Args...);
        void* ptr;
    };

    m = member;

    return ptr;
}

typedef void* (__cdecl *MainArenaAllocFunction)(size_t size);
extern MainArenaAllocFunction MainArenaAlloc;

typedef void (__cdecl *MainArenaDeallocFunction)(void* node);
extern MainArenaDeallocFunction MainArenaDealloc;

extern void* (__thiscall *BaseGameObjectConstructor)(void* self, void* parent);
extern void (__thiscall *BaseGameObjectDestructor)(void* self, bool32 freeMemory);
extern void (__thiscall *BaseGameObjectDestructorNoDealloc)(void* self);

template<typename T>
T* InheritVtable(T* orig)
{
    T* vtable = new T;
    std::copy(orig, orig + 1, vtable);
    return vtable;
}
