#pragma once

#include <cstdint>
#include <windows.h>
#include "mathutil.h"
#include "object_extension.h"

namespace MapObject
{
#pragma pack(push, 1)

    struct MapObjectBase; // Forward declaration for InitData

    union InitParam
    {
        float asFloat;
        uint32_t asDword;
        struct
        {
            uint16_t word1;
            uint16_t word2;
        } asWords;
    };

    /**
     * @brief Data loaded from the quest's .dat file is passed to the object's constructor
     */
    struct InitData
    {
        int16_t skin;
        uint16_t flags;
        struct InnerData
        {
            int16_t indexSmall;
            uint16_t unk1;
            int16_t indexLarge;
            int16_t group;
            int16_t mapSection;
            int16_t unk2;
            Vec3f position;
            Vec3<int32_t> rotation;
            InitParam param1;
            InitParam param2;
            InitParam param3;
            InitParam param4;
            InitParam param5;
            InitParam param6;
            MapObjectBase* spawnedInstance;
        } data;
    };

    struct MapObjectBase
    {
        struct Vtable {
            union {
                void (__thiscall *Destruct)(void* self, BOOL freeMemory);
                DEFINE_FIELD(0x4, void (__thiscall *Update)(void* self));
                DEFINE_FIELD(0x8, void (__thiscall *Render)(void* self));
                DEFINE_FIELD(0xc, void (__thiscall *RenderShadow)(void* self));
                DEFINE_FIELD(0x14, void (__thiscall *ApplyInitData)(void* self, InitData::InnerData* initData));
            };
        };

        typedef void* (__thiscall *ConstructorFunction)(void* self, void* parentObject);
        static const ConstructorFunction Constructor;
        static const Vtable* origVtable;

        union
        {
            Vtable* vtable;

            union {
                DEFINE_FIELD(0x28, int16_t mapSection);
                DEFINE_FIELD(0x38, Vec3f xyz1);
                DEFINE_FIELD(0x50, Vec3f xyz2);
                DEFINE_FIELD(0x5c, Vec3<int32_t> rotation);
                DEFINE_FIELD(0x68, InitParam initParam1);
                DEFINE_FIELD(0x6c, InitParam initParam2);
                DEFINE_FIELD(0x70, InitParam initParam3);
                DEFINE_FIELD(0x84, InitParam initParam4);
                DEFINE_FIELD(0x88, InitParam initParam5);
                DEFINE_FIELD(0x8c, InitParam initParam6);
            };

            // Ensure object's size is at least the same as its superclass
            uint8_t _padding[0x37c];
        };

        MapObjectBase(void* parentObject);

        void Destruct(bool32 freeMemory);
    };

#pragma pack(pop)

    extern void** rootMapObject;
};
