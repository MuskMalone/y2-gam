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

  struct DijkstraNode {
    Entity parentNode;
    bool visited;
    int score;
  };

  class NodeManager {
  public:
    using Cost = int;
    using Path = std::vector<Entity>;

  public:
    // Debug
    static void DisplayDebugLines();

    // Node Management
    static void Update();
    static void AddNode();
    static void AddNeighbour(Entity lhs, Entity rhs);
    static void RemoveNode(Entity node);
    static void ClearAllNodes();

    // Cost Calculation
    static int CalculateCost(Entity lhs, Entity rhs);
    static void PrintCostMap();

    // Pathfinding Algorithm
    static Path DjkstraAlgorithm(Entity start, Entity end);

  private:
    static void FillCostMap();

    // Pathfinding Algorithm Helpers
    static Entity GetLowestScoreNode();
    static int CalculateScore(Entity currentNode, Entity nextNode);
    static Path BuildPath(Entity targetNode);

  private:
    static std::set<Entity> currentlyActiveNodes;
    static std::map<std::pair<Entity, Entity>, Cost> costMap;
  };
}
