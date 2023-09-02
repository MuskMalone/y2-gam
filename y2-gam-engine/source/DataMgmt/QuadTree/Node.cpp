//
//
//#include "DataMgmt/QuadTree/Node.hpp"
//#include <iostream>
//namespace DataMgmt {
//	Node::Node(const Rect& r) : mBounds{ r } {}
//
//	template <typename _pred>
//	bool Node::contain(Entity const& id, _pred p) const { return mBounds.contain(id, p); }
//
//	void Node::insert(Entity const& id) { mIndex.emplace_back(id); }
//
//	void Node::draw() const {
//		// Draw the nodes boundaries
//		mBounds.draw();
//	}
//	//
//	//void Node::color_objects() {
//	//  // Color the balls in the same color as the boundaries
//	//  for (const auto &id : m_index) {
//	//    object_vec[id]->set_temp_color(m_bounds.get_color());
//	//  }
//	//}
//
//	void Node::get(std::vector<std::vector<Entity>>& cont) {
//		cont.emplace_back(mIndex);
//		mIndex.clear();
//		mIndex.shrink_to_fit();
//	}
//
//}
