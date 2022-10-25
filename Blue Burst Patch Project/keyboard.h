#pragma once

#include <cstdint>
#include <vector>

#include "hooking.h"

namespace Keyboard
{
    enum class Keycode : uint8_t 
    {
        None = 0x0,
        Home = 0x6,
        End = 0x7,
        PageUp = 0x8,
        PageDown = 0x9,
        ScrLk = 0xa,
        A = 0x10,
        B = 0x11,
        C = 0x12,
        D = 0x13,
        E = 0x14,
        F = 0x15,
        G = 0x16,
        H = 0x17,
        I = 0x18,
        J = 0x19,
        K = 0x1a,
        L = 0x1b,
        M = 0x1c,
        N = 0x1d,
        O = 0x1e,
        P = 0x1f,
        Q = 0x20,
        R = 0x21,
        S = 0x22,
        T = 0x23,
        U = 0x24,
        V = 0x25,
        W = 0x26,
        X = 0x27,
        Y = 0x28,
        Z = 0x29,
        F1 = 0x40,
        F2 = 0x41,
        F3 = 0x42,
        F4 = 0x43,
        F5 = 0x44,
        F6 = 0x45,
        F7 = 0x46,
        F8 = 0x47,
        F9 = 0x48,
        F10 = 0x49,
        F11 = 0x4a,
        F12 = 0x4b,
        Escape = 0x4c,
        Delete = 0x4e,
        Backspace = 0x50,
        Space = 0x59,
        ArrowLeft = 0x5c,
        ArrowDown = 0x5d,
        ArrowUp = 0x5e,
        ArrowRight = 0x5f,
        Enter = 0x61,
        PrtScn = 0x77,

        NumLock = 0x66,
        NumpadDivide = 0x3e,
        NumpadMultiply = 0x62,
        NumpadSubtract = 0x64,
        NumpadAdd = 0x63,
        NumpadComma = 0x65,
        Numpad0 = 0x67,
        Numpad1 = 0x68,
        Numpad2 = 0x69,
        Numpad3 = 0x70,
        Numpad4 = 0x71,
        Numpad5 = 0x72,
        Numpad6 = 0x73,
        Numpad7 = 0x74,
        Numpad8 = 0x75,
        Numpad9 = 0x76,

        // These don't exist in the original enum but we'll fake them through wrapper functions
        Ctrl,
        Alt,
        Shift
    };

    /**
     * @brief Returns true during one frame after the specified key was pressed down.
     * Returns false while the key is held down.
     */
    bool wasPressed(Keycode key);

#ifdef PATCH_KEYBOARD_HOOKS
    /**
     * @brief Calls the callback when the key is pressed down.
     */
    void onKeyDown(Keycode key, Hooking::HookFn callback);

    /**
     * @brief Calls the callback when all of the keys are pressed down.
     */
    void onKeyDown(std::vector<Keycode> keys, Hooking::HookFn callback);
#endif // PATCH_KEYBOARD_HOOKS
};
