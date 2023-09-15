#pragma once

#include "Math/Vec3.hpp"
#include "glad/glad.h"
#include "Graphics/Texture.hpp"

//struct Sprite
//{
//	Vec3 color;
//	GLenum drawMode{GL_FILL}; //ignore this
//};

struct Sprite {
	Vec4 color;
	std::shared_ptr<Texture> texture;
};