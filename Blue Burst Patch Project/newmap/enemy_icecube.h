#pragma once

#include "../enemy.h"
#include "../object.h"

class EnemyIcecube : public Enemy::EnemyBase
{
public:
    static void __cdecl LoadAssets();
    static void __cdecl UnloadAssets();
    static void* __cdecl Create(Enemy::InitData::InnerData* initData);

    EnemyIcecube(void* parentObject, Enemy::InitData::InnerData* initData);
    
private:
    void Destruct(bool32 freeMemory);
    void Update();
    void Render();
    void RenderShadow();
    float HitByAttack(void* attacker, float powerMultiplier);
    float HitByTech(void* attacker, uint32_t techGroup, float power, void* unk1, uint8_t tech, uint32_t unk2);
    float HitBySpecial(void* attacker, void* unk1);
    
    void CollideWithEntities();
    void SnapToMapSurface();

    Enemy::EnemyBase* childEnemy;
    BaseObject* fakeRoot;
    size_t timer;
};

void PatchIcecubeUnitxt();
