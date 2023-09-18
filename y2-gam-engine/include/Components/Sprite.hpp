#pragma once

#include "Math/Vec4.hpp"
#include "glad/glad.h"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"

struct Sprite {
	Vec4 color;
	std::shared_ptr<SubTexture> texture;
	unsigned int spriteIdx;

	Sprite() = default;
	Sprite(Vec4 color, std::shared_ptr<SubTexture> tex) : color{ color }, texture{ tex } {}
	Sprite(rapidjson::Value const& obj) {}
	void Serialize(rapidjson::Value& obj) {

	}
};