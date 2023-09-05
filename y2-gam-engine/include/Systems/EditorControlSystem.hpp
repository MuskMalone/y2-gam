#pragma once

#include "Core/System.hpp"
#include <Core/Globals.hpp>

class Event;


class EditorControlSystem : public System
{
public:
	void Init();

	void Update(float dt);
};
