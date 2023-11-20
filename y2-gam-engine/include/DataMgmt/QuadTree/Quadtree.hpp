#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       Quadtree.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		quad tree

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

//#include "Circle.h"     // Circle class
//#include "Color.h"      // Color class
//#include "Config.h"     // Global vars, screen size
#include "DataMgmt/QuadTree/Rect.hpp"       // Rectangle class
//#include "Templates.h"  // vec, uptr
#include "Math/MathUtils.h"
#include <vector>
#include <memory>
#include <Core/Types.hpp>
#include <set>
#include <Core/Globals.hpp>
#include <iostream>
#include <Graphics/Renderer.hpp>

// capacity of each node before splitting into more leaves
#define NODE_CAPACITY 32
// max depth of tree
#define NODE_MAX_DEPTH 8
namespace {
	template <class T>
	using vec = std::vector<T>;
	template <class T>
	using uptr = std::unique_ptr<T>;
}
namespace DataMgmt {
	template <typename T>
	class Quadtree {
		int mLevel;
		Rect mRect;
		uptr<Quadtree> mSubnode[4];
		//vec<int> m_index;
		vec<T> mIndex;
		/*  _________________________________________________________________________ */
/*! Split

@return none.

Splits the current node into four subnodes, each representing a quadrant
of the current node's rectangle.
*/
		void Split() {
			//----------------------------------------------------------------
			// Create subnodes and gives each its own quadrant.
			//----------------------------------------------------------------

			Vec2 min = mRect.GetMin();
			Vec2 max = mRect.GetMax();

			float x = min.x;
			float  y = min.y;
			float  width = max.x - min.x;
			float  height = max.y - min.y;

			float  w = static_cast<float>(width) * 0.5f;
			float  h = static_cast<float>(height) * 0.5f;

			Rect SW(Vec2(x, y), Vec2(x + w, y + h));
			Rect SE(Vec2(x + w, y), Vec2(x + width, y + h));
			Rect NW(Vec2(x, y + h), Vec2(x + w, y + height));
			Rect NE(Vec2(x + w, y + h), Vec2(x + width, y + height));

			mSubnode[0] = std::make_unique<Quadtree>(mLevel + 1, SW);
			mSubnode[1] = std::make_unique<Quadtree>(mLevel + 1, SE);
			mSubnode[2] = std::make_unique<Quadtree>(mLevel + 1, NW);
			mSubnode[3] = std::make_unique<Quadtree>(mLevel + 1, NE);
		}/*  _________________________________________________________________________ */
/*! Insert

@param id The object to be inserted.
@param p Predicate function to determine containment.

@return none.

Inserts an object into the quadtree. If the current node has subnodes, 
the object is inserted into the appropriate subnode(s). If the node is 
full and hasn't split yet, it will split and then insert the object.
*/

		template <typename _pred>
		void Insert(const T& id, _pred p) {
			//----------------------------------------------------------------
			// [1] Insert object into subnodes.
			// [2] If split, insert THIS nodes objects into the subnodes.
			// [3] Add object to this node.
			//----------------------------------------------------------------

			// If this subnode has split..
			if (mSubnode[0] != nullptr) {
				// Find the subnodes that Contain it and insert it there.
				if (mSubnode[0]->Contain(id, p)) mSubnode[0]->Insert(id, p);
				if (mSubnode[1]->Contain(id, p)) mSubnode[1]->Insert(id, p);
				if (mSubnode[2]->Contain(id, p)) mSubnode[2]->Insert(id, p);
				if (mSubnode[3]->Contain(id, p)) mSubnode[3]->Insert(id, p);

				return;
			}

			// Add object to this node
			mIndex.emplace_back(id);  // [3]

			// If it has NOT split..and NODE_CAPACITY is reached and we are not at MAX
			// LEVEL..
			if (mIndex.size() > NODE_CAPACITY && mLevel < NODE_MAX_DEPTH) {
				// Split into subnodes.
				Split();

				// Go through all this nodes objects..
				for (const auto& index : mIndex)  // [2]
				{
					// Go through all newly created subnodes..
					for (const auto& subnode : mSubnode) {
						// If they Contain the objects..
						if (subnode->Contain(index, p)) {
							// Insert the object into the subnode
							subnode->Insert(index, p);
						}
					}
				}

				// Remove all indexes from THIS node
				mIndex.clear();
				mIndex.shrink_to_fit();
			}
		}
		/*  _________________________________________________________________________ */
/*! Contain

@param id The object to check containment for.
@param p Predicate function to determine containment.

@return bool Whether the object is contained within the current node's rectangle.

Checks if an object is contained within the current node's rectangle.
*/

		template <typename _pred>
		bool Contain(const T& id, _pred p) const { return mRect.Contain(id, p); }
		bool ContainRect(const Rect& rect) const {
			return mRect.ContainRect(rect);
		}
		//void set_color(const Color &c);

