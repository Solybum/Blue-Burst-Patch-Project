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
        /**
         * @brief A wrapper for std::function that can be compared
         */
        struct ComparableFn
        {
            HookFn fn;
            bool operator<(const ComparableFn& right) const;
        };

        struct PrivateCtorMarker {};
        size_t callAddr;
        std::set<ComparableFn> callbacks;
        void (*__cdecl callbackCaller)();
        friend Hook& CreateHook(size_t callAddrIn, size_t callAddrOut);
        friend class std::allocator<Hook>;

    public:
        // Constructor must be public to be able to use this class with std containers,
        // but it is not constructible without access to private member
        Hook(PrivateCtorMarker thisConstructorIsPrivate, size_t callAddrIn, size_t callAddrOut);
        void AddCallback(HookFn func);
        void RemoveCallback(HookFn func);
        bool operator<(const Hook& right) const;
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
