#pragma once

#include <cstddef>
#include <functional>
#include <unordered_map>
#include <vector>

namespace Hooking
{
    using HookFn = std::function<void ()>;

    class Hook
    {
    private:
        struct PrivateCtorMarker {};

        size_t callAddrIn;
        size_t callAddrOut;
        std::vector<HookFn> callbacks;
        /**
         * @brief This is the actual function that gets patched in
         */
        void (__cdecl *callbackCaller)();

        template<size_t, size_t> friend Hook& CreateHook();
        friend void InstallAllHooks();

        /// Store created hooks here, one per address.
        static std::unordered_map<size_t, Hook> createdHooks;

        /**
         * @brief Used to generate the function that gets patched in
         */
        template<size_t addr>
        static void __cdecl CallHookCallbacks()
        {
            if (auto it = createdHooks.find(addr); it != Hook::createdHooks.end())
                for (auto cb : (*it).second.callbacks) cb();
        }

        /**
         * @brief Patches in the function call to the hook
         */
        void Install() const;

    public:
        // Constructor must be public to be able to use this class with std containers,
        // but it is not constructible without access to private member
        Hook(PrivateCtorMarker thisConstructorIsPrivate, size_t callAddrIn, size_t callAddrOut);
        void AddCallback(HookFn func);
        /**
         * @brief Needed for inserting Hooks into std containers
         */
        bool operator<(const Hook& right) const;
    };

    /**
     * @brief Creates a hook that allows adding callback functions at the specified address.
     * The type parameters must be integer literals.
     * At least 5 bytes will be overwritten at the address.
     * It is the caller's responsibility to ensure the code will be valid after the overwrite.
     */
    template<size_t callAddrIn, size_t callAddrOut>
    Hook& CreateHook()
    {
        // Find existing or construct new
        auto&& [entry, _] = Hook::createdHooks.try_emplace(callAddrIn, Hook::PrivateCtorMarker{}, callAddrIn, callAddrOut);
        // Create the callback calling function
        auto& hook = (*entry).second;
        hook.callbackCaller = Hook::CallHookCallbacks<callAddrIn>;
        return hook;
    }

    /**
     * @brief Immediately adds the given callback to the hook.
     * Useful for restoring the original code overwritten by the hook.
     */
    template<size_t callAddrIn, size_t callAddrOut>
    Hook& CreateHook(HookFn initialCallback)
    {
        auto& hook = CreateHook<callAddrIn, callAddrOut>();
        hook.AddCallback(initialCallback);
        return hook;
    }

    /**
     * @brief Patches in the CALLs to all created hooks.
     */
    void InstallAllHooks();

    // Additional common hooks can be added here:

    /**
     * @brief Called every frame after everything has been updated and rendered.
     */
    extern Hook& afterSceneUpdate;
};
