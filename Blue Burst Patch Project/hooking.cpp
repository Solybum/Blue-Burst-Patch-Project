#ifdef PATCH_HOOKS

#include <algorithm>
#include <cstdint>

#include "hooking.h"
#include "helpers.h"

namespace Hooking
{
    std::unordered_map<size_t, Hook> Hook::createdHooks;

    // Initialize common hooks
    Hook& afterSceneUpdate = CreateHook<0x7a5f23, 0x7a5f2a>([]() {
        // Original code
        *reinterpret_cast<int*>(0x00a9c4ec) += 1;
    });    

    Hook::Hook(PrivateCtorMarker priv, size_t callAddrIn, size_t callAddrOut)
        : callAddrIn(callAddrIn), callAddrOut(callAddrOut) {}

    void Hook::Install() const
    {
        PatchCALL(callAddrIn, callAddrOut, reinterpret_cast<int>(callbackCaller));
    }

    void Hook::AddCallback(HookFn func)
    {
        callbacks.push_back(func);
    }

    bool Hook::operator<(const Hook& right) const
    {
        return this->callAddrIn < right.callAddrIn;
    }

    void InstallAllHooks()
    {
        for (const auto& [_, hook] : Hook::createdHooks)
        {
            hook.Install();
        }
    }
};

#endif // PATCH_HOOKS
