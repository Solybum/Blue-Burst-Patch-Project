#pragma once

#include <cstddef>
#include <set>

namespace Hooking
{
    using HookFn = void (*)();

    class Hook
    {
    private:
        struct PrivateCtorMarker {};
        size_t callAddrIn;
        size_t callAddrOut;
        std::set<HookFn> callbacks;
        void (*__cdecl callbackCaller)();
        friend Hook& CreateHook(size_t callAddrIn, size_t callAddrOut);

    public:
        // Constructor must be public to be able to use this class with std containers,
        // but it is not constructible without access to private member
        Hook(PrivateCtorMarker thisConstructorIsPrivate, size_t callAddrIn, size_t callAddrOut);
        void AddCallback(HookFn func);
        void RemoveCallback(HookFn func);
        void Install() const;
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

    void InstallAllHooks();

    // Additional common hooks can be added here:

    /**
     * @brief Called every frame after everything has been updated and rendered.
     */
    extern Hook& afterSceneUpdate;

    /**
     * @brief Wraps a capturing lambda function in a way that allows it to be cast to a function pointer.
     */
    template<typename L>
    auto LambdaPointer(L&& lambda) {
        // Type signatures of lambdas are never identical even if their call signatures are.
        // Therefore, a new specialization of this function - along with the following static variable -
        // is created for each new lambda that is passed as an argument.
        static L fn = std::forward<L>(lambda);
        return []() {
            return fn();
        };
    }
};
