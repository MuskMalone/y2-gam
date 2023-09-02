#pragma once
#include <Math/Vec2.hpp>
bool CheckAABBDiscrete(Vec2 const& cmin, Vec2 const& cmax, Vec2 const& rmin, Vec2 const& rmax) {
	return (cmin.x < rmax.x && cmax.x > rmin.x && cmin.y < rmax.y && cmax.y > rmin.y);
}