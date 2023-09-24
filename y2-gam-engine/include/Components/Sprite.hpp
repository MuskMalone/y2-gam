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
	Vec4 color;
	std::shared_ptr<SubTexture> texture;
	Layer layer;

	Sprite() = default;
	Sprite(Vec4 color, std::shared_ptr<SubTexture> tex) : color{ color }, texture{ tex } {}
	Sprite(rapidjson::Value const& obj) {
		color = { 1,1,1,1 };
		texture = nullptr;
	}
	bool Serialize(rapidjson::Value& obj) {
		return false;
	}
};