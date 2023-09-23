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
};