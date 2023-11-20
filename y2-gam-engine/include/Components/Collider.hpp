/******************************************************************************/
/*!
\par        Image Engine
\file       Collider.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      collider ecs cmpnt

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include "Math/MathUtils.h"
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>
enum class ColliderType : int{
	BOX = 0,
	CIRCLE
};
struct Collider {
	Vec2 dimension;
	Vec2 position;
	float rotation;
	ColliderType type;
	//this is just a struct to tell the collision system to consider this object for colliding
	//dimensions and everythign are in rigidbody
	Collider() = default;
	Collider(Vec2 const& pos, float const& rot, Vec2 const& dim, ColliderType t = ColliderType::BOX) : 
		dimension{ dim }, 
		position{ pos }, 
		rotation{ rot }, 
		type{ t } {};
	Collider([[maybe_unused]] rapidjson::Value const& obj) : Collider{
		Vec2{obj["posX"].GetFloat(), obj["posY"].GetFloat() },
		obj["rotation"].GetFloat(),
		Vec2{obj["dimX"].GetFloat(), obj["dimY"].GetFloat() },
		static_cast<ColliderType>(obj["collider"].GetInt())
	} {}
	bool Serialize(rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

		sm->InsertValue(obj, "collider", static_cast<int>(type));
		sm->InsertValue(obj, "posX", position.x);
		sm->InsertValue(obj, "posY", position.y);

		sm->InsertValue(obj, "rotation", rotation);

		sm->InsertValue(obj, "dimX", dimension.x);
		sm->InsertValue(obj, "dimY", dimension.y);
		return true;
	}
};