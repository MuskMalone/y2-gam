#pragma once

#include "Core/System.hpp"
#include <Components/RigidBody.hpp>
#include <Math/Vec2.hpp>
#include <Core/Physics.hpp>
#include <unordered_map>

namespace Physics {
	class PhysicsSystem : public System
	{
	public:
		ArbiterHashTable mArbiterTable;
		void Init();

		void PreCollisionUpdate(float dt);
		void PostCollisionUpdate(float dt);
		void Update(float dt);
	private:
		const size_t iterations {10}; // iterations for sequential impulse
	};
	
}
