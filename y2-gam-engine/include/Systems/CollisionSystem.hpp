#pragma once
#include <Components/RigidBody.hpp>
#include <Math/Mat44.hpp>
#include "Core/System.hpp"
#include "DataMgmt/QuadTree/Quadtree.hpp"
#include "Core/Physics.hpp"
#include <Components/BoxCollider.hpp>

namespace Collision {
	using namespace Physics;
    int ClipSegmentToLine(ClipVertex v_out[2], ClipVertex v_in[2], const Vec2& normal, float offset,
        float clip_edge);
    void ComputeIncidentEdge(ClipVertex c[2], const Vec2& h, const Vec2& pos, const Mat22& rot,
        const Vec2& normal);
	uint32_t Collide(Physics::Contact* contacts, RigidBody& b1, RigidBody& b2);

	class CollisionSystem : public System
	{
	public:
		void Init();

		void Update(float dt);
		DataMgmt::Quadtree<Entity>* GetQuadtree() { return &mQuadtree; }
	private:

		DataMgmt::Quadtree<Entity> mQuadtree;
	};
}