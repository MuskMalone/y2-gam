#pragma once
//all the impt stuff here
#include <Core/Types.hpp>
#include <functional>
//NOTE!
//THIS FILE IS GENERATED IN A PRE BUILD EVENT
//DO NOT MODIFY IT

#include <Core/Coordinator.hpp>
#include <rapidjson/document.h>
#include <string>
#include <map>
//all the components here
#include <Components/RigidBody.hpp>

namespace Serializer {
	//serialize entity to rigidbody
	static void EntityAddRigidBody(Entity const& entity, rapidjson::Value const& obj) {
		Coordinator::GetInstance()->AddComponent(entity, RigidBody{ obj });
	}

	//type to string
	template <typename _type>
	std::string TypeToString() {
		if constexpr (std::is_same_v<T, RigidBody>) return "RigidBody";
		return "NULL";
	}

	static const std::map<std::string, std::function<void(Entity const&, rapidjson::Value const&)>> gComponentSerializer{
		{"RigidBody", EntityAddRigidBody}
	};


}
