/******************************************************************************/
/*!
\par        Image Engine
\file       Collision.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		collision stuff

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Math/MathUtils.h"

namespace Physics {
	//first: min point, second: size
	using CollisionRect = std::pair<Vec2, Vec2>;
	/**
 * @brief Checks if two axis-aligned bounding boxes (AABBs) are overlapping.
 *
 * @param cmin The minimum coordinates of the first AABB.
 * @param cmax The maximum coordinates of the first AABB.
 * @param rmin The minimum coordinates of the second AABB.
 * @param rmax The maximum coordinates of the second AABB.
 *
 * @return true if the AABBs are overlapping, false otherwise.
 */
	bool CheckAABBDiscrete(Vec2 const& cmin, Vec2 const& cmax, Vec2 const& rmin, Vec2 const& rmax) {
		return (cmin.x < rmax.x && cmax.x > rmin.x && cmin.y < rmax.y && cmax.y > rmin.y);
	}
	/**
 * @brief Performs ray-rectangle intersection testing.
 *
 * @param rayOrigin The origin of the ray.
 * @param rayDir The direction of the ray.
 * @param target The target rectangle to test for intersection.
 * @param contactPoint Output parameter to store the contact point of the intersection.
 * @param contactNormal Output parameter to store the contact normal of the intersection.
 * @param tHitNear Output parameter to store the distance along the ray to the nearest intersection point.
 *
 * @return true if the ray intersects the rectangle, false otherwise.
 */
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
	
}

