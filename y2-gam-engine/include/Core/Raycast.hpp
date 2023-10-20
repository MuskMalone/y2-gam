#pragma once

#include <Math/MathUtils.h>

namespace Image {
  struct RaycastHit {
    Vec3 normal;
    Vec3 point;
    float distance;
    unsigned int entityID;
    std::string tag;
  };

  class Raycast {
  public:
    static RaycastHit* performRaycast(Vec3 origin, Vec3 direction, float maxDistance);
  };
}


