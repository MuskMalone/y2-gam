/******************************************************************************/
/*!
\par        Image Engine
\file       Circle.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Oct 13, 2023

\brief      Cirle component for rendering.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include <rapidjson/document.h>

struct Circle {
	glm::vec4 color{ 1.f, 1.f, 1.f, 1.f};
	float radius{ 0.5f };
	float thickness{ 1.f };
	float fade{ 0.005f };

	//Layer layer{};

	Circle() = default;
	Circle(glm::vec4 color) : color{ color } {}
	Circle([[maybe_unused]] rapidjson::Value const& obj) {
		color = { 1,1,1,1 };
		//layer = Layer::FOREGROUND;
	}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return true;
	}
};