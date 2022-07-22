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
#include "battleparam.h"
#include "object.h"

using Enemy::EnemyBase;
using Enemy::EntityFlag;
using EntityList::BaseEntityWrapper;
using Map::MapObjectWrapper;

auto PlaySpawnAnimationParticleEffects = reinterpret_cast<void (__cdecl *)(volatile Vec3f* position)>(0x00578998);
auto CreateBloodStain = reinterpret_cast<void (__thiscall *)(void* self)>(0x00535b38);

class NewEnemy : public EnemyBase
{
public:
    static AnimatedModel* modelData;
    static const Enemy::NpcType enemyType = (Enemy::NpcType) 1337;

private:
    static const Enemy::CollisionBox collisionBox;
    // TODO: Put these in the actual bp files
    static const BattleParam::BPStatsEntry bpStats;
    static const BattleParam::BPAttacksEntry bpAttacks;
    static const BattleParam::BPResistsEntry bpResists;

    enum class Animation : uint16_t
    {
        Slide = 0,
        Spawn = 1,
        Idle = Slide,
        BattleCry = Slide,
        Walk = Slide,
        Attack = Spawn,
        Flinch = Spawn,
        Die = Spawn
    };

    EntityIndex targetEntityIndex;
    Vec3<float> targetPosition;
    bool hasTarget;

    bool playingFullAnimation;
    AnimatedModel model;

    bool hasSpawned;
    size_t spawnSequenceCounter;
    size_t deathSequenceCounter;

public:
    NewEnemy(void* parentObject, void* initData) :
        targetEntityIndex(UndefinedEntityIndex),
        hasTarget(false),
        playingFullAnimation(false),
        hasSpawned(false),
        spawnSequenceCounter(0),
        deathSequenceCounter(0),
        model(*NewEnemy::modelData),
        EnemyBase(parentObject)
    {
        OVERRIDE_METHOD(NewEnemy, Destruct);
        OVERRIDE_METHOD(NewEnemy, Update);
        OVERRIDE_METHOD(NewEnemy, Render);

        // Custom index requires modified unitxt file
        nameUnitxtIndex = 109;
        attribute = Enemy::Attribute::Native;
        vtable->ApplyInitData(this, initData);
        Enemy::SetStatsFromBattleParams(this, &NewEnemy::bpStats, &NewEnemy::bpAttacks, &NewEnemy::bpResists);

        Enemy::InsertIntoEntityList(this);

        Enemy::InitCollisionBoxes(this, &NewEnemy::collisionBox, 1);

        UseAnimation(Animation::Spawn);
    }

private:
    void Destruct(bool32 freeMemory)
    {
        FreeCollisionBoxes(this);

        EnemyBase::Destruct(false);

        // Manually call own C++ destructor
        NewEnemy::~NewEnemy();

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
        auto newPos = *const_cast<Vec3f*>(&xyz2);
        newPos.x -= s * speed;
        newPos.z -= c * speed;

        // Apply new position if there is walkable ground
        auto ground = GetGround(&newPos, 0x15);

        if (ground != nullptr && ground->position != nullptr)
        {
            xyz2.set(newPos);
        }
    }

    void SnapToMapSurface()
    {
        auto pos = *const_cast<Vec3f*>(&xyz2);
        // Move the projection point slightly up to ensure it's actually above the ground
        pos.y += 10.0;
        // 0x15 is what other entities seem to use
        auto ground = GetGround(&pos, 0x15);
        
        if (ground != nullptr && ground->position != nullptr)
        {
            xyz2.set(ground->position);
        }
    }

