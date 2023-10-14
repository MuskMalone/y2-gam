/******************************************************************************/
/*!
\par        Image Engine
\file       NodeManager.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 14, 2023

\brief      The source file for creating and managing nodes for the purposes
						of AI path graphing.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include "Scripting/NodeManager.hpp"
#include "Graphics/Renderer.hpp"
#include "Core/Coordinator.hpp"
#include "Core/System.hpp"
#include <Core/Globals.hpp>
#include "Systems/InputSystem.hpp"

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {
  // Static Initialization
  std::set<Entity> NodeManager::currentlyActiveNodes;
	std::map<std::pair<Entity, Entity>, NodeManager::Cost> NodeManager::costMap;

  void NodeManager::DisplayDebugLines() {
    ::gCoordinator = Coordinator::GetInstance();
    for (Entity const& e : currentlyActiveNodes) {
      for (Entity const& n : gCoordinator->GetComponent<Node>(e).neighbours) {
        Renderer::DrawLine(gCoordinator->GetComponent<Transform>(e).position, gCoordinator->GetComponent<Transform>(n).position, { 0.f,1.f,0.f,1.f });
      }
    }
  }

  void NodeManager::AddNode() {
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		Entity node = ::gCoordinator->CreateEntity();
		//::gCoordinator->AddComponent<Script>(node, { "ObjectNode" });
		::gCoordinator->AddComponent<Node>(node, Node());
		Vec3 position = Vec3(inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second, -150.f);

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

    currentlyActiveNodes.insert(node);
  }

	void NodeManager::AddNeighbour(Entity lhs, Entity rhs) {

	}

	void NodeManager::RemoveNode(Entity node) {
		for (Entity const& e : currentlyActiveNodes) {
			auto search = gCoordinator->GetComponent<Node>(e).neighbours.find(node);
			if (search != gCoordinator->GetComponent<Node>(e).neighbours.end()) {
				gCoordinator->GetComponent<Node>(e).neighbours.erase(node);
			}
		}
		gCoordinator->DestroyEntity(node);
		currentlyActiveNodes.erase(node);
	}

	void NodeManager::ClearAllNodes() {
		for (Entity const& e : currentlyActiveNodes) {
			gCoordinator->DestroyEntity(e);
		}
		currentlyActiveNodes.clear();
	}

	int NodeManager::CalculateCost(Entity lhs, Entity rhs) {
		Vec2 posLHS{ gCoordinator->GetComponent<Transform>(lhs).position.x, gCoordinator->GetComponent<Transform>(lhs).position.y };
		Vec2 posRHS{ gCoordinator->GetComponent<Transform>(rhs).position.x, gCoordinator->GetComponent<Transform>(rhs).position.y };

		return (posRHS.x - posLHS.x) * (posRHS.x - posLHS.x) + (posRHS.y - posLHS.y) * (posRHS.y - posLHS.y);
	}

	void NodeManager::FillCostMap() {

	}
}