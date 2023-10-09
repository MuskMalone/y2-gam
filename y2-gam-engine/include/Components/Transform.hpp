#pragma once
#include <glm/glm.hpp>
#include <Core/Serialization/SerializationManager.hpp>


//world transform values
struct Transform
{
	glm::vec3 position{};
	glm::vec3 rotation{};
	glm::vec3 scale{};

	Transform() = default;
	Transform(glm::vec3 const& pos, glm::vec3 const& rot, glm::vec3 const& sca) : position{ pos }, rotation{ rot }, scale{ sca } {}
	Transform(rapidjson::Value const& obj) {
		position = glm::vec3{ obj["posX"].GetFloat(), obj["posY"].GetFloat(), obj["posZ"].GetFloat() };
		rotation = glm::vec3{ obj["rotX"].GetFloat(), obj["rotY"].GetFloat(), obj["rotZ"].GetFloat() };
		scale = glm::vec3{ obj["scaleX"].GetFloat(), obj["scaleY"].GetFloat(), obj["scaleZ"].GetFloat() };
	}
	bool Serialize(rapidjson::Value& obj) {
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
		return true;


	}
};
