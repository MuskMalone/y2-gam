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
#include "Graphics/FontRenderer.hpp"
#include "Core/Coordinator.hpp"
#include "Core/System.hpp"
#include <Core/Globals.hpp>

#include "Systems/InputSystem.hpp"
#include "Systems/TextSystem.hpp"

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {
  // Static Initialization
  std::set<Entity> NodeManager::currentlyActiveNodes;
	std::map<std::pair<Entity, Entity>, NodeManager::Cost> NodeManager::costMap;

	void NodeManager::Update() {
		::gCoordinator = Coordinator::GetInstance();

//#ifdef _DEBUG
		//DisplayDebugLines();
		for (Entity const& e : currentlyActiveNodes) {
			for (Entity const& n : gCoordinator->GetComponent<Node>(e).neighbours) {
				Vec2 firstPosition{ gCoordinator->GetComponent<Transform>(e).position.x,  gCoordinator->GetComponent<Transform>(e).position.y };
				Vec2 secondPosition{ gCoordinator->GetComponent<Transform>(n).position.x,  gCoordinator->GetComponent<Transform>(n).position.y };
				Vec2 firstScreenPosition{ TextSystem::WorldToScreenCoordinates(Vec2(firstPosition.x, firstPosition.y)) };
				Vec2 secondScreenPosition{ TextSystem::WorldToScreenCoordinates(Vec2(secondPosition.x, secondPosition.y)) };
				Image::FontRenderer::RenderText("Arial", std::to_string(costMap[std::pair(e, n)]), 
					(firstScreenPosition.x + secondScreenPosition.x) / 2, (firstScreenPosition.y + secondScreenPosition.y) / 2,
					0.05f, Vec3(0.f, 1.f, 0.f));
			}
		}
//#endif

		for (Entity const& e : currentlyActiveNodes) {
			if (::gCoordinator->GetComponent<Node>(e).selected) {
				::gCoordinator->GetComponent<Text>(e).text = "Selected";
			}
			else {
				::gCoordinator->GetComponent<Text>(e).text = "Node " + std::to_string(e);
			}
		}

		int count{};
		std::vector<Entity> currentSelection;

		for (Entity const& e : currentlyActiveNodes) {
			if (::gCoordinator->GetComponent<Node>(e).selected) {
				currentSelection.push_back(e);
				++count;
			}

			if (count == 2) {
				AddNeighbour(currentSelection.front(), currentSelection.back());
				::gCoordinator->GetComponent<Node>(currentSelection.front()).selected = false;
				::gCoordinator->GetComponent<Node>(currentSelection.back()).selected = false;
			}
		}
	}

  void NodeManager::DisplayDebugLines() {
    
    for (Entity const& e : currentlyActiveNodes) {
      for (Entity const& n : gCoordinator->GetComponent<Node>(e).neighbours) {
				Vec2 firstPosition{ gCoordinator->GetComponent<Transform>(e).position.x,  gCoordinator->GetComponent<Transform>(e).position.y };
				Vec2 secondPosition{ gCoordinator->GetComponent<Transform>(n).position.x,  gCoordinator->GetComponent<Transform>(n).position.y };
				
        Renderer::DrawLine(glm::vec3(firstPosition.x, firstPosition.y, 0), 
					glm::vec3(secondPosition.x, secondPosition.y, 1), { 0,1,0,1 });

				/*
				std::cout << "First Position: " << gCoordinator->GetComponent<Transform>(e).position.x << " ," << gCoordinator->GetComponent<Transform>(e).position.y << "\n";
				std::cout << "Second Position: " << gCoordinator->GetComponent<Transform>(n).position.x << " ," << gCoordinator->GetComponent<Transform>(n).position.y << "\n";
				*/
      }
    }
  }

  void NodeManager::AddNode() {
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		Entity node = ::gCoordinator->CreateEntity();
		//::gCoordinator->AddComponent<Script>(node, { "ObjectNode" });
		::gCoordinator->AddComponent<Node>(node, Node());
		Vec3 position = Vec3(inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second, 1.f);

		float scale{ 10.f };
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

		// Temporarily added to work with raycast
		::gCoordinator->AddComponent(
			node,
			Collider{
			});
    currentlyActiveNodes.insert(node);
  }

	void NodeManager::AddNeighbour(Entity lhs, Entity rhs) {
		::gCoordinator->GetComponent<Node>(lhs).neighbours.insert(rhs);
		::gCoordinator->GetComponent<Node>(rhs).neighbours.insert(lhs);
		FillCostMap();
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
		FillCostMap();
	}

	void NodeManager::ClearAllNodes() {
		for (Entity const& e : currentlyActiveNodes) {
			gCoordinator->DestroyEntity(e);
		}
		currentlyActiveNodes.clear();
		FillCostMap();
	}

	int NodeManager::CalculateCost(Entity lhs, Entity rhs) {
		Vec2 posLHS{ gCoordinator->GetComponent<Transform>(lhs).position.x, gCoordinator->GetComponent<Transform>(lhs).position.y };
		Vec2 posRHS{ gCoordinator->GetComponent<Transform>(rhs).position.x, gCoordinator->GetComponent<Transform>(rhs).position.y };

		return static_cast<int>((posRHS.x - posLHS.x) * (posRHS.x - posLHS.x) + (posRHS.y - posLHS.y) * (posRHS.y - posLHS.y));
	}

	void NodeManager::FillCostMap() {
		for (Entity const& e : currentlyActiveNodes) {
			for (Entity const& n : gCoordinator->GetComponent<Node>(e).neighbours) {
				costMap[std::pair(e, n)] = CalculateCost(e, n);
			}
		}
	}

	void NodeManager::PrintCostMap() {
		std::cout << "*****COSTMAPSTART*****\n";
		for (auto const& entry : costMap) {
			std::pair<Entity, Entity> const& key = entry.first;
			NodeManager::Cost const& value = entry.second;

			std::cout << "Key: (" << key.first << ", " << key.second << "), Value: " << value << "\n";
		}
		std::cout << "*****COSTMAPEND*****\n";
	}
}