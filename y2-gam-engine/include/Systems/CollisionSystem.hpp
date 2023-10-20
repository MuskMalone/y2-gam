/******************************************************************************/
/*!
\par        Image Engine
\file       CollisionSystem.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		collision detection system

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

//#include <Components/RigidBody.hpp>
#include "Math/MathUtils.h"
#include "Core/System.hpp"
#include "DataMgmt/QuadTree/Quadtree.hpp"
#include "Core/Physics.hpp"
#include <Components/Collider.hpp>
#include <functional>
namespace Collision {
	using namespace Physics;
    int ClipSegmentToLine(ClipVertex vOut[2], ClipVertex vIn[2], const Vec2& normal, float offset,
        float clipEdge);
    void ComputeIncidentEdge(ClipVertex c[2], const Vec2& h, const Vec2& pos, const Mat22& rot,
        const Vec2& normal);

	//uint32_t BoxBoxCollide(Physics::Contact* contacts, RigidBody& b1, RigidBody& b2);
	//uint32_t CircleCircleCollide(Physics::Contact* contacts, RigidBody& b1, RigidBody& b2);
	//uint32_t CircleBoxCollide(Physics::Contact* contacts, RigidBody& b1, RigidBody& b2);
	//uint32_t BoxCircleCollide(Physics::Contact* contacts, RigidBody& b1, RigidBody& b2);

	uint32_t BoxBoxCollide(Physics::Contact* contacts, Collider& b1, Collider& b2);
	uint32_t CircleCircleCollide(Physics::Contact* contacts, Collider& b1, Collider& b2);
	uint32_t CircleBoxCollide(Physics::Contact* contacts, Collider& b1, Collider& b2);
	uint32_t BoxCircleCollide(Physics::Contact* contacts, Collider& b1, Collider& b2);

	class CollisionSystem : public System
	{
	public:
		void Init();

		void Update(float dt);

		void Debug();

		bool Raycast(Vec2 const& origin, Vec2 const& end, RayHit& rh);

	private:
		using ColliderLookupKey = std::pair<ColliderType, ColliderType>;

		bool RaycastBody(Vec2 const& origin, Vec2 const& end, Entity e, Vec2& cn, Vec2& cp, float& time);
		Arbiter Collide(Entity b1, Entity b2);
		DataMgmt::Quadtree<Entity> mQuadtree;
		std::map <ColliderLookupKey, std::function<uint32_t(Contact*, Collider&, Collider&)>> mLookupTable;
	};
}