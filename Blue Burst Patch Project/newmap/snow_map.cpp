#ifdef PATCH_NEWMAP

#include <cstdlib>
#include <windows.h>

#include "d3d8types.h"

#include "../enemy.h"
#include "../map_object.h"
#include "../player.h"
#include "enemy_icecube.h"
#include "map_object_cloud.h"
#include "map_object_newdoor.h"
#include "map_object_snowfall.h"
#include "newmap.h"
#include "setdata.h"
#include "snow_map.h"

static const char* snowMapName = "SNOW01";
static MapAssetPrefixes::Prefixes snowMapAssetPrefixStrings = {
    "map_snow01",
    "map_snow01"
};

static SetDataTable::Inner1::Inner2 snowSetDataTableInner2 = {
    "map_snow01",
    "map_snow01",
    "map_snow01"
};
static SetDataTable::Inner1 snowSetDataTableInner1 = {
    &snowSetDataTableInner2,
    1
};

void __thiscall EnableNrelPointLight()
{
    auto player = GetPlayer(0);
    if (player)
    {
        auto light = D3DLIGHT8();
        light.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
        light.Diffuse.r = 1.0;
        light.Diffuse.g = 0.0;
        light.Diffuse.b = 0.0;
        light.Diffuse.a = 1.0;
        light.Ambient.r = 1.0;
        light.Ambient.g = 0.0;
        light.Ambient.b = 0.0;
        light.Ambient.a = 1.0;
        light.Specular.r = 1.0;
        light.Specular.g = 0.0;
        light.Specular.b = 0.0;
        light.Specular.a = 1.0;
        light.Position.x = player->position.x;
        light.Position.y = player->position.y + 10.0;
        light.Position.z = player->position.z;
        light.Range = 500.0;
        light.Attenuation0 = 0.0;
        light.Attenuation1 = 0.5;
        light.Attenuation2 = 2.0 / (light.Range * light.Range);
        
        auto lightIndex = 1;
        (*d3dDevice)->lpVtbl->LightEnable(*d3dDevice, lightIndex, 1);
        (*d3dDevice)->lpVtbl->SetLight(*d3dDevice, lightIndex, &light);
    }
    
    reinterpret_cast<decltype(EnableNrelPointLight)*>(0x008060fc)();
}

bool __cdecl LoadSnowMapStuff()
{
    PatchIcecubeUnitxt();
    *reinterpret_cast<decltype(EnableNrelPointLight)**>(0x00b475f8) = EnableNrelPointLight;
    return true;
}

CustomMapDefinition snowMapEntry = {
    .assetPrefixes = {
        &snowMapAssetPrefixStrings,
        1
    },
    .setDataTable = {
        &snowSetDataTableInner1,
        1
    },
    .mapLoader = {
        snowMapName,
        LoadSnowMapStuff,
        nullptr
    },
    .songFilename = "slbgm_snow.ogg",
    .slbgmFilePath = "data/slbgm_snow.txt",
    .pacFilename = "snow.pac",
    .pacMetadataFilename = "snow_pac.txt",
    .slbgmIndex = 0,
    .allowedMonsters = {
        Enemy::GetEnemyDefinition(Enemy::NpcType::Booma),
        Enemy::GetEnemyDefinition(Enemy::NpcType::SavageWolf),
        SpawnableEntityWithCloneCount(1338, EnemyIcecube, 1)
    },
    .allowedObjects = {
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::PlayerSet1),
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::EventCollision),
        MapObject::GetMapObjectDefinition(MapObject::MapObjectType::FloatingJelifish),
        SpawnableEntity(1337, MapObjectCloud),
        SpawnableEntity(1338, MapObjectSnowfall),
        SpawnableEntity(1339, MapObjectNewdoor)
    }
};

#endif // PATCH_NEWMAP
