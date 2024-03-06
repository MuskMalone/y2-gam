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
		force = Vec2{ obj["x"].GetFloat(), obj["y"].GetFloat() };
	}
	bool Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "x", force.x);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "y", force.y);
		return true;

	}
};
