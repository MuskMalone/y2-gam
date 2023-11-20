/******************************************************************************/
/*!
\par        Image Engine
\file       UIImage.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Nov 20, 2023

\brief      Header file for the UIImage Component.

\copyright  Copyright (C) 2023 [Your Company or Institution]. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of [Your Company or Institution] is prohibited.
*/
/******************************************************************************/

#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <Core/Globals.hpp>
#include <rapidjson/document.h>
#include "Core/Types.hpp"
#include <Components/Sprite.hpp>

struct UIImage
{
	bool enabled{true};

	UIImage() = default;
	UIImage(bool en) : enabled{en} {}
	UIImage([[maybe_unused]] rapidjson::Value const& obj) {}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};
