#pragma once

namespace Image {
  class NodeManager {
  public:
    static void DisplayDebugLines();
    static void AddNode();

  private:
    static std::vector<Entity> currentlyActiveNodes;
  };
}
