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
	ColliderType type;
	//this is just a struct to tell the collision system to consider this object for colliding
	//dimensions and everythign are in rigidbody
	Collider(ColliderType t = ColliderType::BOX) : type{t}{};
	Collider([[maybe_unused]] rapidjson::Value const& obj) : type{ static_cast<ColliderType>(obj["collider"].GetInt()) }{

	}
	bool Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "collider", static_cast<int>(type));
		return true;
	}
};