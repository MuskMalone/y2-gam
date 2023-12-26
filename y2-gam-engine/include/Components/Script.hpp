/******************************************************************************/
/*!
\par        Image Engine
\file       Script.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 26, 2023

\brief      The main script component. Also handles the serialization of
						public script variables from the respective scripts.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <Core/Serialization/SerializationManager.hpp>
#include "Scripting/ScriptManager.hpp"

struct Script {
  std::string name;
	std::string scriptTagged;

	Script() = default;

	Script(std::string scriptName, std::string tag) : name{ scriptName }, scriptTagged{ tag } {}

	Script(rapidjson::Value const& obj) {
		name = obj["scriptName"].GetString();
		scriptTagged = obj["scriptTag"].GetString();
	}

	bool Serialize(rapidjson::Value& obj) {
		std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

		sm->InsertValue(obj, "scriptName", name);
		sm->InsertValue(obj, "scriptTag", scriptTagged);


		return true;
	}
};