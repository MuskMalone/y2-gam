/******************************************************************************/
/*!
\par        Image Engine
\file       Gravity.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      gravity ecs cmpnt

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "Math/MathUtils.h"
#include <Core/Serialization/SerializationManager.hpp>
#include <Math/MathUtils.h>

struct Gravity
{
	Vec2 force;
	Gravity() = default;
	Gravity(Vec2 const& v) : force{ v } {}
	Gravity(rapidjson::Value const& obj) {
		force = Vec2{ obj["x"].GetFloat(), obj["y"].GetFloat() };
	}
	bool Serialize(rapidjson::Value& obj) {
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "x", force.x);
		Serializer::SerializationManager::GetInstance()->InsertValue(obj, "y", force.y);
		return true;

	}
};
