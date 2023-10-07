#pragma once

#include "Core/System.hpp"
#include <Core/Globals.hpp>

class Event;


class InputSystem : public System
{
public:
	enum class InputKeyState {
		KEY_PRESSED = 0,
		KEY_CLICKED,
		KEY_RELEASED,
		MOUSE_PRESSED,
		MOUSE_CLICKED,
		MOUSE_RELEASED
	};
	void Init();
	bool CheckKey(InputKeyState state, size_t key) const;
	void Update();

private:
	KeyState mButtonsPressed;
	KeyState mButtonsClicked;
	KeyState mButtonsReleased;
	MouseKeyState mMouseButtonsPressed;	
	MouseKeyState mMouseButtonsClicked;
	MouseKeyState mMouseButtonsReleased;
	MousePosition mMousePos;
	void InputListener(Event& event);
};