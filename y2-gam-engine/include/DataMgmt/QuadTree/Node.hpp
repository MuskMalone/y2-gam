

#pragma once

//#include "Circle.h"
#include "DataMgmt/QuadTree/Rect.hpp"
//#include "Templates.h"
#include <vector>
#include "Core/Types.hpp"
namespace DataMgmt {
	class Node {
	private:
		Rect mBounds;
		std::vector<Entity> mIndex;

	public:
		Node(const Rect& r) : mBounds{ r } {}

		void insert(Entity const& id) { mIndex.emplace_back(id); }

		void get(std::vector<std::vector<Entity>>& cont) {
			cont.emplace_back(mIndex);
			mIndex.clear();
			mIndex.shrink_to_fit();
		}
		void draw() const {
			// Draw the nodes boundaries
			mBounds.draw();
		}
		//void color_objects();

		template <typename _pred>
		bool contain(Entity const& id, _pred) const { return mBounds.contain(id, p); }
	};
}

