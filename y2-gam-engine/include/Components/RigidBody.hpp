#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       RigidBody.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      rigidbody ecs cmpnt

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include <Core/Globals.hpp>
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>
#include <Math/MathUtils.h>

struct RigidBody
{	
	//dimension, position, and rotation are merely here to push values from collider
	//to modify the drawn properties, u should be referring to Transform.hpp
	//to modify the collidable dimensions, u should be referring to Collider.hpp

	Vec2 dimension{}; //if collider is circle, both values of dimension is radius
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
	RigidBody(Vec2 pos, float rotation, float mass, Vec2 dimension, bool rotate = false) {
		this->position = pos;
		this->rotation = rotation;
		this->dimension = dimension;
		this->mass = mass;
		this->isLockRotation = rotate;
		this->friction = 0.9f;

		SetMass(mass);
	}
	RigidBody(rapidjson::Value const& obj) : RigidBody{
		Vec2{ obj["posX"].GetFloat(), obj["posY"].GetFloat() },
		obj["rotation"].GetFloat(), obj["mass"].GetFloat(), 
		Vec2{ obj["dimX"].GetFloat(), obj["dimY"].GetFloat() },
		obj["lockRotate"].GetBool()
	} {}
	void SetMass(float m) {
		mass = m;
		if (mass < FLOAT_MAX && mass > FLT_EPSILON) {
			invMass = 1.0f / mass;
			inertia = mass * (dimension.x * dimension.x + dimension.y * dimension.y) / 12.0f;
			invInertia = 1.0f / inertia;
		}
		else {
			mass = FLOAT_MAX;
			inertia = FLOAT_MAX;
		}
	}
	bool Serialize(rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

		sm->InsertValue(obj, "dimX", dimension.x);
		sm->InsertValue(obj, "dimY", dimension.y);
		sm->InsertValue(obj, "rotation", rotation);
		sm->InsertValue(obj, "mass", mass);
		sm->InsertValue(obj, "posX", position.x);
		sm->InsertValue(obj, "posY", position.y);
		sm->InsertValue(obj, "lockRotate", isLockRotation);
		return true;
	}
};
