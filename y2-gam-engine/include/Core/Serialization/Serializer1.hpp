#pragma once
#include <Core/Types.hpp>
#include <functional>
#include <Core/Coordinator.hpp>
#include <rapidjson/document.h>
#include <string>
#include <map>
#include <Components/RigidBody.hpp>

namespace Serializer {
	static void EntityAddRigidBody(Entity const& entity, rapidjson::Value const& obj) {
		Coordinator::GetInstance()->AddComponent(entity, RigidBody{ obj });
	}
	template <typename _type>
	std::string TypeToString() {
		if constexpr (std::is_same_v<T, RigidBody>) return "RigidBody";
		return "NULL";
	}
	static const std::map<std::string, std::function<void(Entity const&, rapidjson::Value const&)>> gComponentSerializer{
		{"RigidBody", EntityAddRigidBody}
	};
}
