#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       ComponentsArray.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      Defines the IComponentArray interface and the ComponentArray class
			which manages components for entities.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "Types.hpp"
#include <array>
#include <cassert>
#include <unordered_map>


class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
	virtual bool FindData(Entity entity) = 0;
	virtual void CloneData(Entity from, Entity to) = 0;
};


template<typename T>
class ComponentArray : public IComponentArray
{
public:
	using Type = T;
	/*  _________________________________________________________________________ */
/*! InsertData

@param entity The entity to which the component will be added.
@param component The component data to be added.

@return none.

Adds a component to the specified entity. Ensures that the component is not
already added to the entity.
*/
	void InsertData(Entity entity, T component)
	{
		assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

		// Put new entry at end
		size_t newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}
	/*  _________________________________________________________________________ */
/*! RemoveData

@param entity The entity from which the component will be removed.

@return none.

Removes a component from the specified entity. Ensures that the component exists
for the entity before removal.
*/
	void RemoveData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
		size_t indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mSize;
	}
	/*  _________________________________________________________________________ */
/*! GetData

@param entity The entity whose component data is to be retrieved.

@return Reference to the component data of the specified entity.

Retrieves the component data for the specified entity. Ensures that the component
exists for the entity.
*/
	T& GetData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");
		return mComponentArray[mEntityToIndexMap[entity]];
	}
	/*  _________________________________________________________________________ */
/*! CloneData

@param from The source entity from which the component data will be cloned.
@param to The destination entity to which the cloned component data will be added.

@return none.

Clones the component data from one entity to another.
*/
	void CloneData(Entity from, Entity to) override {
		InsertData(to, GetData(from));
	}
	/*  _________________________________________________________________________ */
/*! FindData

@param entity The entity whose component existence is to be checked.

@return Boolean value indicating whether the component exists for the specified entity.

Checks if a component exists for the specified entity.
*/

	bool FindData(Entity entity) override {
		return (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end());
	}
	/*  _________________________________________________________________________ */
/*! EntityDestroyed

@param entity The entity that has been destroyed.

@return none.


Handles th

e scenario when an entity is destroyed. If the entity has a component,
it will be removed.
*/
	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
		{
			RemoveData(entity);
		}

	}


private:
	std::array<T, MAX_ENTITIES> mComponentArray{};
	std::unordered_map<Entity, size_t> mEntityToIndexMap{};
	std::unordered_map<size_t, Entity> mIndexToEntityMap{};
	size_t mSize{};
};
