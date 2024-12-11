#ifdef PATCH_NEWMAP

#define _USE_MATH_DEFINES

#include <cmath>

#include "map_object_turret.h"

#include "../common.h"
#include "../enemy.h"
#include "../entity.h"
#include "../entitylist.h"
#include "../map_object.h"
#include "../mathutil.h"
#include "../ninja.h"
#include "../object.h"
#include "../psobb_functions.h"

enum ModelIndex
{
    Cylinder = 0,
    Gun = 1
};

static void* xvm = nullptr;
static TBinModel* bml = nullptr;
static auto cylinderCollisionBox = Enemy::CollisionBox(0.0, 5.0, 0.0, 1.0);

class TurretBullet : public Enemy::EnemyBase
{
public:
    Vec3f endPosition;
    Vec3f velocityVec;
    int targetEntityId;

    TurretBullet(const Vec3f& startPosition, const Vec3f& endPosition, float velocity, int targetEntityId = -1) :
        Enemy::EnemyBase(*reinterpret_cast<void**>(0x00aca36c)),
        endPosition(endPosition),
        targetEntityId(targetEntityId)
    {
        OVERRIDE_METHOD(TurretBullet, Destruct);
        OVERRIDE_METHOD(TurretBullet, Update);
        OVERRIDE_METHOD(TurretBullet, Render);
        OVERRIDE_METHOD(TurretBullet, RenderShadow);
        
        xyz1.set(startPosition);

        rotation.x = 0;
        rotation.y = RadToIntAngle(std::atan2(endPosition.x - xyz1.x, endPosition.z - xyz1.z));
        rotation.z = RadToIntAngle(std::atan2(std::sqrt(DistanceSquaredXZ(endPosition, xyz1)), endPosition.y - xyz1.y));

        velocityVec.x = startPosition.x - endPosition.x;
        velocityVec.y = startPosition.y - endPosition.y;
        velocityVec.z = startPosition.z - endPosition.z;
        auto mag = std::sqrt(velocityVec.x * velocityVec.x + velocityVec.y * velocityVec.y + velocityVec.z * velocityVec.z);
        velocityVec.x = velocityVec.x / mag * velocity;
        velocityVec.y = velocityVec.y / mag * velocity;
        velocityVec.z = velocityVec.z / mag * velocity;
        
        entityIndex = 0x1000;
        
        static const BattleParam::BPStatsEntry bpStats =
        {
            8,
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

        static const BattleParam::BPAttacksEntry bpAttacks = {};

        static const BattleParam::BPResistsEntry bpResists =
        {
            0,
            50,
            0,
            20,
            15
        };
        Enemy::SetStatsFromBattleParams(this, &bpStats, &bpAttacks, &bpResists);
        
        unkElemDamage1 = 0;
        unkElemDamage2 = 0;
        unkElemDamage3 = 0;
        unkElemDamage4 = 0;
        unkElemDamage5 = 0;
    }
    
    void Destruct(BOOL freeMemory)
    {
        Enemy::EnemyBase::Destruct(false);

        TurretBullet::~TurretBullet();

        if (freeMemory)
        {
            MainArenaDealloc(this);
        }
    }
    
    void Update()
    {
        const auto bulletRadius = 10.0;
        if (DistanceSquaredXZ(xyz1, endPosition) < Squared(bulletRadius))
        {
            objectFlags = ObjectFlag(objectFlags | ObjectFlag::AwaitingDestruction);
            auto ptr = EntityList::FindEntity(targetEntityId);
            if (!ptr) return;
            auto target = Entity::BaseEntityWrapper(ptr);
            target.GetHit(this, 1.0);
            return;
        }

        xyz1.x -= velocityVec.x;
        xyz1.y -= velocityVec.y;
        xyz1.z -= velocityVec.z;
    }

    void Render()
    {
        auto gunModel = GetModel(bml, ModelIndex::Gun);
        
        Transform::PushTransformStackCopy();
        Transform::TranslateTransformStackHead(const_cast<Vec3f*>(&xyz1));
        const auto scale = 0.1;
        Transform::ScaleMatrix(nullptr, scale, scale, scale);
        Transform::RotateMatrixEuler(nullptr, rotation.x, rotation.y, rotation.z);

        UseTextureArchive(xvm);

        RenderXj(gunModel);

        Transform::PopTransformStack();
    }

    void RenderShadow() {}
};

MapObjectTurret::MapObjectTurret(void* parentObject, MapObject::InitData::InnerData* initData) :
    MapObject::MapObjectBase(parentObject),
    shootCooldownTimer(0)
{
    OVERRIDE_METHOD(MapObjectTurret, Destruct);
    OVERRIDE_METHOD(MapObjectTurret, Update);
    OVERRIDE_METHOD(MapObjectTurret, Render);
    OVERRIDE_METHOD(MapObjectTurret, RenderShadow);
    
    vtable->ApplyInitData(this, initData);
    collisionBoxes = nullptr;
    Enemy::InitCollisionBoxes(this, &cylinderCollisionBox, 1);
    Enemy::InsertIntoEntityList(this);
}

void MapObjectTurret::Destruct(BOOL freeMemory)
{
    FreeCollisionBoxes(this);

    MapObjectBase::Destruct(false);

    Enemy::RemoveFromEntityList(this);

    MapObjectTurret::~MapObjectTurret();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void MapObjectTurret::Update()
{
    auto nearestDist = 1000000000.0;
    Enemy::EnemyBase* nearestEnemy = nullptr;
    for (auto ptr : EntityList::Enemies())
    {
        auto enemy = reinterpret_cast<Enemy::EnemyBase*>(ptr);
        auto dist = DistanceSquaredXZ(xyz1, enemy->xyz2);
        if (dist < nearestDist)
        {
            nearestDist = dist;
            nearestEnemy = enemy;
        }
    }

    const auto maxShootDist = 200.0;
    if (nearestDist > Squared(maxShootDist) || nearestEnemy == nullptr) return;

    auto targetAngle = std::atan2(nearestEnemy->xyz2.x - xyz1.x, nearestEnemy->xyz2.z - xyz1.z);
    auto deltaAngle = targetAngle - IntAngleToRad(rotation.y);
    if (deltaAngle > M_PI) rotation.y += RadToIntAngle(M_PI * 2.0);
    else if (deltaAngle < -M_PI) rotation.y -= RadToIntAngle(M_PI * 2.0);
    auto angleVelocity = (targetAngle - IntAngleToRad(rotation.y)) * 0.5;
    const auto maxVelocity = 0.06;
    if (angleVelocity > maxVelocity) angleVelocity = maxVelocity;
    else if (angleVelocity < -maxVelocity) angleVelocity = -maxVelocity;
    rotation.y += RadToIntAngle(angleVelocity);
    if (rotation.y < 0) rotation.y = (-rotation.y) % 0xffff;

    const auto shootCooldownFrameCount = 45;
    if (++shootCooldownTimer < shootCooldownFrameCount) return;
    if (std::abs(RadDistance(IntAngleToRad(rotation.y), targetAngle)) > maxVelocity) return;

    shootCooldownTimer = 0;
    auto bulletStartPos = Vec3f();
    bulletStartPos.set(xyz1);
    
    bulletStartPos.y += 25.0;
    auto angle = IntAngleToRad(rotation.y);
    auto sin = std::sin(angle);
    auto cos = std::cos(angle);
    bulletStartPos.x += sin * 18.0;
    bulletStartPos.z += cos * 18.0;
    
    PlaySoundEffect(100, &bulletStartPos);
    PlayParticleEffect(&bulletStartPos, 9, 0, 1);

    nearestEnemy->collisionBoxes[0].unknownFlags |= 0x4000000;
    auto bulletEndPos = Vec3f();
    bulletEndPos.x = nearestEnemy->collisionBoxes[0].x2;
    bulletEndPos.y = nearestEnemy->collisionBoxes[0].y2;
    bulletEndPos.z = nearestEnemy->collisionBoxes[0].z2;

    void* bulletBuf = MainArenaAlloc(sizeof(TurretBullet));
    new (bulletBuf) TurretBullet(bulletStartPos, bulletEndPos, 10.0, nearestEnemy->entityIndex);
}

void MapObjectTurret::Render()
{
    auto towerModel = GetModel(bml, ModelIndex::Cylinder);
    auto gunModel = GetModel(bml, ModelIndex::Gun);
    
    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(const_cast<Vec3f*>(&xyz1));
    Transform::RotateMatrix(nullptr, rotation.y);

    UseTextureArchive(xvm);

    RenderXj(towerModel);
    RenderXj(gunModel);

    Transform::PopTransformStack();
}

void MapObjectTurret::RenderShadow() {}

void __cdecl MapObjectTurret::LoadAssets()
{
    static BmlMetadata bmlMeta = {
        "bm_obj_turret.bml",
        nullptr,
        nullptr,
        2,
        0
    };

    xvm = LoadXvm("bm_obj_turret.xvm");
    
    bml = (TBinModel*) pf_malloc(sizeof(TBinModel));
    LoadBml(bml, 5, &bmlMeta, nullptr, 0, nullptr);

    PrepareBmlMeshes(bml);
}

void __cdecl MapObjectTurret::UnloadAssets()
{
    if (xvm)
    {
        FreeXvm(xvm);
        xvm = nullptr;
    }

    if (bml) {
        FreeBml(bml, 1);
        bml = nullptr;
    }
}

void* __cdecl MapObjectTurret::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectTurret));
    return new (buf) MapObjectTurret(*MapObject::rootMapObject, initData);
}

#undef _USE_MATH_DEFINES

#endif
