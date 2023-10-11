#pragma once

#include <vector>
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>

struct Node {
	std::vector<Node> neighbours;

	Node() = default;

	Node([[maybe_unused]] rapidjson::Value const& obj) {}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};