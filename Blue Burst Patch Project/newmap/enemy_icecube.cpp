#include "common.h"
#include <type_traits>
#ifdef PATCH_NEWMAP

#include <cstdint>
#include <stdexcept>
#include <unordered_map>

#include "../enemy.h"
#include "../helpers.h"
#include "../mathutil.h"
#include "../ninja.h"
#include "../object_extension.h"
#include "../object.h"
#include "../psobb_functions.h"
#include "enemy_icecube.h"

static size_t icecubeUnitxtIndex = 0x1000;
static std::unordered_map<size_t, std::wstring> prefixedNamesCache;
static auto icecubeCollisionBox = Enemy::CollisionBox(0.0, 5.0, 0.0, 8.0);
static void* xvm = nullptr;
static TBinModel* bml = nullptr;

EnemyIcecube::EnemyIcecube(void* parentObject, Enemy::InitData::InnerData* initData) :
    timer(0),
    Enemy::EnemyBase(parentObject)
{
    OVERRIDE_METHOD(EnemyIcecube, Destruct);
    OVERRIDE_METHOD(EnemyIcecube, Update);
    OVERRIDE_METHOD(EnemyIcecube, Render);
    OVERRIDE_METHOD(EnemyIcecube, RenderShadow);
    OVERRIDE_METHOD(EnemyIcecube, HitByAttack);
    OVERRIDE_METHOD(EnemyIcecube, HitByTech);
    OVERRIDE_METHOD(EnemyIcecube, HitBySpecial);

    vtable->ApplyInitData(this, initData);
    Enemy::InsertIntoEntityList(this);
    Enemy::InitCollisionBoxes(this, &icecubeCollisionBox, 1);

    // Initdata param6 = child enemy type
    // Find child constructor
    auto childEnemyType = (Enemy::NpcType) initData->param6;
    auto childEnemyConstructor = Enemy::FindEnemyConstructor(childEnemyType);
    if (childEnemyConstructor == nullptr)
        throw std::runtime_error("Invalid icecube child type");

    auto childInitData = Enemy::InitData::InnerData
    {
        .indexSmall = (int16_t) (initData->indexSmall + 1),
        .cloneCount = 0,
        .floorNumber = initData->floorNumber,
        .entityLarge = (int16_t) (initData->entityLarge + 1),
        .mapSection = initData->mapSection,
        .waveNumber = initData->waveNumber,
        .waveNumber2 = initData->waveNumber2,
        .unknown = 0,
        .position = initData->position,
        .rotation = initData->rotation,
        .param1 = 0.0,
        .param2 = 0.0,
        .param3 = 0.0,
        .param4 = 0.0,
        .param5 = 0.0,
        .param6 = 0,
        .param7 = 0,
        .spawnedInstance = nullptr
    };

    // Construct child with a fake parent object by temporarily replacing the enemy root
    auto origRoot = *Enemy::rootEnemyObject;
    fakeRoot = new BaseObject();
    BaseGameObjectConstructor(fakeRoot, nullptr);
    *Enemy::rootEnemyObject = fakeRoot;
    childEnemy = (Enemy::EnemyBase*) childEnemyConstructor->constructor(&childInitData);
    *Enemy::rootEnemyObject = origRoot;

    // Combine unitxt ids
    nameUnitxtIndex = icecubeUnitxtIndex | childEnemy->nameUnitxtIndex;
}

