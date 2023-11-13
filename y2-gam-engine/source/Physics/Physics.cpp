#include "../include/pch.hpp"
#include <Physics/Physics.hpp>
#include <Core/Coordinator.hpp>
#include <Systems/CollisionSystem.hpp>
#include <Systems/PhysicsSystem.hpp>
namespace Physics {
	
	//for phys system. returns entities that are collided and resolved by physics
	bool IsCollided(Entity const& e1, Entity const& e2) {
		return Coordinator::GetInstance()->GetSystem<PhysicsSystem>()->IsCollided(e1, e2);
	}
	bool IsCollided(Entity const& e1, Entity const& e2, Arbiter& a) {
		return Coordinator::GetInstance()->GetSystem<PhysicsSystem>()->IsCollided(e1, e2, a);
	}
	ArbiterVec IsCollided(Entity const& e1) {
		return Coordinator::GetInstance()->GetSystem<PhysicsSystem>()->IsCollided(e1);
	}

	//for collisionsystem. returns entities whose colliders intersect.
	// physics resolution not guaranteed for this method
	bool IsIntersected(Entity const& e1, Entity const& e2) {
		return Coordinator::GetInstance()->GetSystem<Collision::CollisionSystem>()->IsIntersected(e1, e2);
	}
	bool IsIntersected(Entity const& e1, Entity const& e2, Arbiter& a) {
		return Coordinator::GetInstance()->GetSystem<Collision::CollisionSystem>()->IsIntersected(e1, e2, a);
	}
	ArbiterVec IsIntersected(Entity const& e1) {
		return Coordinator::GetInstance()->GetSystem<Collision::CollisionSystem>()->IsIntersected(e1);
	}
}