#define _USE_MATH_DEFINES

#include <cstdint>
#include <cstddef>
#include <new>
#include <algorithm>
#include <cmath>
#include <limits>
#include "newenemy.h"
#include "common.h"
#include "math.h"
#include "object_extension.h"
#include "enemy.h"
#include "map.h"
#include "initlist.h"
#include "object_wrapper.h"
#include "entitylist.h"

using Enemy::EnemyBase;
using EntityList::BaseEntityWrapper;
using Map::MapObjectWrapper;

struct NewEnemy : EnemyBase
{
    static Enemy::BmlData* bml;
    static const Enemy::NpcType enemyType = (Enemy::NpcType) 1337;

    int i;

    enum class Animation : uint16_t
    {
        Idle = 0,
        Walk = 2,
        AttackR = 3,
        AttackL = 4
    };

    EntityIndex targetEntityIndex;
    Vec3<float> targetPosition;
    bool hasTarget;

    bool playingFullAnimation;

    NewEnemy::NewEnemy(void* parentObject, void* initData) :
        targetEntityIndex(UndefinedEntityIndex),
        hasTarget(false),
        playingFullAnimation(false),
        EnemyBase(parentObject)
    {
        OVERRIDE_METHOD(NewEnemy, Destruct);
        OVERRIDE_METHOD(NewEnemy, Update);
        OVERRIDE_METHOD(NewEnemy, Render);

        vtable->ApplyInitData(this, initData);

        Enemy::InsertIntoEntityList(this);

        njtl = NewEnemy::bml->nj[0].njtl;
        njcm = NewEnemy::bml->nj[0].njcm;
        njm = NewEnemy::bml->njm;

        InitEnemyCollisionBoxes(this, (void*) 0x009bc8e0, 1);

        unknownAnimationData = reinterpret_cast<void*>(0x009bc980);
        InitEnemyAnimations(this, 0);

        UseAnimation(Animation::Idle);
    }

    void Destruct(bool32 freeMemory)
    {
        FreeCollisionBoxes(this);

        EnemyBase::Destruct(false);

        if (freeMemory)
        {
            MainArenaDealloc(this);
        }
    }

    void UseAnimation(Animation anim)
    {
        animationId = (uint16_t) anim;
    }

    void PlayAnimationFullyOnce(Animation anim)
    {
        UseAnimation(anim);
        playingFullAnimation = true;
    }

    bool AnimationEnded()
    {
        return currentAnimationCounter + currentAnimationSpeed >= currentAnimationLength;
    }

    void Spin()
    {
        i++;
        xyz2.x = std::sin(i / 5.0) * 20.0 + xyz1.x;
        xyz2.z = std::cos(i / 5.0) * 20.0 + xyz1.z;
    }

    void FaceTowardsTarget()
    {
        // Get angle towards target
        auto angleDelta = atan2(targetPosition.z - xyz2.z, targetPosition.x - xyz2.x);
        int16_t targetAngleInt = RadToIntAngle(angleDelta);
        // Invert and Offset by 90deg because idk
        targetAngleInt = 0xffff - targetAngleInt;
        targetAngleInt -= 0x3fff;

        double currentAngle = IntAngleToRad(rotation.y);
        double targetAngle = IntAngleToRad(targetAngleInt);

        // Turn slowly
        double angleDistance = RadDistance(currentAngle, targetAngle);
        double turnAmount = fmin(abs(angleDistance), M_PI / 180.0) * Sign(angleDistance);

        rotation.y = RadToIntAngle(currentAngle + turnAmount);
    }

    void MoveForwards()
    {
        // Project a point in the current heading direction
        auto rot = IntAngleToRad(rotation.y);
        auto s = sin(rot);
        auto c = cos(rot);

        const auto speed = 0.25;
        auto newPos = xyz2;
        newPos.x -= s * speed;
        newPos.z -= c * speed;

        // Apply new position if there is walkable ground
        auto ground = GetGround(&newPos, 0x15);

        if (ground != nullptr && ground->position != nullptr)
        {
            xyz2 = newPos;
        }
    }

    void SnapToMapSurface()
    {
        auto pos = xyz2;
        // Move the projection point slightly up to ensure it's actually above the ground
        pos.y += 10.0;
        // 0x15 is what other entities seem to use
        auto ground = GetGround(&pos, 0x15);
        
        if (ground != nullptr && ground->position != nullptr)
        {
            xyz2 = *ground->position;
        }
    }

    void CollideWithEntities()
    {
        ::CollideWithEntities(this);
        // Apply position changes from collisions
        xyz5 = xyz2;
    }

    bool IsValidTarget(MapObjectWrapper& obj) const
    {
        // Object is a box
        auto type = obj.mapObjectType();
        return type != nullptr && *type == 136 && obj.mapSection() == originalMapSection;
    }

    void SearchForNewTarget()
    {
        auto infinity = std::numeric_limits<float>::infinity();

        auto nearestTargetPos = Vec3<float>(infinity, infinity, infinity);
        auto nearestTargetIndex = -1;
        auto nearestTargetDistanceSquared = infinity;
        bool foundTarget = false;

        for (auto ptr : EntityList::Objects())
        {
            auto obj = MapObjectWrapper(ptr);
            if (IsValidTarget(obj))
            {
                // Check if nearer than previously selected target
                auto dist = DistanceSquaredXZ(obj.position(), xyz2);

                if (dist < nearestTargetDistanceSquared)
                {
                    nearestTargetPos = obj.position();
                    nearestTargetIndex = obj.entityIndex();
                    nearestTargetDistanceSquared = dist;
                    foundTarget = true;
                }
            }
        }

        if (foundTarget)
        {
            targetPosition = nearestTargetPos;
            targetEntityIndex = nearestTargetIndex;
            hasTarget = true;
        }
    }

