#pragma once

namespace Image {
  class NodeManager {
  public:
    using Cost = int;

  public:
    static void DisplayDebugLines();

    static void AddNode();
    static void RemoveNode(Entity node);
    static void ClearAllNodes();

    static void CalculateCost(Entity lhs, Entity rhs);
    static void FillCostMap();
    
  private:
    static std::vector<Entity> currentlyActiveNodes;
    static std::map<std::pair<Entity, Entity>, Cost> costMap;
  };
}
