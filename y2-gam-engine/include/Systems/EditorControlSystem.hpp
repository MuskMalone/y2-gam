#pragma once

#include "Core/System.hpp"
#include <Core/Globals.hpp>

class Event;


class EditorControlSystem : public System
{
public:
	void Init();

	void Update(float dt);

private:
	std::bitset<ENGINE_KEYS_COUNT> mButtonsPressed;
	std::bitset<ENGINE_KEYS_COUNT> mButtonsClicked;

	void InputListener(Event& event);
};
