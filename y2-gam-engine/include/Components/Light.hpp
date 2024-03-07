#pragma once

#include "Math/MathUtils.h"
#include <Core/Serialization/SerializationManager.hpp>
#include <Math/MathUtils.h>
#include <rapidjson/document.h>

struct Light
{
	Vec2 force;
	Light() = default;
	Light(Vec2 const& v) : force{ v } {}
	Light(rapidjson::Value const& obj) {
	}
	bool Serialize(rapidjson::Value& obj) {
		return false;

	}
};