    void UpdateTargetExistence()
    {
        auto targetPtr = EntityList::FindEntity(targetEntityIndex);
        hasTarget = targetPtr != nullptr;

        if (hasTarget)
        {
            auto target = BaseEntityWrapper(targetPtr);
            targetPosition = target.position();
        }
    }

    void Behavior()
    {
        if (!hasTarget)
        {
            // No target, search new
            UseAnimation(Animation::Idle);
            SearchForNewTarget();
        }
        
        UpdateTargetExistence();

        if (hasTarget)
        {
            if (DistanceSquaredXZ(targetPosition, xyz2) < Squared(20.0))
            {
                // Close enough to attack
                Animation attackAnim = Animation::AttackR;
                if (GetRandomFloat(this) < 0.5) attackAnim = Animation::AttackL;
                PlayAnimationFullyOnce(attackAnim);
            }
            else
            {
                // Move towards target
                UseAnimation(Animation::Walk);
                FaceTowardsTarget();
                MoveForwards();
            }
        }
    }

    bool IsAttacking()
    {
        Animation currentAnim = (Animation) animationId;
        return currentAnim == Animation::AttackR || currentAnim == Animation::AttackL;
    }

    void PlaySoundEffect(uint32_t soundId)
    {
        ::PlaySoundEffect(soundId, const_cast<Vec3<float>*>(&xyz2));
    }

    void NewEnemy::Update()
    {
        if (playingFullAnimation)
        {
            playingFullAnimation = !AnimationEnded();
        }
        else
        {
            Behavior();
        }

        // Damaging portion of attack animation comes out at frame 7
        if (IsAttacking() && CheckAnimationDuration(this, 7.0))
        {
            PlaySoundEffect(0xe8);

            auto targetPtr = EntityList::FindEntity(targetEntityIndex);
            if (targetPtr != nullptr) BaseEntityWrapper(targetPtr).GetHit(this, 1.0);
        }

        SnapToMapSurface();
        CollideWithEntities();

        AnimateEntity(this);

        EnableReticle(this);

        AddMinimapIcon(const_cast<Vec3<float>*>(&xyz2), (uint32_t) MinimapIconColor::Enemy, MinimapIconShape::Circle, true);
    }

    void NewEnemy::Render()
    {
        Graphics::EnableTexture(const_cast<void*>(njtl));
        // Apply transformations and render
        Transform::PushTransformStackCopy();
        Transform::TranslateTransformStackHead(const_cast<Vec3<float>*>(&xyz2));
        Transform::RotateMatrix(nullptr, rotation.y);
        Graphics::Render3D(this);
        Transform::PopTransformStack();
    }
};

Enemy::BmlData* NewEnemy::bml;

const char* newEnemyNjNames[] =
{
    "re8_b_beast_wola_body.nj",
    "re8_b_rdbeast_wola_body.nj",
    "re8_b_srdbeast_wola_body.nj"
};

const char* newEnemyNjmNames[] =
{
    "appear_bm1_s_wala_body.njm",
    "atackl_bm1_s_wala_body.njm",
    "atackr_bm1_s_wala_body.njm",
    "damage_bm1_s_wala_body.njm",
    "deadb_bm1_s_wala_body.njm",
    "dead_bm1_s_wala_body.njm",
    "leader_bm1_s_wala_body.njm",
    "mihari_bm1_s_wala_body.njm",
    "run_bm1_s_wala_body.njm",
    "stund_bm1_s_wala_body.njm",
    "wakeup_bm1_s_wala_body.njm",
    "walk_bm1_s_wala_body.njm"
};

Enemy::BmlContentsInfo newEnemyBmlToc =
{
    "bm_ene_newenemy.bml",
    newEnemyNjNames,
    newEnemyNjmNames,
    arraySize(newEnemyNjNames),
    arraySize(newEnemyNjmNames)
};

void __cdecl GlobalInit()
{
    NewEnemy::bml = Enemy::LoadBml(&newEnemyBmlToc);
}

void __cdecl GlobalUninit()
{
    if (NewEnemy::bml != nullptr)
    {
        Enemy::FreeBml(NewEnemy::bml, true);
        NewEnemy::bml = nullptr;
    }
}

void* __cdecl CreateNewEnemy(void* initData)
{
    void* buf = MainArenaAlloc(sizeof(NewEnemy));
    NewEnemy* enemy = new (buf) NewEnemy(*Enemy::rootEnemyObject, initData);
    return enemy;
}

void ApplyNewEnemyPatch()
{
    auto& forest1InitList = Map::GetMapInitList(Map::MapType::Forest1);
    forest1InitList.AddFunctionPair(InitList::FunctionPair(GlobalInit, GlobalUninit));

    auto& forest1Enemies = Enemy::GetEnemyConstructorList(Map::MapType::Forest1);
    Enemy::TaggedEnemyConstructor newEnemyTagged(NewEnemy::enemyType, CreateNewEnemy);
    forest1Enemies.push_back(newEnemyTagged);
}
