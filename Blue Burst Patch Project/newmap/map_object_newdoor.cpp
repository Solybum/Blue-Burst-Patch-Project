#ifdef PATCH_NEWMAP

#include "map_object_newdoor.h"
#include "ninja.h"
#include "psobb_functions.h"

static BmlMetadata bmlMeta = {
    "bm_obj_newdoor.bml",
    nullptr,
    nullptr,
    1,
    1
};

static void* xvm = nullptr;
static TBinModel* bml = nullptr;

void __cdecl MapObjectNewdoor::LoadAssets()
{
    xvm = LoadXvm("bm_obj_newdoor.xvm");

    bml = (TBinModel*) pf_malloc(sizeof(TBinModel));
    LoadBml(bml, 5, &bmlMeta, nullptr, 0, nullptr);
}

void __cdecl MapObjectNewdoor::UnloadAssets()
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

MapObjectNewdoor::MapObjectNewdoor(void* parentObject, MapObject::InitData::InnerData* initData)
    : animElapsed(0), MapObject::MapObjectBase(parentObject)
{
    OVERRIDE_METHOD(MapObjectNewdoor, Destruct);
    OVERRIDE_METHOD(MapObjectNewdoor, Update);
    OVERRIDE_METHOD(MapObjectNewdoor, Render);

    vtable->ApplyInitData(this, initData);
}

void MapObjectNewdoor::Destruct(BOOL freeMemory)
{
    MapObjectBase::Destruct(false);

    // Manually call own C++ destructor
    MapObjectNewdoor::~MapObjectNewdoor();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void MapObjectNewdoor::Update()
{
    animElapsed += 1;
    
    auto motion = GetMotion(bml, 0);
    if (animElapsed > motion->frameCount)
        animElapsed = 0;
}

void MapObjectNewdoor::Render()
{
    auto model = GetModel(bml, 0);
    auto motion = GetMotion(bml, 0);

    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(const_cast<Vec3<float>*>(&xyz2));
    Transform::RotateMatrix(nullptr, rotation.y);

    UseTextureArchive(xvm);

    RenderAnimatedXjUp(model, motion, animElapsed);

    Transform::PopTransformStack();
}

void* __cdecl MapObjectNewdoor::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(MapObjectNewdoor));
    MapObjectNewdoor* obj = new (buf) MapObjectNewdoor(*MapObject::rootMapObject, initData);
    return obj;
}

#endif
