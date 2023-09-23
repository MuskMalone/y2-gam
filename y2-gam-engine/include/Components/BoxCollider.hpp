#pragma once
#include "Math/Vec2.hpp"
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>

struct BoxCollider {
	//this is just a struct to tell the collision system to consider this object for colliding
	//dimensions and everythign are in rigidbody
	BoxCollider() = default;
	BoxCollider(rapidjson::Value const& obj) {

	}
	void Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "collider", true);
	}
};