#pragma once

#include "Math/Vec2.hpp"


struct Gravity
{
	Vec2 force;
	Gravity() = default;
	Gravity(Vec2 const& v) : force{ v } {}
	Gravity(rapidjson::Value const& obj) {}
};
