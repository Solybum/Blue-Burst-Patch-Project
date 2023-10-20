#ifdef PATCH_NEWMAP

#include <cstdint>
#include <new>
#include <d3d8.h>
#include "map_object_cloud.h"
#include "map_object.h"
#include "map.h"
#include "object_extension.h"
#include "common.h"

auto LoadXvm = reinterpret_cast<void* (__cdecl *)(const char* filename)>(0x008009dc);
auto FreeXvm = reinterpret_cast<BOOL (__fastcall *)(void* xvm)>(0x00829de0);

#pragma pack(push, 1)
struct Njtl
{
    struct
    {
        const char* filename;
        void* unk1;
        void* unk2;
    }* textures;
    uint32_t textureCount;
};

struct BmlMetadata
{
    const char* bmlFilename;
    const char** modelFilenames;
    const char** animationFilenames;
    const uint16_t modelCount;
    const uint16_t animationCount;
};
#pragma pack(pop)

auto LoadBml = reinterpret_cast<void* (__thiscall *)(void* result, uint32_t unk1, BmlMetadata* bmlMeta, void* unk2, uint32_t unk3, void* optionalDstBuf)>(0x005b7a84);
auto FreeBml = reinterpret_cast<void (__thiscall *)(void* bml, uint32_t doFree)>(0x004066a8);

auto GetModel = reinterpret_cast<void* (__thiscall *)(void* bml, uint32_t index)>(0x005b8610);

BmlMetadata bmlMeta = {
    "bm_obj_cloud.bml",
    nullptr,
    nullptr,
    1,
    0
};

void* cloudXvm = nullptr;
void* bml = nullptr;

void __cdecl Cloud::LoadAssets()
{
    cloudXvm = LoadXvm("bm_object_cloud.xvm");

    bml = reinterpret_cast<void* (__cdecl*)(uint32_t size)>(0x008581c5)(0x440);
    LoadBml(bml, 5, &bmlMeta, nullptr, 0, nullptr);

    auto PrepareBmlMeshes = reinterpret_cast<void (__thiscall *)(void* bml)>(0x005b90d0);
    PrepareBmlMeshes(bml);
}

void __cdecl Cloud::UnloadAssets()
{
    if (cloudXvm)
    {
        FreeXvm(cloudXvm);
        cloudXvm = nullptr;
    }

    if (bml) {
        FreeBml(bml, 1);
        bml = nullptr;
    }
}

Cloud::Cloud(void* parentObject, MapObject::InitData::InnerData* initData)
    : texCoordX(0),
      MapObject::MapObjectBase(parentObject)
{
    OVERRIDE_METHOD(Cloud, Destruct);
    OVERRIDE_METHOD(Cloud, Update);
    OVERRIDE_METHOD(Cloud, Render);

    vtable->ApplyInitData(this, initData);
}

void Cloud::Destruct(BOOL freeMemory)
{
    MapObjectBase::Destruct(false);

    // Manually call own C++ destructor
    Cloud::~Cloud();

    if (freeMemory)
    {
        MainArenaDealloc(this);
    }
}

void Cloud::Update()
{

    struct Player {
        union
        {
            union {
                DEFINE_FIELD(0x28, int16_t room);
                DEFINE_FIELD(0x38, float x);
                DEFINE_FIELD(0x3c, float y);
                DEFINE_FIELD(0x40, float z);
            };
            uint8_t _padding[0x500];
        };
    };

    auto player = *((Player**)0x00a94254);

    xyz2.x = player->x;
    xyz2.y = 100.0;
    xyz2.z = player->z;

    texCoordX++;
}

void Cloud::Render()
{
    auto model = GetModel(bml, 0);

    Transform::PushTransformStackCopy();
    Transform::TranslateTransformStackHead(const_cast<Vec3<float>*>(&xyz2));
    Transform::RotateMatrix(nullptr, rotation.y);

    auto UseTextureArchive = reinterpret_cast<void (__thiscall *)(void* tex)>(0x00829cc8);
    auto RenderXj = reinterpret_cast<void (__fastcall *)(void* model)>(0x00835090);

    float translation = 0.0001 * (float)texCoordX;
    D3DMATRIX mx = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        translation, translation, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };

    UseTextureArchive(cloudXvm);
    (*d3dDevice)->lpVtbl->SetTextureStageState(*d3dDevice, 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    (*d3dDevice)->lpVtbl->SetTransform(*d3dDevice, D3DTS_TEXTURE0, &mx);
    RenderXj(model);

    Transform::PopTransformStack();
}

Cloud* __cdecl Cloud::Create(MapObject::InitData::InnerData* initData)
{
    void* buf = MainArenaAlloc(sizeof(Cloud));
    Cloud* cloud = new (buf) Cloud(*MapObject::rootMapObject, initData);
    return cloud;
}

void EnableMapObjectCloud()
{
    auto& forest1InitList = Map::GetMapInitList(Map::MapType::Forest1);
    forest1InitList.AddFunctionPair(InitList::FunctionPair(Cloud::LoadAssets, Cloud::UnloadAssets));

    *reinterpret_cast<decltype(Cloud::Create)**>(0x00a0dd44) = Cloud::Create;
}

#endif
