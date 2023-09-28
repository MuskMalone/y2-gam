

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

		void Insert(Entity const& id) { mIndex.emplace_back(id); }

		void Get(std::vector<std::vector<Entity>>& cont) {
			cont.emplace_back(mIndex);
			mIndex.clear();
			mIndex.shrink_to_fit();
		}
		void Draw() const {
			// Draw the nodes boundaries
			mBounds.Draw();
		}
		//void color_objects();

		template <typename _pred>
		bool Contain(Entity const& id, _pred) const { return mBounds.Contain(id, p); }
	};
}

