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

	Node() = default;

	Node(std::set<Entity> n, bool s) : neighbours{ n }, selected{ s } {}

	Node([[maybe_unused]] rapidjson::Value const& obj) {
		// To be completed (Need to break down std::set)
	}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		// To be completed
		return false;
	}
};