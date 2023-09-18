#pragma once

#include "Math/Vec2.hpp"
#include <Core/Globals.hpp>
#include <rapidjson/document.h>

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
	RigidBody(rapidjson::Value const& obj) {

	}
	void Serialize(rapidjson::Value& obj) {

	}
};
