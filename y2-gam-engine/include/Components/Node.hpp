/******************************************************************************/
/*!
\par        Image Engine
\file       Node.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 14, 2023

\brief      Defines the component for nodes, which are for AI pathfinding
						graphing.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>
#include "Core/Types.hpp"

struct Node {
	std::set<Entity> neighbours;
	bool selected;
	int type;

	Node() = default;

	Node(std::set<Entity> n, bool s) : neighbours{ n }, selected{ s }, type{ 0 } {}

	Node([[maybe_unused]] rapidjson::Value const& obj) : selected{ obj["selected"].GetBool() }, type{ obj["type"].GetInt() } {
		const auto arr = obj["neighbours"].GetArray();
		for (auto const& v : arr) {
       neighbours.insert(v.GetUint());
		}
	}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
		sm->InsertValue(obj, "selected", selected);

		rapidjson::Value objArr{ rapidjson::kArrayType };
		objArr.SetArray();
		for (auto& v : neighbours) {
			sm->PushToArray(objArr, v);
		}

		sm->InsertValue(obj, "neighbours", objArr);
		sm->InsertValue(obj, "type", type);

		return true;
	}
};