#pragma once

#include "Math/MathUtils.h"
#include <Core/Serialization/SerializationManager.hpp>
#include <Math/MathUtils.h>
#include <rapidjson/document.h>
struct LightBlocker
{
	glm::vec2 dimension;
	glm::vec2 position;
	float rotation;
	LightBlocker() = default;
	LightBlocker(glm::vec2 const& dim, glm::vec2 const& pos, float rot) : dimension{ dim }, position{ pos }, rotation{ rot } {}
	LightBlocker(rapidjson::Value const& obj) {
		dimension = glm::vec2{ obj["dimx"].GetFloat(), obj["dimy"].GetFloat() };
		position = glm::vec2{ obj["posx"].GetFloat(), obj["posy"].GetFloat() };
		rotation = obj["rotation"].GetFloat();
	}
	bool Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "dimx", dimension.x);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "dimy", dimension.y);

		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "posx", position.x);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "posy", position.y);

		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "rotation", rotation);
		return true;

	}
};
