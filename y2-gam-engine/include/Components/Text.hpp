/******************************************************************************/
/*!
\par        Image Engine
\file       Text.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 14, 2023

\brief      Defines the component for rendering text.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <string>
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>
#include "Math/MathUtils.h"

struct Text {
	std::string fontName;
	float scale;
	std::string text;
	Vec3 color;

	Text() = default;

	Text(std::string fn, float s, std::string tex, Vec3 col) :
		fontName{ fn }, scale{ s }, text{ tex }, color{ col } {}

	Text([[maybe_unused]] rapidjson::Value const& obj) :
		fontName{ obj["fontName"].GetString() },
		scale{ obj["scale"].GetFloat() },
		text{ obj["text"].GetString() },
		color{ obj["coloX"].GetFloat(), obj["coloY"].GetFloat(), obj["coloZ"].GetFloat() } {}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

		sm->InsertValue(obj, "fontName", fontName);
		sm->InsertValue(obj, "scale", scale);
		sm->InsertValue(obj, "text", text);
		sm->InsertValue(obj, "coloX", color.x);
		sm->InsertValue(obj, "coloY", color.y);
		sm->InsertValue(obj, "coloZ", color.z);

		return true;
	}
};