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
  class NodeManager {
  public:
    using Cost = int;

  public:
    static void Update();
    static void DisplayDebugLines();

    static void AddNode();
    static void AddNeighbour(Entity lhs, Entity rhs);

    static void RemoveNode(Entity node);
    static void ClearAllNodes();

    static int CalculateCost(Entity lhs, Entity rhs);
    static void FillCostMap();
    static void PrintCostMap();

  private:

  private:
    static std::set<Entity> currentlyActiveNodes;
    static std::map<std::pair<Entity, Entity>, Cost> costMap;
  };
}
