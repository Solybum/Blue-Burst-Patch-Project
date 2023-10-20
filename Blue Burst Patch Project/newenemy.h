#pragma once

#include "initlist.h"
#include "enemy.h"
#include "map.h"
#include "newgfx/animation.h"

using Enemy::EnemyBase;
using Map::MapObjectWrapper;

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

    Entity::EntityIndex targetEntityIndex;
    Vec3<float> targetPosition;
    bool hasTarget;

    bool playingFullAnimation;
    AnimatedModel model;

    bool hasSpawned;
    size_t spawnSequenceCounter;
    size_t deathSequenceCounter;

public:
    NewEnemy(void* parentObject, void* initData);

private:
    void Destruct(bool32 freeMemory);
    void UseAnimation(Animation newAnim);
    void PlayAnimationFullyOnce(Animation anim);
    void FaceTowardsTarget();
    void MoveForwards();
    void SnapToMapSurface();
    void CollideWithEntities();
    bool IsValidTarget(MapObjectWrapper& obj) const;
    void SearchForNewTarget();
    void UpdateTargetExistence();
    void Behavior();
    bool IsAttacking();
    void PlaySoundEffect(uint32_t soundId);
    void FlagForDestruction();
    void Update();
    void Render();
};

void ApplyNewEnemyPatch();
