#pragma once

#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>
#include "Core/Types.hpp"

struct Node {
	std::set<Entity> neighbours;

	Node() = default;

	Node([[maybe_unused]] rapidjson::Value const& obj) {}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};