#pragma once
#include <math.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat2.h"
#include "Mat3.h"
#include "Mat4.h"

using namespace Image;
namespace Image {
	constexpr float gPI{ 3.141592653589793238462643383279502884197169399375105f };

	float Degree(float rad);

	float Radian(float degree);
}