void EnemyIcecube::Destruct(bool32 freeMemory)
{
    FreeCollisionBoxes(this);
    
    if (fakeRoot)
    {
        // Destructors will recurse
        if (fakeRoot->childObject != nullptr)
            fakeRoot->childObject->vtable->Destruct(fakeRoot->childObject, 1);
        
        delete fakeRoot;
        fakeRoot = nullptr;
    }

    EnemyBase::Destruct(false);

    // Manually call own C++ destructor
    EnemyIcecube::~EnemyIcecube();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void EnemyIcecube::Update()
{
    Vec3f pos;
    pos.set(xyz2);

    SnapToMapSurface();
    CollideWithEntities();
    EnableReticle(this);
    AddMinimapIcon(&pos, (uint32_t) MinimapIconColor::Enemy, MinimapIconShape::Circle, true);

    if (++timer > 30 * 10)
    {
        // Destroy self
        objectFlags = ObjectFlag(objectFlags | ObjectFlag::AwaitingDestruction);

        // Move child to normal root
        auto actualChild = fakeRoot->childObject;
        if (actualChild)
        {
            fakeRoot->childObject->parentObject = nullptr;
            fakeRoot->childObject = nullptr;
            delete fakeRoot;
            fakeRoot = nullptr;

            auto root = *Enemy::rootEnemyObject;
            auto head = root->childObject;
            actualChild->parentObject = root;

            if (head == nullptr)
            {
                actualChild->prevSiblingObject = (BaseObject*) actualChild;
                root->childObject = (BaseObject*) actualChild;
            }
            else
            {
                actualChild->prevSiblingObject = head->prevSiblingObject;
                head->prevSiblingObject->nextSiblingObject = (BaseObject*) actualChild;
                head->prevSiblingObject = (BaseObject*) actualChild;
            }
        }
    }
}

void EnemyIcecube::Render()
{
    DisableFog();

    Vec3f pos;
    pos.set(xyz2);

    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(&pos);
    Transform::RotateMatrix(nullptr, rotation.y);

    UseTextureArchive((void*) childEnemy->njtl);
    RenderNj((void*) childEnemy->njcm);
    
    // Functions related to depth buffer settings
    // For some reason these are required to get the transparency to work
    auto x = *reinterpret_cast<uint32_t*>(0x00aab47c + 4 * 3);
    *reinterpret_cast<uint32_t*>(0x00a16380) = 1;
    reinterpret_cast<void (__cdecl *)()>(0x007a9380)();
    reinterpret_cast<void (__cdecl *)(uint32_t)>(0x007a94a0)(2);
    reinterpret_cast<void (__cdecl *)()>(0x007a9440)();
    UseTextureArchive(xvm);
    RenderXj(GetModel(bml, 0));
    reinterpret_cast<void (__cdecl *)()>(0x007a9470)();
    *reinterpret_cast<uint32_t*>(0x00a16380) = 0;
    reinterpret_cast<void (__cdecl *)(uint32_t)>(0x007a94a0)(x);

    Transform::PopTransformStack();
    EnableFog();
}

// Disable shadow because it looks weird
void EnemyIcecube::RenderShadow() {}

// Disable damage
float EnemyIcecube::HitByAttack(void* attacker, float powerMultiplier) { return 0.0; }
float EnemyIcecube::HitByTech(void* attacker, uint32_t techGroup, float power, void* unk1, uint8_t tech, uint32_t unk2) { return 0.0; }
float EnemyIcecube::HitBySpecial(void* attacker, void* unk1) { return 0.0; }

void EnemyIcecube::CollideWithEntities()
{
    ::CollideWithEntities(this);
    // Apply position changes from collisions
    xyz5.set(xyz2);
}

void EnemyIcecube::SnapToMapSurface()
{
    Vec3f pos;
    pos.set(xyz2);
    // Move the projection point slightly up to ensure it's actually above the ground
    pos.y += 10.0;
    // 0x15 is what other entities seem to use
    auto ground = GetGround(&pos, 0x15);
    
    if (ground != nullptr && ground->position != nullptr)
    {
        xyz2.set(ground->position);
    }
}

void __cdecl EnemyIcecube::LoadAssets()
{
    static BmlMetadata bmlMeta = {
        "icecube.bml",
        nullptr,
        nullptr,
        1,
        0
    };

    xvm = LoadXvm("icecube.xvm");
    
    bml = (TBinModel*) pf_malloc(sizeof(TBinModel));
    LoadBml(bml, 5, &bmlMeta, nullptr, 0, nullptr);

    PrepareBmlMeshes(bml);
}

void __cdecl EnemyIcecube::UnloadAssets()
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

void* __cdecl EnemyIcecube::Create(Enemy::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(EnemyIcecube));
    EnemyIcecube* icecube = new (buf) EnemyIcecube(*Enemy::rootEnemyObject, initData);
    return icecube;
}

const wchar_t* __cdecl GetUnitxtEnemyName(uint32_t unitxtIndex)
{
    auto origFunc = reinterpret_cast<decltype(GetUnitxtEnemyName)*>(0x00793014);
    if (unitxtIndex & icecubeUnitxtIndex)
    {
        // Decode child enemy unitxt index and add prefix to child name
        auto childUnitxtIndex = unitxtIndex & ~icecubeUnitxtIndex;
        if (prefixedNamesCache.count(childUnitxtIndex) == 0)
        {
            // Cache name
            auto childName = origFunc(childUnitxtIndex);
            prefixedNamesCache[childUnitxtIndex] = std::wstring(L"Crystallized ") + childName;
        }
        return prefixedNamesCache[childUnitxtIndex].c_str();
    }
    return origFunc(unitxtIndex);
}

void PatchIcecubeUnitxt()
{
    // Need to patch unitxt name getter function because we want to add a prefix to the child enemy's name
    PatchCALL(0x00731f08, 0x00731f08 + 5, (int) GetUnitxtEnemyName);
}

#endif
