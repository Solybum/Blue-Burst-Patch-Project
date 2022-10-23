#include <unordered_map>

#include "hooking.h"
#include "helpers.h"

/**
 * @brief Wraps a capturing lambda function in a way that allows it to be cast to a function pointer.
 */
template<typename L>
auto lambdaPointer(L&& lambda) {
    // Type signatures of lambdas are never identical even if their call signatures are.
    // Therefore, a new specialization of this function - along with the following static variable -
    // is created for each new lambda that is passed as an argument.
    static L fn = std::forward<L>(lambda);
    return []() {
        return fn();
    };
}

namespace Hooking
{
    // Initialize common hooks
    Hook& afterSceneUpdate = CreateHook(0x7a5f23, 0x7a5f2a, []() {
        // Original code
        *reinterpret_cast<int*>(0x00a9c4ec) += 1;
    });


    /// Store created hooks here, one per address.
    std::unordered_map<size_t, Hook> createdHooks;

    Hook::Hook(size_t callAddrIn, size_t callAddrOut)
    {
        // To minimize the number of bytes that have to be overwritten by the hook
        // we want the callback calling function to not take any arguments.
        // Instead, we embed the callback list inside the function with a closure.
        callbackCaller = lambdaPointer([this]() {
            for (auto func : this->callbacks) func();
        });

        PatchCALL(callAddrIn, callAddrOut, reinterpret_cast<int>(callbackCaller));
    }

    Hook& CreateHook(size_t callAddrIn, size_t callAddrOut)
    {
        // Find existing or construct new
        auto&& [entry, _] = createdHooks.try_emplace(callAddrIn, callAddrIn, callAddrOut);
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
};
