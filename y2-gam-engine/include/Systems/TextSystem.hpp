#pragma once

#include "Core/System.hpp"

class TextSystem : public System
{
public:
	void Init();
	void Update();
	void Exit();

	static Vec2 WorldToScreenCoordinates(Vec2 worldCoordinates);

private:

};