    void CollideWithEntities()
    {
        ::CollideWithEntities(this);
        // Apply position changes from collisions
        xyz5.set(xyz2);
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

        auto nearestTargetPos = Vec3<float>{infinity, infinity, infinity};
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
                    nearestTargetPos.set(obj.position());
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
            targetPosition.set(target.position());
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

    void FlagForDestruction()
    {
        objectFlags = ObjectFlag(objectFlags | ObjectFlag::AwaitingDestruction);
    }

    void Update()
    {
        model.UpdateAnimation();

        if (playingFullAnimation && model.AnimationEnded())
        {
            playingFullAnimation = false;
            model.AnimationLoopingEnabled(true);
        }

        if (entityFlags & EntityFlag::Dead)
        {
            // Dead
            switch (deathSequenceCounter)
            {
                case 0:
                    UseAnimation(Animation::Die);
                    break;
                case 30:
                    CreateBloodStain(this);
                    break;
                case 60:
                    FlagForDestruction();
                    break;
            }

            // Sink into ground
            xyz2.y -= 0.5;
            deathSequenceCounter++;
            return;
        }

        if (!hasSpawned)
        {
            // Spawning
            switch (spawnSequenceCounter)
            {
                case 0:
                    PlaySoundEffect(0x124);
                    PlaySpawnAnimationParticleEffects(&xyz2);
                    break;
                case 30:
                    hasSpawned = true;
                    break;
            }
            
            spawnSequenceCounter++;
            return;
        }

        if (entityFlags & EntityFlag::TookDamage)
        {
            // Flinch from damage
            PlaySoundEffect(81);
            PlayAnimationFullyOnce(Animation::Flinch);
            entityFlags = EntityFlag(entityFlags & ~EntityFlag::TookDamage);
        }

        if (!playingFullAnimation) Behavior();

        // Damaging portion of attack animation comes out at a quarter into the animation
        if (IsAttacking() && model.CurrentFrameRatio(0.25))
        {
            PlaySoundEffect(0xe8);

            auto targetPtr = EntityList::FindEntity(targetEntityIndex);
            if (targetPtr != nullptr) BaseEntityWrapper(targetPtr).GetHit(this, 1.0);
        }

        SnapToMapSurface();
        CollideWithEntities();

        EnableReticle(this);

        AddMinimapIcon(const_cast<Vec3<float>*>(&xyz2), (uint32_t) MinimapIconColor::Enemy, MinimapIconShape::Circle, true);
    }

    void Render()
    {
        if (spawnSequenceCounter < 5) return;

        // Apply transformations and render
        Transform::PushTransformStackCopy();
        Transform::TranslateTransformStackHead(const_cast<Vec3<float>*>(&xyz2));
        Transform::RotateMatrix(nullptr, rotation.y);

        if (hasSpawned) model.UseNormalShading();
        else model.UseTransparentShading();

        model.Draw();

        Transform::PopTransformStack();
    }
};

AnimatedModel* NewEnemy::modelData;

const Enemy::CollisionBox NewEnemy::collisionBox(0.0, 5.0, 0.0, 5.0);

const BattleParam::BPStatsEntry NewEnemy::bpStats =
{
    80,
    0,
    75,
    1000,
    0,
    65,
    12,
    5,
    0.0,
    0.0,
    0,
    6,
    0,
    0,
    0
};

const BattleParam::BPAttacksEntry NewEnemy::bpAttacks =
{
    // Nothing
};

const BattleParam::BPResistsEntry NewEnemy::bpResists =
{
    0,
    50,
    0,
    20,
    15
};

void __cdecl GlobalInit()
{
    NewEnemy::modelData = new AnimatedModel("pso-ene-seal-tex-mask-rig_packed-texture.fbx");
}

void __cdecl GlobalUninit()
{
    delete NewEnemy::modelData;
    NewEnemy::modelData = nullptr;
}

void* __cdecl CreateNewEnemy(void* initData)
{
    void* buf = MainArenaAlloc(sizeof(NewEnemy));
    NewEnemy* enemy = new (buf) NewEnemy(*Enemy::rootEnemyObject, initData);
    return enemy;
}

/// Removes limit on number of enemy name entries in the unitxt
void PatchEnemyNameUnitxtLimit()
{
    *(uint8_t*) 0x00793028 = 0x90;
    *(uint8_t*) 0x00793029 = 0x90;
}

void MakeNewEnemySpawnable()
{
    auto& forest1InitList = Map::GetMapInitList(Map::MapType::Forest1);
    forest1InitList.AddFunctionPair(InitList::FunctionPair(GlobalInit, GlobalUninit));

    auto& forest1Enemies = Enemy::GetEnemyConstructorList(Map::MapType::Forest1);
    Enemy::TaggedEnemyConstructor newEnemyTagged(NewEnemy::enemyType, CreateNewEnemy);
    forest1Enemies.push_back(newEnemyTagged);
}

void ApplyNewEnemyPatch()
{
    PatchEnemyNameUnitxtLimit();
    MakeNewEnemySpawnable();
}
