#pragma once

#include "object_wrapper.h"

namespace Entity
{
    typedef int16_t EntityIndex;
    const EntityIndex UndefinedEntityIndex = -1;

    enum EntityFlag : uint32_t
    {
        Poisoned = 1,
        Paralyzed = 2,
        Shocked = 4,
        Slowed = 8,
        Confused = 0x10,
        Frozen = 0x20,
        TookDamage = 0x200,
        Dead = 0x800
    };

    class BaseEntityWrapper : public ObjectWrapper
    {
    public:
        BaseEntityWrapper(void* obj);

        DECLARE_WRAPPED_MEMBER(0x1c, EntityIndex, entityIndex);
        DECLARE_WRAPPED_MEMBER(0x2e, int16_t, mapSection);
        DECLARE_WRAPPED_MEMBER(0x30, Entity::EntityFlag, entityFlags);
        DECLARE_WRAPPED_MEMBER(0x38, Vec3<float>, position);
        DECLARE_WRAPPED_MEMBER(0x5c, Vec3<uint32_t>, rotation);

        DECLARE_WRAPPED_METHOD(23, void, GetHit, void* attacker, float damageMultiplier);
    };
};
