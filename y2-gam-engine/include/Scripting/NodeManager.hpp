/******************************************************************************/
/*!
\par        Image Engine
\file       NodeManager.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 14, 2023

\brief      The header file for creating and managing nodes for the purposes
            of AI path graphing.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

namespace Image {
  constexpr int NUM_NODE_TYPES = 2;

  enum class NodeType {
    Walk,
    Jump
  };

  // Colors of the nodes, in the same order as the NodeType enum
constexpr glm::vec4 NODE_COLORS[NUM_NODE_TYPES] = {
    {0, 0, 0, 0.3f}, // Black
    {1, 0, 0, 0.3f}  // Red
  };

  struct DijkstraNode {
    Entity parentNode;                // The 'real' node that represents this node
    bool visited;                     // Has this node been visited?
    int score;                        // The score of this node
    Entity previousNodeInPath;        // The previous node in the path
  };

  class NodeManager {
  public:
    using Cost = int;
    using Path = std::vector<Entity>;
    using DijkstraGraph = std::vector<DijkstraNode>;

  public:
    // Debug
    static void DisplayDebugLines();

    // Node Management
    static void Update();
    static void AddNode();
    static void AddNeighbour(Entity lhs, Entity rhs);
    static void RemoveNode(Entity node);
    static void ClearAllNodes();
    static void ChangeNodeType(Entity node);

    // Cost Calculation
    static int CalculateCost(Entity lhs, Entity rhs);
    static void PrintCostMap();
    static Entity FindClosestNodeToPosition(Vec2 position);
    static Entity FindClosestNodeToEntity(Entity entity);

    // Pathfinding Algorithm
    static Path DjkstraAlgorithm(Entity start, Entity end);
    static void PrintPath(Path const& path);

    // Getters
    static std::set<Entity> const& GetCurrentlyActiveNodes();

  private:
    static void FillCostMap();

    // Helper Functions
    static DijkstraNode& GetLowestScoreNode(DijkstraGraph& dg);
    static int CalculateScore(DijkstraNode currentNode, DijkstraNode nextNode);
    static Path BuildPath(DijkstraNode targetNode, DijkstraGraph dg);
    static float CalculateDistanceSquared(Vec2& lhs, Vec2& rhs);

  private:
    static std::set<Entity> currentlyActiveNodes;
    static std::map<std::pair<Entity, Entity>, Cost> costMap;
  };
}
