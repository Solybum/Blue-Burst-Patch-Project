#define _USE_MATH_DEFINES

#include <cstdint>
#include <cstddef>
#include <new>
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
#include "newenemy.h"
#include "common.h"
#include "math.h"
#include "object_extension.h"
#include "enemy.h"
#include "map.h"
#include "initlist.h"
#include "object_wrapper.h"
#include "entitylist.h"
#include "newgfx/animation.h"

using Enemy::EnemyBase;
using EntityList::BaseEntityWrapper;
using Map::MapObjectWrapper;

struct NewEnemy : EnemyBase
{
    static AnimatedModel* modelData;
    static const Enemy::NpcType enemyType = (Enemy::NpcType) 1337;

    enum class Animation : uint16_t
    {
        None = 0,
        Idle = 0,
        Walk = 0,
        Attack = 0
    };

    EntityIndex targetEntityIndex;
    Vec3<float> targetPosition;
    bool hasTarget;

    bool playingFullAnimation;
    AnimatedModel model;

    NewEnemy::NewEnemy(void* parentObject, void* initData) :
        targetEntityIndex(UndefinedEntityIndex),
        hasTarget(false),
        playingFullAnimation(false),
        model(*NewEnemy::modelData),
        EnemyBase(parentObject)
    {
        OVERRIDE_METHOD(NewEnemy, Destruct);
        OVERRIDE_METHOD(NewEnemy, Update);
        OVERRIDE_METHOD(NewEnemy, Render);

        vtable->ApplyInitData(this, initData);

        Enemy::InsertIntoEntityList(this);

        InitEnemyCollisionBoxes(this, (void*) 0x009bc8e0, 1);

        animationId = -1;
        UseAnimation(Animation::Attack);
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

    void UseAnimation(Animation newAnim)
    {
        if (newAnim != (Animation) animationId)
        {
            model.ChangeAnimation((size_t) newAnim);
            animationId = (uint16_t) newAnim;
        }
    }

    void PlayAnimationFullyOnce(Animation anim)
    {
        UseAnimation(anim);
        model.AnimationLoopingEnabled(false);
        playingFullAnimation = true;
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
                PlayAnimationFullyOnce(Animation::Attack);
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
        return currentAnim == Animation::Attack;
    }

    void PlaySoundEffect(uint32_t soundId)
    {
        ::PlaySoundEffect(soundId, const_cast<Vec3<float>*>(&xyz2));
    }

    void Update()
    {
        if (playingFullAnimation)
        {
            if (model.AnimationEnded())
            {
                playingFullAnimation = false;
                model.AnimationLoopingEnabled(true);
            }
        }
        else
        {
            Behavior();
        }

        // Damaging portion of attack animation comes out at halfway through the animation
        if (IsAttacking() && model.AnimationTime() > 1.0)
        {
            PlaySoundEffect(0xe8);

            auto targetPtr = EntityList::FindEntity(targetEntityIndex);
            if (targetPtr != nullptr) BaseEntityWrapper(targetPtr).GetHit(this, 1.0);
        }

        SnapToMapSurface();
        CollideWithEntities();

        model.UpdateAnimation();

        EnableReticle(this);

        AddMinimapIcon(const_cast<Vec3<float>*>(&xyz2), (uint32_t) MinimapIconColor::Enemy, MinimapIconShape::Circle, true);
    }

    void Render()
    {
        // Apply transformations and render
        Transform::PushTransformStackCopy();
        Transform::TranslateTransformStackHead(const_cast<Vec3<float>*>(&xyz2));
        Transform::RotateMatrix(nullptr, rotation.y);

        model.Draw();

        Transform::PopTransformStack();
    }
};

AnimatedModel* NewEnemy::modelData;

void __cdecl GlobalInit()
{
    NewEnemy::modelData = new AnimatedModel("newenemy");
}

void __cdecl GlobalUninit()
{
    delete NewEnemy::modelData;
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
    auto& lobbyInitList = Map::GetMapInitList(Map::MapType::Lobby);
    forest1InitList.AddFunctionPair(InitList::FunctionPair(GlobalInit, GlobalUninit));
    lobbyInitList.AddFunctionPair(InitList::FunctionPair(GlobalInit, GlobalUninit));

    auto& forest1Enemies = Enemy::GetEnemyConstructorList(Map::MapType::Forest1);
    Enemy::TaggedEnemyConstructor newEnemyTagged(NewEnemy::enemyType, CreateNewEnemy);
    forest1Enemies.push_back(newEnemyTagged);
}
