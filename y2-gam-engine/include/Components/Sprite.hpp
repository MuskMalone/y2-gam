/******************************************************************************/
/*!
\par        Image Engine
\file       Sprite.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 10, 2023

\brief      Header file for the Sprite Component.

			Defines the Sprite component, which represents a graphical element
			in the game engine. The Sprite component contains properties such
			as color, texture, and layer. It also provides functionalities for
			serialization.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once


#include "glad/glad.h"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"
#include <Core/Serialization/SerializationManager.hpp>

enum class Layer {
	BACKGROUND,
	MIDGROUND,
	FOREGROUND,
	UI,
};

struct Sprite {
	glm::vec4 color{};
	std::shared_ptr<SubTexture> texture{};
	Layer layer{};

	Sprite() = default;
	Sprite(glm::vec4 color, std::shared_ptr<SubTexture> tex, Layer lyr = Layer::FOREGROUND) : color{ color }, texture{ tex }, layer{lyr} {}
	Sprite([[maybe_unused]] rapidjson::Value const& obj) {
		color = { obj["r"].GetFloat(),obj["g"].GetFloat(),obj["b"].GetFloat(),obj["a"].GetFloat() };
		texture = nullptr;
		layer = static_cast<Layer>(obj["layer"].GetInt());
	}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

		sm->InsertValue(obj, "r", color.x);
		sm->InsertValue(obj, "g", color.y);
		sm->InsertValue(obj, "b", color.z);
		sm->InsertValue(obj, "a", color.z);
		sm->InsertValue(obj, "layer", static_cast<int>(layer));
		return true;
	}
};