	public:
		Quadtree() = default;
		Quadtree(const int mLevel, const Rect& mRect)
			: mLevel{ mLevel },
			mRect{ mRect },
		  mSubnode{ nullptr, nullptr, nullptr, nullptr } {
		  mIndex.reserve(NODE_CAPACITY);

	  }

	  template <typename _container, typename _pred>
	  void Update(_container const& objSet, _pred p) {
		  //----------------------------------------------------------------
		  // Clear the quadtree and insert it with objects.
		  //----------------------------------------------------------------

		  mIndex.clear();
		  mIndex.shrink_to_fit();

		  mSubnode[0] = nullptr;
		  mSubnode[1] = nullptr;
		  mSubnode[2] = nullptr;
		  mSubnode[3] = nullptr;

		  for (const auto& object : objSet) {
			  //insert(object->get_index());
			  Insert(object, p);
		  }
	  }

	  /*  _________________________________________________________________________ */
	  /*! Get

	  @param cont Container to store the objects.

	  @return none.

	  Retrieves all objects from the deepest nodes of the quadtree and stores
	  them in the provided container.
	  */

	  void Get(vec<vec<T>> &cont) const {
		  //----------------------------------------------------------------
		  // [1] Find the deepest level node.
		  // [2] If there are indexes, add to container.
		  //----------------------------------------------------------------

		  // If this subnode has split..
		  if (mSubnode[0] != nullptr)  // [1]
		  {
			  // Continue down the tree
			  mSubnode[0]->Get(cont);
			  mSubnode[1]->Get(cont);
			  mSubnode[2]->Get(cont);
			  mSubnode[3]->Get(cont);

			  return;
		  }

		  // Insert indexes into our container
		  if (mIndex.size() != 0) cont.emplace_back(mIndex);  // [2]
	  }
	  /*  _________________________________________________________________________ */
/*! Retrieve

@param cont Container to store the objects.
@param rect The rectangle to retrieve objects from.

@return none.

Retrieves all objects that intersect with the provided rectangle and
stores them in the provided container.
*/
	  void Retrieve(vec<T> &cont, const Rect &rect) const {
		  //----------------------------------------------------------------
		  // [1] Find the deepest level node.
		  // [2] If there are indexes, add to container.
		  //----------------------------------------------------------------

		  // If this subnode has split..
		  if (mSubnode[0] != nullptr)  // [1]
		  {
			  // Continue down the tree
			  if (mSubnode[0]->ContainRect(rect)) mSubnode[0]->Retrieve(cont, rect);
			  if (mSubnode[1]->ContainRect(rect)) mSubnode[1]->Retrieve(cont, rect);
			  if (mSubnode[2]->ContainRect(rect)) mSubnode[2]->Retrieve(cont, rect);
			  if (mSubnode[3]->ContainRect(rect)) mSubnode[3]->Retrieve(cont, rect);

			  return;
		  }

		  // Add all indexes to our container
		  for (const auto& index : mIndex) cont.emplace_back(index);
	  }
	  void Debug() const {
		  //----------------------------------------------------------------
		  // [1] Draw this nodes boundaries.
		  // [2] Draw subnodes boundaries.
		  //----------------------------------------------------------------

		  if (mSubnode[0] != nullptr)  // [2]
		  {
			  //// Set color of each subnode
			  //m_subnode[0]->set_color(pastel_red);
			  //m_subnode[1]->set_color(pastel_gray);
			  //m_subnode[2]->set_color(pastel_orange);
			  //m_subnode[3]->set_color(pastel_pink);

			  // Continue down the tree
			  mSubnode[0]->Debug();
			  mSubnode[1]->Debug();
			  mSubnode[2]->Debug();
			  mSubnode[3]->Debug();

			  return;
		  }
		  //// Color the balls in the same color as the boundaries
		  //for (const auto &id : m_index) {
		  //  object_vec[id]->set_temp_color(m_rect.get_color());
		  //}

		  // Only draw the nodes with objects in them.
		  if (mIndex.size() != 0) mRect.Draw();  // [1]
		  // m_rect.draw(); // draw them all '
	  }
	  void Reset() {
		  //----------------------------------------------------------------
		  // Sets bounds to the screens bounds and clears the quadtrees.
		  //----------------------------------------------------------------

		  mRect = Rect(Vec2(static_cast<float>(-WORLD_LIMIT_X), static_cast<float>(-WORLD_LIMIT_Y)), Vec2(static_cast<float>(WORLD_LIMIT_X), static_cast<float>(WORLD_LIMIT_Y)));
		  mIndex.clear();
		  mIndex.shrink_to_fit();

		  mSubnode[0] = nullptr;
		  mSubnode[1] = nullptr;
		  mSubnode[2] = nullptr;
		  mSubnode[3] = nullptr;
	  }
	};

}

//extern Quadtree quadtree;
