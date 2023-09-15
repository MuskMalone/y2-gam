#pragma once

#include "Math/Vec3.hpp"
#include "glad/glad.h"

struct Renderable
{
	Vec3 color;
	GLenum drawMode{GL_FILL}; //ignore this
};

struct Sprite {
	Vec4 color;

};