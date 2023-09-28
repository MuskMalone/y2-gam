#pragma once

#include "Math/Vec4.hpp"
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
	glm::vec4 color;
	std::shared_ptr<SubTexture> texture;
	Layer layer;

	Sprite() = default;
	Sprite(glm::vec4 color, std::shared_ptr<SubTexture> tex, Layer lyr = Layer::FOREGROUND) : color{ color }, texture{ tex }, layer{lyr} {}
	Sprite(rapidjson::Value const& obj) {
		color = { 1,1,1,1 };
		texture = nullptr;
		layer = Layer::FOREGROUND;
	}
	bool Serialize(rapidjson::Value& obj) {
		return true;
	}
};