#pragma once

#include "Math/MathUtils.h"
#include <Core/Serialization/SerializationManager.hpp>
#include <Math/MathUtils.h>
#include <rapidjson/document.h>
struct Light
{
	glm::vec3 color;
	glm::vec2 pos;
	float radius;
	float intensity;
	Light() = default;
	Light(glm::vec3 const& col, glm::vec2 const& p, float const& rad, float const& intense) : color{ col }, pos{ p }, radius{ rad }, intensity{ intense } {}
	Light(rapidjson::Value const& obj) {
		color = glm::vec3{ obj["colx"].GetFloat(), obj["coly"].GetFloat(), obj["colz"].GetFloat() };
		pos = glm::vec2{ obj["posx"].GetFloat(), obj["posy"].GetFloat() };
		radius = obj["radius"].GetFloat();
		intensity = obj["intensity"].GetFloat();
	}
	bool Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "colx", color.x);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "coly", color.y);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "colz", color.z);

		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "posx", pos.x);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "posy", pos.y);

		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "radius", radius);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "intensity", intensity);

		return true;

	}

};
