

#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       Node.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		quad tree node

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
		/*  _________________________________________________________________________ */
/*! Node (Constructor)

@param r The rectangle bounds of the node.

@return none.

Constructs a Node with the specified rectangle bounds.
*/
		Node(const Rect& r) : mBounds{ r } {}

		/*  _________________________________________________________________________ */
/*! Insert

@param id The entity to be inserted into the node.

@return none.

Inserts an entity into the node.
*/
		void Insert(Entity const& id) { mIndex.emplace_back(id); }

		/*  _________________________________________________________________________ */
/*! Get

@param cont Container to store the entities.

@return none.

Retrieves all entities from the node and stores them in the provided
container. Clears the node's entities after retrieval.
*/
		void Get(std::vector<std::vector<Entity>>& cont) {
			cont.emplace_back(mIndex);
			mIndex.clear();
			mIndex.shrink_to_fit();
		}


		/*  _________________________________________________________________________ */
		/*! Draw

		@return none.

		Draws the boundaries of the node.
		*/
		void Draw() const {
			// Draw the nodes boundaries
			mBounds.Draw();
		}
		//void color_objects();

		template <typename _pred>
		bool Contain(Entity const& id, _pred) const { return mBounds.Contain(id, p); }
	};
}

