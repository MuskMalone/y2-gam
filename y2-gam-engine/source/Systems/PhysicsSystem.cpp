#include "Systems/PhysicsSystem.hpp"

#include "Components/Gravity.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Thrust.hpp"
#include "Components/Transform.hpp"
#include "Core/Coordinator.hpp"
#include <Components/AABBCollider.hpp>
#include <memory>
#include <glm/matrix.hpp>

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void PhysicsSystem::Init()
{
	gCoordinator = Coordinator::GetCoordinator();
}

void PhysicsSystem::PreCollisionUpdate(float dt)
{
	for (auto const& entity : mEntities)
	{
		auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);
		auto& transform = gCoordinator->GetComponent<Transform>(entity);
		auto& collider = gCoordinator->GetComponent<AABBCollider>(entity);
		auto const& gravity = gCoordinator->GetComponent<Gravity>(entity);

		//rigidBody.velocity = Vec3(0, 0, 0) - transform.position;
		transform.position += rigidBody.velocity * dt;
		collider.position = transform.position;

		rigidBody.velocity += gravity.force * dt;

	}
}

void PhysicsSystem::PostCollisionUpdate(float dt) {
	for (auto const& entity : mEntities) {
		auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);

	}
}
