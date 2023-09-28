#include "Math/MathUtils.h"
namespace Image {

	float Image::Degree(float rad) {
		return rad * (180.f / gPI);
	}

	float Image::Radian(float degree) {
		return degree * (gPI / 180.f);
	}

}
