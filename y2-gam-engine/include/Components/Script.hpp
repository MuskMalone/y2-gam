/******************************************************************************/
/*!
\par        Image Engine
\file       Script.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      A simple script component, with the name representing the
            script name.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <Core/Serialization/SerializationManager.hpp>

struct Script {
  std::string name;

	Script() = default;
	Script(std::string n) : name{ n } {}
	Script(rapidjson::Value const& obj) {
		name = obj["script"].GetString();
	}

	bool Serialize(rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

		sm->InsertValue(obj, "script", name);

		return true;
	}
};