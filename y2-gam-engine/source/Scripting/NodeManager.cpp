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
#include "Systems/RenderSystem.hpp"

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {
  // Static Initialization
  std::set<Entity> NodeManager::currentlyActiveNodes;
	std::map<std::pair<Entity, Entity>, NodeManager::Cost> NodeManager::costMap;

	/*  _________________________________________________________________________ */
	/*! Update

	@return none.

	Facilitates the creation of nodes and the addition of neighbours to nodes.
	*/
	void NodeManager::Update() {
		::gCoordinator = Coordinator::GetInstance();

//#ifdef _DEBUG
		//DisplayDebugLines();
		if (::gCoordinator->GetSystem<RenderSystem>()->GetDebugMode()) {
			for (Entity const& ent : currentlyActiveNodes) {
				for (Entity const& neighbour : gCoordinator->GetComponent<Node>(ent).neighbours) {

					Vec2 firstPosition{ gCoordinator->GetComponent<Transform>(ent).position.x,
						gCoordinator->GetComponent<Transform>(ent).position.y };
					Vec2 secondPosition{ gCoordinator->GetComponent<Transform>(neighbour).position.x,
						gCoordinator->GetComponent<Transform>(neighbour).position.y };

					Vec2 firstScreenPosition { 
						TextSystem::WorldToScreenCoordinates(Vec2(firstPosition.x, firstPosition.y)) 
					};

					Vec2 secondScreenPosition{ 
						TextSystem::WorldToScreenCoordinates(Vec2(secondPosition.x, secondPosition.y)) 
					};

					Vec2 centerPosition{ (firstScreenPosition.x + secondScreenPosition.x) / 2, 
						(firstScreenPosition.y + secondScreenPosition.y) / 2 };

					Image::FontRenderer::RenderText("Arial", std::to_string(costMap[std::pair(ent, neighbour)]),
						centerPosition.x, centerPosition.y,
						0.07f, Vec3(0.f, 1.f, 0.f));
				}
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

	/*  _________________________________________________________________________ */
	/*! DisplayDebugLines

	@return none.

	Draws debug lines between the connected nodes. Seen in debug mode.
	*/
  void NodeManager::DisplayDebugLines() {
    
    for (Entity const& e : currentlyActiveNodes) {
      for (Entity const& n : gCoordinator->GetComponent<Node>(e).neighbours) {
				Vec2 firstPosition{ gCoordinator->GetComponent<Transform>(e).position.x,  
					gCoordinator->GetComponent<Transform>(e).position.y };
				Vec2 secondPosition{ gCoordinator->GetComponent<Transform>(n).position.x,  
					gCoordinator->GetComponent<Transform>(n).position.y };
				
        Renderer::DrawLine(glm::vec3(firstPosition.x, firstPosition.y, 0), 
					glm::vec3(secondPosition.x, secondPosition.y, 1), { 0,1,0,1 });
      }
    }
  }

	/*  _________________________________________________________________________ */
	/*! AddNode

	@return none.

	Adds entities with the 'node' component to the scene.
	*/
  void NodeManager::AddNode() {
		auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
		Entity node = ::gCoordinator->CreateEntity();
		::gCoordinator->AddComponent<Node>(node, Node());
		Vec3 position = Vec3(inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second, 1.f);

		float scale{ 10.f };

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

		::gCoordinator->AddComponent(
			node,
			Collider{
				Vec2(position), 0.f, Vec2(scale, scale)
			});

		::gCoordinator->AddComponent(
			node,
			Tag{
				"Node" + std::to_string(node)
			});
    currentlyActiveNodes.insert(node);
  }

	/*  _________________________________________________________________________ */
	/*! AddNeighbour
	
	@param lhs
	The first node to connect.

	@param rhs
	The second node to connect.

	@return none.

	Adds a connection between two nodes. After that, the cost map is updated.
	*/
	void NodeManager::AddNeighbour(Entity lhs, Entity rhs) {
		::gCoordinator->GetComponent<Node>(lhs).neighbours.insert(rhs);
		::gCoordinator->GetComponent<Node>(rhs).neighbours.insert(lhs);
		FillCostMap();
	}

	/*  _________________________________________________________________________ */
	/*! RemoveNode

	@param node
	The node to remove.

	@return none.

	Retrieves the neighbours of the node to remove and removes the node from their
	neighbour list. After that, the node is removed from the scene and the cost map
	is updated.
	*/
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

	/*  _________________________________________________________________________ */
	/*! ClearAllNodes

	@return none.

	Clears all nodes from the scene and updates the cost map.
	*/
	void NodeManager::ClearAllNodes() {
		for (Entity const& e : currentlyActiveNodes) {
			gCoordinator->DestroyEntity(e);
		}
		currentlyActiveNodes.clear();
		FillCostMap();
	}

	/*  _________________________________________________________________________ */
	/*! CalculateCost

	@param lhs
	The first node to calculate cost for.

	@param rhs
	The second node to calculate cost for.

	@return int
	The cost between the two nodes.
	
	Calculates the cost between two nodes, based on the distance between them.
	*/
	int NodeManager::CalculateCost(Entity lhs, Entity rhs) {
		Vec2 posLHS{ gCoordinator->GetComponent<Transform>(lhs).position.x, 
			gCoordinator->GetComponent<Transform>(lhs).position.y };

		Vec2 posRHS{ gCoordinator->GetComponent<Transform>(rhs).position.x, 
			gCoordinator->GetComponent<Transform>(rhs).position.y };

		return static_cast<int>((posRHS.x - posLHS.x) * (posRHS.x - posLHS.x) + 
			(posRHS.y - posLHS.y) * (posRHS.y - posLHS.y));
	}

	/*  _________________________________________________________________________ */
	/*! FillCostMap

	@return none.

	Clears all nodes from the scene and updates the cost map.
	*/
	void NodeManager::FillCostMap() {
		for (Entity const& e : currentlyActiveNodes) {
			for (Entity const& n : gCoordinator->GetComponent<Node>(e).neighbours) {
				costMap[std::pair(e, n)] = CalculateCost(e, n);
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! PrintCostMap

	@return none.

	Prints the cost map to the console.
	*/
	void NodeManager::PrintCostMap() {
		std::cout << "*****COSTMAPSTART*****\n";
		for (auto const& entry : costMap) {
			std::pair<Entity, Entity> const& key = entry.first;
			NodeManager::Cost const& value = entry.second;

			std::cout << "Key: (" << key.first << ", " << key.second << "), Value: " << value << "\n";
		}
		std::cout << "*****COSTMAPEND*****\n";
	}

	/*  _________________________________________________________________________ */
	/*! FindClosestNodeToPosition
	
	@param position
	The position to find the closest node to.

	@return Entity
	The closest node to the position.

	Finds the closest node to a position. Used in scenarios like finding the closest
	node to the player, for future pathfinding.
	*/
	Entity NodeManager::FindClosestNodeToPosition(Vec2 position) {
		Entity closestNode{};
		float closestDistance{ FLT_MAX };

		for (Entity const& ent : currentlyActiveNodes) {
      Vec2 nodePosition{ gCoordinator->GetComponent<Transform>(ent).position.x, 
				gCoordinator->GetComponent<Transform>(ent).position.y };

      float distance{ CalculateDistanceSquared(position, nodePosition) };
      if (distance < closestDistance) {
        closestDistance = distance;
        closestNode = ent;
      }
    }

		return closestNode;
	}

	/*  _________________________________________________________________________ */
	/*! FindClosestNodeToEntity

	@param Entity
	The entity to find the closest node to.

	@return Entity
	The closest node to the entity.

	Wrapper function that finds the closest node to an entity.
	*/
	Entity NodeManager::FindClosestNodeToEntity(Entity entity) {
		Vec2 entityPosition{ 
			Vec2(gCoordinator->GetComponent<Transform>(entity).position.x, 
        gCoordinator->GetComponent<Transform>(entity).position.y)
		};

		return FindClosestNodeToPosition(entityPosition);
	}

	/*  _________________________________________________________________________ */
	/*! GetLowestScoreNode

	@param dg
	The graph to find the lowest score node from.

	@return DijsktraNode
	The lowest score node.

	Find the lowest score node from a graph.
	*/
	DijkstraNode& NodeManager::GetLowestScoreNode(DijkstraGraph& dg) {
		// Find the lowest score node and return a reference of it
		DijkstraNode* lowestScoreNode{ nullptr };
		for (DijkstraNode& node : dg) {
      if (!node.visited) {
        if (lowestScoreNode == nullptr) {
          lowestScoreNode = &node;
        }
        else {
          if (node.score < lowestScoreNode->score) {
            lowestScoreNode = &node;
          }
        }
      }
    }

		return *lowestScoreNode;
	}

	/*  _________________________________________________________________________ */
	/*! CalculateScore

	@param currentNode
	The current node to calculate score for.

	@param nextNode
	The next node to calculate score for.

	@return int
	The score between the two nodes.

	Finds the score between two nodes.
	*/
	int NodeManager::CalculateScore(DijkstraNode currentNode, DijkstraNode nextNode) {
		return currentNode.score + static_cast<int>(costMap[std::pair(currentNode.parentNode, nextNode.parentNode)]);
	}

	/*  _________________________________________________________________________ */
	/*! BuildPath

	@param targetNode
	The target node to build path for.

	@param dg
	The graph to build path from.

	@return Path
	The path between the two nodes.

	Builds a path between two nodes, based on the previous calculations.
	*/
	NodeManager::Path NodeManager::BuildPath(DijkstraNode targetNode, DijkstraGraph dg) {
		Path result;
		DijkstraNode currentNode{ targetNode };

		while (currentNode.previousNodeInPath != NULL) {
      result.push_back(currentNode.parentNode);

			// Find the previous node in the path
			for (DijkstraNode& node : dg) {
        if (node.parentNode == currentNode.previousNodeInPath) {
          currentNode = node;
          break;
        }
      }
    }

		result.push_back(currentNode.parentNode);

		std::reverse(result.begin(), result.end());

		return result;
	}

	/*  _________________________________________________________________________ */
	/*! CalculateDistanceSquared

	@param lhs
	The first node to calculate distance for.

	@param rhs
	The second node to calculate distance for.

	@return float
	The distance between the two nodes.

	Calculates the distance between two nodes.
	*/
	float NodeManager::CalculateDistanceSquared(Vec2& lhs, Vec2& rhs) {
		float x{ lhs.x - rhs.x };
    float y{ lhs.y - rhs.y };

		return x * x + y * y;
	}

	/*  _________________________________________________________________________ */
	/*! DjkstraAlgorithm

	@param start
	The first node to calculate distance for.

	@param end
	The second node to calculate distance for.

	@return Path
	The path between the two nodes.

	Pathfinding algorithm that uses Dijkstra's algorithm to find the shortest path
	between two nodes.
	*/
	NodeManager::Path NodeManager::DjkstraAlgorithm(Entity start, Entity end) {
		if (start == end) {
			Path result;
			result.push_back(start);
			return result;
		}

		// Populate vector of current djikstra nodes
		DijkstraGraph djikstraGraph{};
		for (Entity const& e : currentlyActiveNodes) {
			DijkstraNode node{};
      node.parentNode = e;
      node.visited = false;
      node.score = INT_MAX;
			node.previousNodeInPath = NULL;
      djikstraGraph.push_back(node);
    }

		// Set start node score to 0
		for (DijkstraNode& node : djikstraGraph) {
      if (node.parentNode == start) {
        node.score = 0;
      }
    }

		// While there are unvisited nodes
		while (true) {
			DijkstraNode& currentNode{ GetLowestScoreNode(djikstraGraph) };
			currentNode.visited = true;

			for (Entity const& neighbour : gCoordinator->GetComponent<Node>(currentNode.parentNode).neighbours) {
				for (DijkstraNode& neighbourNode : djikstraGraph) {
          if (neighbourNode.parentNode == neighbour) {
						if (!neighbourNode.visited) {
							int score{ CalculateScore(currentNode, neighbourNode) };
							if (score < neighbourNode.score) {
								neighbourNode.score = score;
								neighbourNode.previousNodeInPath = currentNode.parentNode;
							}
						}
          }
        }
      }

			if (currentNode.parentNode == end) {
				return BuildPath(currentNode, djikstraGraph);
      }

			if (GetLowestScoreNode(djikstraGraph).score == INT_MAX) {
				LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "No path could be found!", __FUNCTION__);
        break;
      }
		}

		return Path();
	}

	/*  _________________________________________________________________________ */
	/*! PrintPath

	@param path
	The path to print.

	@return none.

	Prints the path to the console.
	*/
	void NodeManager::PrintPath(Path const& path) {
		if (path.size() == 1) {
			std::cout << path[0] << " -> " << path[0] << "\n";
			return;
		}

		std::cout << "Path: ";
		for (auto const& node : path) {
			std::cout << node << " -> ";
		}
		std::cout << "End Reached\n";
	}
}