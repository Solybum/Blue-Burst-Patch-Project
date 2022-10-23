#pragma once

#include <cstddef>
#include <functional>
#include <set>

namespace Hooking
{
    using HookFn = std::function<void ()>;

    class Hook
    {
    private:
        Hook(size_t callAddrIn, size_t callAddrOut);
        std::set<HookFn> callbacks;
        void (*__cdecl callbackCaller)();
        friend Hook& CreateHook(size_t callAddr);

    public:
        void AddCallback(HookFn func);
        void RemoveCallback(HookFn func);
    };

    /**
     * @brief Creates a hook that allows adding callback functions at the specified address.
     * At least 5 bytes will be overwritten at the address.
     * It is the caller's responsibility to ensure the code will be valid after the overwrite.
     */
    Hook& CreateHook(size_t callAddrIn, size_t callAddrOut);

    /**
     * @brief Immediately adds the given callback to the hook.
     * Useful for restoring the original code overwritten by the hook.
     */
    Hook& CreateHook(size_t callAddrIn, size_t callAddrOut, HookFn initialCallback);


    // Additional common hooks can be added here:

    /**
     * @brief Called every frame after everything has been updated and rendered.
     */
    extern Hook& afterSceneUpdate;
};
