#pragma once

#include "Core/System.hpp"
#include <Components/RigidBody.hpp>
#include "Math/MathUtils.h"
#include <Core/Physics.hpp>
#include <unordered_map>
#include <Core/Types.hpp>
#include <Core/Event.hpp>
namespace Physics {
	class PhysicsSystem : public System
	{
	public:

		void Init();

		void PreCollisionUpdate(float dt);
		void PostCollisionUpdate(float dt);

	private:
		const size_t iterations {10}; // iterations for sequential impulse
		ArbiterHashTable mArbiterTable;
		void CollisionListener(Event& event);
	};
	
}
