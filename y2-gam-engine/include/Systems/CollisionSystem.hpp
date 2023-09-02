#pragma once

#include "Core/System.hpp"
#include "DataMgmt/QuadTree/Quadtree.hpp"

class CollisionSystem : public System
{
public:
	void Init();

	void Update(float dt);
private:
	DataMgmt::Quadtree mQuadtree;
};
