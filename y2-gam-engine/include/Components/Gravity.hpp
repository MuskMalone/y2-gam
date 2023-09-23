#pragma once

#include "Math/Vec2.hpp"
#include <Core/Serialization/SerializationManager.hpp>

struct Gravity
{
	Vec2 force;
	Gravity() = default;
	Gravity(Vec2 const& v) : force{ v } {}
	Gravity(rapidjson::Value const& obj) {
		force = Vec2{ obj["x"].GetFloat(), obj["y"].GetFloat() };
	}
	bool Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "x", force.x);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "y", force.y);
		return true;

	}
};
