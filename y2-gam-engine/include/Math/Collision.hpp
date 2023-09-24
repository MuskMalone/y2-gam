#pragma once
#include <Math/Vec2.hpp>

//first: min point, second: size
using CollisionRect = std::pair<Vec2, Vec2>;

bool CheckAABBDiscrete(Vec2 const& cmin, Vec2 const& cmax, Vec2 const& rmin, Vec2 const& rmax) {
	return (cmin.x < rmax.x && cmax.x > rmin.x && cmin.y < rmax.y && cmax.y > rmin.y);
}
bool CheckRayRect(const Vec2& rayOrigin, const Vec2& rayDir, const CollisionRect& target, Vec2& contactPoint, Vec2& contactNormal, float& tHitNear)
{
	contactNormal = { 0,0 };
	contactPoint = { 0,0 };

	// Cache division
	Vec2 invdir{ 1.0f / rayDir.x, 1.0f / rayDir.y };

	// Calculate intersections with rectangle bounding axes
	Vec2 tNear = (target.first - rayOrigin) * invdir;
	Vec2 tFar = (target.first + target.second - rayOrigin) * invdir;

	if (std::isnan(tFar.y) || std::isnan(tFar.x)) return false;
	if (std::isnan(tNear.y) || std::isnan(tNear.x)) return false;

	// sort distances
	if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
	if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

	// early rejection		
	if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

	// closest 'time' will be the first contact
	tHitNear = std::max<float>(tNear.x, tNear.y);

	// furthest 'time' is contact on opposite side of target
	float tHitFar = std::min<float>(tFar.x, tFar.y);

	// Reject if ray direction is pointing away from object
	if (tHitFar < 0)
		return false;

	// contact point of collision from parametric line equation
	contactPoint = rayOrigin + (rayDir * tHitNear);

	if (tNear.x > tNear.y)
		if (invdir.x < 0)
			contactNormal = { 1, 0 };
		else
			contactNormal = { -1, 0 };
	else if (tNear.x < tNear.y)
		if (invdir.y < 0)
			contactNormal = { 0, 1 };
		else
			contactNormal = { 0, -1 };

	return true;
}

bool CheckSweptAABB(const CollisionRect& rDynamic, Vec2 const& dynamicVelocity,  const float dt, const CollisionRect& rStatic,
	Vec2& contactPoint, Vec2& contactNormal, float& contactTime)
{
	// Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
	if (dynamicVelocity.x == 0 && dynamicVelocity.y == 0)
		return false;

	// Expand target rectangle by source dimensions
	CollisionRect expandedTarget;
	expandedTarget.first = rStatic.first - rDynamic.second / 2.f;
	expandedTarget.second = rStatic.second + rDynamic.second;

	if (CheckRayRect((rDynamic.first + rDynamic.second / 2.f), (dynamicVelocity * dt), expandedTarget, contactPoint, contactNormal, contactTime))
		return (contactTime >= 0.0f && contactTime < 1.0f);
	else
		return false;
}
bool ResolveSweptAABB(CollisionRect const& rDynamic, Vec2& dynamicVelocity, const float dt, CollisionRect const& rStatic)
{
	Vec2 contactPoint, contactNormal;
	float contactTime = 0.0f;
	if (CheckSweptAABB(rDynamic, dynamicVelocity, dt, rStatic, contactPoint, contactNormal, contactTime))
	{
		dynamicVelocity += ((contactNormal * Vec2(std::fabs(dynamicVelocity.x), std::fabs(dynamicVelocity.y)) * (1.f - contactTime)));

		return true;
	}
	return false;
}