#pragma once
#pragma once

#include "Core/System.hpp"
#include <Core/Globals.hpp>
#include <utility>

class Event;


class InputSystem : public System
{
public:
	enum class KeyState {
		KEY_PRESSED = 0,
		KEY_CLICKED,
		KEY_RELEASED,
		MOUSE_PRESSED,
		MOUSE_CLICKED,
		MOUSE_RELEASED
	};
	void Init();
	bool CheckKey(KeyState state, size_t key) const;
	void Update();

private:
	std::bitset<ENGINE_KEYS_COUNT> mButtonsPressed;
	std::bitset<ENGINE_KEYS_COUNT> mButtonsClicked;
	std::bitset<ENGINE_KEYS_COUNT> mButtonsReleased;
	std::bitset<ENGINE_MOUSEKEYS_COUNT> mMouseButtonsPressed;	
	std::bitset<ENGINE_MOUSEKEYS_COUNT> mMouseButtonsClicked;
	std::bitset<ENGINE_MOUSEKEYS_COUNT> mMouseButtonsReleased;
	MousePosition mMousePos;
	void InputListener(Event& event);
};