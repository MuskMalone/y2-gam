/******************************************************************************/
/*!
\par        Image Engine
\file       Layering.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 10, 2023

\brief      Defines the layer component, which facilitates the layering of
            objects in the game world.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <Core/Serialization/SerializationManager.hpp>

struct Layering {
	std::string assignedLayer;

	Layering() = default;
	Layering(std::string const& al) : assignedLayer{ al } {}
	Layering(rapidjson::Value const& obj) {
		assignedLayer = obj["layering"].GetString();
	}

	bool Serialize(rapidjson::Value& obj) {
		std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

		sm->InsertValue(obj, "layering", assignedLayer);

		return true;
	}
};