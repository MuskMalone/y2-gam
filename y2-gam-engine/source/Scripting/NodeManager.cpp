#include "../include/pch.hpp"

#include "Graphics/Renderer.hpp"
#include "Scripting/NodeManager.hpp"
#include "Components/Node.hpp"
#include "Components/Transform.hpp"
#include "Core/Coordinator.hpp"
#include <Systems/InputSystem.hpp>
#include "Math/MathUtils.h"

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {
  // Static Initialization
  std::vector<Entity> NodeManager::currentlyActiveNodes;
	std::map<std::pair<Entity, Entity>, NodeManager::Cost> NodeManager::costMap;

  void NodeManager::DisplayDebugLines() {
    ::gCoordinator = Coordinator::GetInstance();
    for (Entity const& e : currentlyActiveNodes) {
      for (Entity const& n : gCoordinator->GetComponent<Node>(e).neighbours) {
        Renderer::DrawLine(gCoordinator->GetComponent<Transform>(e).position, gCoordinator->GetComponent<Transform>(n).position, { 0,1,0,1 });
      }
    }
  }
  void NodeManager::AddNode() {
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		Entity node = ::gCoordinator->CreateEntity();
		//::gCoordinator->AddComponent<Script>(node, { "ObjectNode" });
		::gCoordinator->AddComponent<Node>(node, Node());
		//Vec3 position = Vec3(inputSystem->GetMousePos().first, inputSystem->GetMousePos().second, -150.f);
		Vec3 position = Vec3(0, 0, -150.f);

		float scale{ 5.f };
		::gCoordinator->AddComponent(
			node,
			RigidBody{
				Vec2(position), 0.f, 10.f, Vec2(scale, scale), false
			});
		::gCoordinator->AddComponent(
			node,
			Transform{
				{position.x,position.y,position.z},
				{0.f,0.f,0.f},
				{scale, scale, scale}
			});
		::gCoordinator->AddComponent(
			node,
			Sprite{
				{0, 0, 0, 1},
				nullptr
			});
		::gCoordinator->AddComponent(
			node,
			Text{
				"Lato",
				0.05f,
				"Node " + std::to_string(node),
				{1, 1, 0}
			});

    currentlyActiveNodes.push_back(node);
  }

	void NodeManager::RemoveNode(Entity node) {

	}

	void NodeManager::ClearAllNodes() {

	}

	void NodeManager::CalculateCost(Entity lhs, Entity rhs) {

	}
	void NodeManager::FillCostMap() {

	}
}