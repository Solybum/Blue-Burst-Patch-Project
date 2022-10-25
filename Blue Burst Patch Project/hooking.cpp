#ifdef PATCH_HOOKS

#include <unordered_map>

#include "hooking.h"
#include "helpers.h"

namespace Hooking
{
    /// Store created hooks here, one per address.
    std::unordered_map<size_t, Hook> createdHooks;

    // Initialize common hooks
    Hook& afterSceneUpdate = CreateHook(0x7a5f23, 0x7a5f2a, []() {
        // Original code
        *reinterpret_cast<int*>(0x00a9c4ec) += 1;
    });    

    Hook::Hook(PrivateCtorMarker priv, size_t callAddrIn, size_t callAddrOut)
        : callAddrIn(callAddrIn), callAddrOut(callAddrOut)
    {
        // To minimize the number of bytes that have to be overwritten by the hook
        // we want the callback calling function to not take any arguments.
        // Instead, we embed the callback list inside the function with a closure.
        callbackCaller = LambdaPointer([this]() {
            for (auto& fn : this->callbacks) fn();
        });
    }

    void Hook::Install() const
    {
        PatchCALL(callAddrIn, callAddrOut, reinterpret_cast<int>(callbackCaller));
    }

    Hook& CreateHook(size_t callAddrIn, size_t callAddrOut)
    {
        // Find existing or construct new
        auto&& [entry, _] = createdHooks.try_emplace(callAddrIn, Hook::PrivateCtorMarker{}, callAddrIn, callAddrOut);
        return (*entry).second;
    }

    Hook& CreateHook(size_t callAddrIn, size_t callAddrOut, HookFn initialCallback)
    {
        auto& hook = CreateHook(callAddrIn, callAddrOut);
        hook.AddCallback(initialCallback);
        return hook;
    }

    void Hook::AddCallback(HookFn func)
    {
        callbacks.insert(func);
    }

    void Hook::RemoveCallback(HookFn func)
    {
        callbacks.erase(func);
    }

    bool Hook::operator<(const Hook& right) const
    {
        return this->callAddrIn < right.callAddrIn;
    }

    void InstallAllHooks()
    {
        for (const auto& [_, hook] : createdHooks)
        {
            hook.Install();
        }
    }
};

#endif // PATCH_HOOKS
