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
		color = { 1,1,1,1 };
		texture = nullptr;
		layer = Layer::FOREGROUND;
	}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return true;
	}
};