#pragma once

#include <cstdint>

#include "object_extension.h"

enum ObjectFlag : uint16_t
{
    /// An object with this flag will be destroyed on the next update tick.
    AwaitingDestruction = 1,
    /// An object with this flag will have its relative4 destroyed on the next update tick.
    Relative4AwaitingDestruction = 2,
    UpdateDisallowed = 0x8,
    RenderDisallowed = 0x10,
    DestructionDisallowed = 0x20,
    ShadowRenderDisallowed = 0x100
};

#pragma pack(push, 1)
/// The base PSOBB game object. All other objects inherit this.
struct BaseObject
{
    struct Vtable {
        union {
            void (__thiscall *Destruct)(void* self, bool32 free_memory);
            DEFINE_FIELD(0x4, void (__thiscall *Update)(void* self));
            DEFINE_FIELD(0x8, void (__thiscall *Render)(void* self));
            DEFINE_FIELD(0xc, void (__thiscall *RenderShadow)(void* self));
        };
    };

    Vtable* vtable;
    void* typeId;
    ObjectFlag objectFlags;
    uint16_t unused;
    /// The game attaches objects to these pointers to create a graph of objects.
    /// They probably have some unknown meaning.
    BaseObject* prevSiblingObject;
    BaseObject* nextSiblingObject;
    BaseObject* parentObject;
    BaseObject* childObject;
};
#pragma pack(pop)

extern BaseObject::Vtable* baseObjectVtable;
