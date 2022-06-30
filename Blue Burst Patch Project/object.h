#pragma once

#include <cstdint>

enum ObjectFlag : uint16_t
{
    /// An object with this flag will be destroyed on the next update tick.
    AwaitingDestruction = 1,
    /// An object with this flag will have its relative4 destroyed on the next update tick.
    Relative4AwaitingDestruction = 2
};

#pragma pack(push, 1)
/// The base PSOBB game object. All other objects inherit this.
struct BaseObject
{
    void** vtable;
    void* typeId;
    ObjectFlag flags;
    uint16_t unused;
    /// The game attaches objects to these pointers to create a graph of objects.
    /// They probably have some unknown meaning.
    BaseObject* relative1;
    BaseObject* relative2;
    BaseObject* relative3;
    BaseObject* relative4;
};
#pragma pack(pop)
