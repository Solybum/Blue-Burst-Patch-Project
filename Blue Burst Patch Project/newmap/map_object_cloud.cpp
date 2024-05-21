#include "mathutil.h"
#ifdef PATCH_NEWMAP

#include <cstdint>
#include <new>
#include <d3d8.h>
#include "map_object_cloud.h"
#include "map_object.h"
#include "map.h"
#include "object_extension.h"
#include "common.h"
#include "player.h"
#include "ninja.h"
#include "psobb_functions.h"

static BmlMetadata bmlMeta = {
    "bm_obj_cloud.bml",
    nullptr,
    nullptr,
    1,
    0
};

static void* xvm = nullptr;
static TBinModel* bml = nullptr;

void __cdecl MapObjectCloud::LoadAssets()
{
    xvm = LoadXvm("bm_obj_cloud.xvm");

    bml = (TBinModel*) pf_malloc(sizeof(TBinModel));
    LoadBml(bml, 5, &bmlMeta, nullptr, 0, nullptr);

    PrepareBmlMeshes(bml);
}

void __cdecl MapObjectCloud::UnloadAssets()
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

MapObjectCloud::MapObjectCloud(void* parentObject, MapObject::InitData::InnerData* initData)
    : texCoordX(0),
      MapObject::MapObjectBase(parentObject)
{
    OVERRIDE_METHOD(MapObjectCloud, Destruct);
    OVERRIDE_METHOD(MapObjectCloud, Update);
    OVERRIDE_METHOD(MapObjectCloud, Render);

    vtable->ApplyInitData(this, initData);
}

void MapObjectCloud::Destruct(BOOL freeMemory)
{
    MapObjectBase::Destruct(false);

    // Manually call own C++ destructor
    MapObjectCloud::~MapObjectCloud();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void MapObjectCloud::Update()
{
    // Follow player to prevent getting culled by view distance
    auto player = GetPlayer(0);
    xyz2.set(player->position);

    texCoordX++;
}

void MapObjectCloud::Render()
{
    // Always render at 0,0,0
    Vec3f renderPosition = {0.0, 0.0, 0.0};
    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(&renderPosition);
    Transform::RotateMatrix(nullptr, rotation.y);

    UseTextureArchive(xvm);

    // Apply a 2D translation to texture coordinates to create a scrolling animation effect
    float translation = 0.0001 * (float)texCoordX;
    D3DMATRIX textureTranslationMatrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        translation, translation, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };

    // Apply transform
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    (*d3dDevice)->lpVtbl->SetTransform(*d3dDevice, D3DTS_TEXTURE0, &textureTranslationMatrix);

    auto model = GetModel(bml, 0);
    RenderXj(model);

    // Reset transform
    (*d3dDevice)->lpVtbl->SetTransform(*d3dDevice, D3DTS_TEXTURE0, Transform::identityMatrix);
    Transform::PopTransformStack();
}

void* __cdecl MapObjectCloud::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectCloud));
    MapObjectCloud* cloud = new (buf) MapObjectCloud(*MapObject::rootMapObject, initData);
    return cloud;
}

#endif
