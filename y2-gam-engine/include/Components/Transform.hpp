#pragma once

#include "Math/Vec3.hpp"
#include "Math/Mat44.hpp"
#include <Core/Serialization/SerializationManager.hpp>


//world transform values
struct Transform
{
	Vec3 position{};
	Vec3 rotation{};
	Vec3 scale{};

	Transform() = default;
	Transform(Vec3 const& pos, Vec3 const& rot, Vec3 const& sca) : position{ pos }, rotation{ rot }, scale{ sca } {}
	Transform(rapidjson::Value const& obj) {
		position = Vec3{ obj["posX"].GetFloat(), obj["posY"].GetFloat(), obj["posZ"].GetFloat() };
		rotation = Vec3{ obj["rotX"].GetFloat(), obj["rotY"].GetFloat(), obj["rotZ"].GetFloat() };
		scale = Vec3{ obj["scaleX"].GetFloat(), obj["scaleY"].GetFloat(), obj["scaleZ"].GetFloat() };
	}
	void Serialize(rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

		sm->InsertValue(obj, "posX", position.x);
		sm->InsertValue(obj, "posY", position.y);
		sm->InsertValue(obj, "posZ", position.z);

		sm->InsertValue(obj, "rotX", rotation.x);
		sm->InsertValue(obj, "rotY", rotation.y);
		sm->InsertValue(obj, "rotZ", rotation.z);

		sm->InsertValue(obj, "scaleX", scale.x);
		sm->InsertValue(obj, "scaleY", scale.y);
		sm->InsertValue(obj, "scaleZ", scale.z);

	}
};
