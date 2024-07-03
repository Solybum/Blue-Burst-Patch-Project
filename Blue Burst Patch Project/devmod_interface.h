#pragma once

#include <windows.h>

#include "helpers.h"
#include "common.h"
#include "map_object.h"

template <typename... InitFnArg>
class DevmodInterface
{
private:
    HMODULE mod;
    void (*fnPtrInit)(InitFnArg...);
    void (*fnPtrUninit)();

    void Unload()
    {
        if (fnPtrUninit) fnPtrUninit();

        if (mod)
        {
            FreeLibrary(mod);
            mod = nullptr;
        }

        fnPtrInit = nullptr;
        fnPtrUninit = nullptr;
    }

    void Reload(InitFnArg... args)
    {
        Unload();

        Log(ToWideString("LoadLibrary").c_str());
        mod = LoadLibrary("devmod.dll");
        if (!mod) return;

        fnPtrInit = reinterpret_cast<decltype(fnPtrInit)>(GetProcAddress(mod, "devmodInit"));
        fnPtrUninit = reinterpret_cast<decltype(fnPtrUninit)>(GetProcAddress(mod, "devmodUninit"));

        if (fnPtrInit) fnPtrInit(args...);
    }

public:
    DevmodInterface(InitFnArg... args) :
        mod(nullptr),
        fnPtrInit(nullptr),
        fnPtrUninit(nullptr)
    {
        Reload(args...);
    }
    
    ~DevmodInterface()
    {
        Unload();
    }
};

struct MapObjectProxyInterface
{
    void (__cdecl *LoadAssets)();
    void (__cdecl *UnloadAssets)();
    void* (__cdecl *Create)(MapObject::InitData::InnerData* initData);
    void (*Destruct)(void*, BOOL freeMemory);
    void (*Update)(void*);
    void (*Render)(void*);
    void (*RenderShadow)(void*);
};

class MapObjectDevmodInterface : DevmodInterface<MapObjectProxyInterface*>
{
public:
    MapObjectProxyInterface proxyInterface;
    
    MapObjectDevmodInterface() : DevmodInterface(&proxyInterface) {}
};

template <MapObjectDevmodInterface** devmod>
class MapObjectDevmodProxy : MapObject::MapObjectBase
{
public:
    static void* wrappedObj;

    static void __cdecl LoadAssets()
    {
        (*devmod)->proxyInterface.LoadAssets();
    }

    static void __cdecl UnloadAssets()
    {
        (*devmod)->proxyInterface.UnloadAssets();
    }
    
    static void* __cdecl Create(MapObject::InitData::InnerData* initData)
    {
        MapObjectDevmodProxy::wrappedObj = (*devmod)->proxyInterface.Create(initData);
        return MapObjectDevmodProxy::wrappedObj;
    }

    void Destruct(BOOL freeMemory)
    {
        (*devmod)->proxyInterface.Destruct(MapObjectDevmodProxy::wrappedObj, freeMemory);
        MapObjectDevmodProxy::wrappedObj = nullptr;
    }
    
    void Update()
    {
        (*devmod)->proxyInterface.Update(MapObjectDevmodProxy::wrappedObj);
    }

    void Render()
    {
        (*devmod)->proxyInterface.Render(MapObjectDevmodProxy::wrappedObj);
    }
    
    void RenderShadow()
    {
        (*devmod)->proxyInterface.RenderShadow(MapObjectDevmodProxy::wrappedObj);
    }
};
