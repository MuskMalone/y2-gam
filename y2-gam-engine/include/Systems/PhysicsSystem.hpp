#pragma once

#include "Core/System.hpp"


class PhysicsSystem : public System
{
public:
	void Init();

	void PreCollisionUpdate(float dt);
	void PostCollisionUpdate(float dt);
};
