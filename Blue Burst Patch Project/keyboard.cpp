#include <algorithm>
#include <windows.h>

#include "keyboard.h"

namespace Keyboard
{
	auto keyboardState = reinterpret_cast<void*>(0xa9caa0);
	auto origWasKeyPressed = reinterpret_cast<BOOL (__cdecl *)(uint8_t keycode)>(0x7bfff8);
	auto origIsCtrlPressed = reinterpret_cast<BOOL (__fastcall *)(void* keyboardState)>(0x78f484);
	auto origIsAltPressed = reinterpret_cast<BOOL (__fastcall *)(void* keyboardState)>(0x0078f46c);
	auto origIsShiftPressed = reinterpret_cast<BOOL (__fastcall *)(void* keyboardState)>(0x0078f44c);


	bool wasPressed(Keycode key)
	{
		bool pressed = false;
		switch (key)
		{
			case Keycode::Ctrl:
				pressed = origIsCtrlPressed(keyboardState);
				break;
			case Keycode::Alt:
				pressed = origIsAltPressed(keyboardState);
				break;
			case Keycode::Shift:
				pressed = origIsShiftPressed(keyboardState);
				break;
			default:
				pressed = origWasKeyPressed(static_cast<uint8_t>(key));
				break;
		}
		return pressed;
	}

#ifdef PATCH_KEYBOARD_HOOKS
	void onKeyDown(Keycode key, Hooking::HookFn callback)
	{
		Hooking::afterSceneUpdate.AddCallback(Hooking::LambdaPointer([key, callback]() {
			if (wasPressed(key)) callback();
		}));
	}

	void onKeyDown(std::vector<Keycode> keys, Hooking::HookFn callback)
	{
		Hooking::afterSceneUpdate.AddCallback(Hooking::LambdaPointer([keys, callback]() {
			if (std::all_of(keys.begin(), keys.end(), wasPressed)) callback();
		}));
	}
#endif // PATCH_KEYBOARD_HOOKS
};
