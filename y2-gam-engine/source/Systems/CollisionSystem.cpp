#include "Systems/CollisionSystem.hpp"
#include <memory>
#include "Core/Coordinator.hpp"
#include "Components/AABBCollider.hpp"
#include "Components/RigidBody.hpp"
#include <Core/Globals.hpp>
#include <Math/Collision.hpp>

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void CollisionSystem::Init() {
	gCoordinator = Coordinator::GetCoordinator();
	using namespace DataMgmt;
	mQuadtree = DataMgmt::Quadtree<Entity>{ 0, Rect(Vec2(-WORLD_LIMIT_X, -WORLD_LIMIT_Y), Vec2(WORLD_LIMIT_X, WORLD_LIMIT_Y)) };
}

void CollisionSystem::Update(float dt) {
	//for (auto const& entity : mEntities) {
	//	auto& collider = gCoordinator->GetComponent<AABBCollider>(entity);
	//	auto& rigidBody = gCoordinator->GetComponent<RigidBody>(entity);


	//}
	
	mQuadtree.Update(mEntities, [](Entity const& e, DataMgmt::Rect const& r) {
		auto const& collider{ Coordinator::GetCoordinator()->GetComponent<AABBCollider>(e) };
		Vec2 cmin = collider.position - collider.dimension*.5f;
		Vec2 cmax = collider.position + collider.dimension * .5f;
		Vec2 rmin = r.get_min();
		Vec2 rmax = r.get_max();
		//std::cout << "================\n";
		//std::cout << cmin.x << " " << cmin.y << std::endl;
		//std::cout << cmax.x << " " << cmax.y << std::endl;
		//std::cout << rmin.x << " " << rmin.y << std::endl;
		//std::cout << rmax.x << " " << rmax.y << std::endl;
		//std::cout << CheckAABBDiscrete(cmin, cmax, rmin, rmax) << std::endl;
		//basic aabb check
		return CheckAABBDiscrete(cmin, cmax, rmin, rmax);
	});
	std::vector<std::vector<Entity>> entity2dVec;
	mQuadtree.Get(entity2dVec);
	//std::cout << entity2dVec.size() << std::endl;
	//calculate each list of entities in each quad
	for (auto const& entityVec : entity2dVec) {
		for (int i{}; i < entityVec.size(); ++i) {
			for (int j{ i + 1 }; j < entityVec.size(); ++j) {
				// COLLISION DETECTION AND RESOLUTION IS SUPPOSED TO BE PLACEHOLDERS 
				// WILL REMOVE LATER
				//Vec2 cp, cn;
				//float t;
				auto& rigidbody1{ gCoordinator->GetComponent<RigidBody>(entityVec[i]) };
				auto& rigidbody2{ gCoordinator->GetComponent<RigidBody>(entityVec[j]) };
				auto const& collider1{ gCoordinator->GetComponent<AABBCollider>(entityVec[i]) };
				auto const& collider2{ gCoordinator->GetComponent<AABBCollider>(entityVec[j]) };
				Vec2 cmin = collider1.position - collider1.dimension * .5f;
				Vec2 rmin = collider2.position - collider2.dimension * .5f;
				Vec2 cmax = collider1.position + collider1.dimension * .5f;
				Vec2 rmax = collider2.position + collider2.dimension * .5f;
				if (CheckAABBDiscrete(cmin, cmax, rmin, rmax)) {
					rigidbody1.velocity = -rigidbody1.velocity;
					rigidbody2.velocity = -rigidbody2.velocity;

				}
				//if (CheckSweptAABB({cmin, collider1.dimension}, rigidbody1.velocity, dt, { rmin, collider2.dimension }, cp, cn, t)) {
				//	//std::cout << "collided\n";
				//	ResolveSweptAABB({ cmin, collider1.dimension }, rigidbody1.velocity, dt, { rmin, collider2.dimension });
				//}
			}
		}
	}
}