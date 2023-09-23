#pragma once

#include "Math/Vec2.hpp"
#include <Core/Globals.hpp>
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>


struct RigidBody
{
	Vec2 dimension{};
	Vec2 position{};
	float rotation{};

	float angularVelocity{};

	Vec2 force{};
	float torque{};

	float friction{};
	float mass{}, invMass{};
	Vec2 velocity{};

	float inertia{}, invInertia{};

	bool isLockRotation{ false };
	bool isGrounded{ false };

	Vec2 acceleration{};
	RigidBody() = default;
	RigidBody(Vec2 pos, float rotation, float mass, Vec2 dimension, bool rotate = true) {
		this->position = pos;
		this->rotation = rotation;
		this->dimension = dimension;
		this->mass = mass;
		this->isLockRotation = rotate;
		this->friction = 0.2f;

		if (mass < FLOAT_MAX) {
			invMass = 1.0f / mass;
			inertia = mass * (dimension.x * dimension.x + dimension.y * dimension.y) / 12.0f;
			invInertia = 1.0f / inertia;
		}
		else {
			inertia = FLOAT_MAX;
		}
	}
	RigidBody(rapidjson::Value const& obj) : RigidBody{
		Vec2{ obj["posX"].GetFloat(), obj["posY"].GetFloat() },
		obj["rotation"].GetFloat(), obj["mass"].GetFloat(), 
		Vec2{ obj["dimX"].GetFloat(), obj["dimY"].GetFloat() },
		obj["lockRotate"].GetBool()
	} {}
	void Serialize(rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

		sm->InsertValue(obj, "dimX", dimension.x);
		sm->InsertValue(obj, "dimY", dimension.y);
		sm->InsertValue(obj, "rotation", rotation);
		sm->InsertValue(obj, "mass", mass);
		sm->InsertValue(obj, "posX", position.x);
		sm->InsertValue(obj, "posY", position.y);
		sm->InsertValue(obj, "lockRotate", isLockRotation);

	}
};
