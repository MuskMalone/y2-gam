#pragma once
#include <Core/Serialization/SerializationManager.hpp>

struct Prefab {
	size_t prefabId;
	Prefab() = default;
	Prefab(rapidjson::Value const& obj) : prefabId{ obj["prefabId"].GetUint64() } {}
	bool Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "prefabId", prefabId);
		return true;
	}
};