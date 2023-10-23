#pragma once

#include "mathutil.h"
#include "object_extension.h"

struct Player
{
    struct Vtable
    {

    };

    union
    {
        Vtable* vtable;

        union
        {
            DEFINE_FIELD(0x28, int16_t room);
            DEFINE_FIELD(0x38, Vec3f position);
        };

        uint8_t _padding[0xec0];
    };
};

extern Player* (__cdecl *GetPlayer)(uint16_t id);
