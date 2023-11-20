#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       EntityManager.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      entity manager

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Types.hpp"
#include <array>
#include <cassert>
#include <queue>
#include <iostream>
#include <exception>

class EntityManager
{
public:
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			mAvailableEntities.push(entity);
		}
	}
	/*  _________________________________________________________________________ */
/*! CreateEntity

@return Entity The newly created entity's ID.

Creates a new entity by assigning an available entity ID. Ensures that the
maximum number of entities is not exceeded.
*/
	Entity CreateEntity()
	{
		std::cout << mLivingEntityCount << "mLivingENtityCountCreate\n";
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");
		if (mLivingEntityCount >= MAX_ENTITIES) throw std::out_of_range{"too many entities	"};
		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return id;
	}
	/*  _________________________________________________________________________ */
/*! DestroyEntity

@param entity The ID of the entity to be destroyed.

@return none.

Destroys the specified entity by resetting its signature and making its ID 
available for future entities.
*/
	void DestroyEntity(Entity entity)
	{
		std::cout << mLivingEntityCount << "mLivingENtityCountDestory-------\n";
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity].reset();
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}
	/*  _________________________________________________________________________ */
/*! SetSignature

@param entity The ID of the entity whose signature is to be set.
@param signature The signature to be set for the entity.

@return none.

Sets the signature for the specified entity.
*/
	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity] = signature;
	}
	/*  _________________________________________________________________________ */
/*! GetSignature

@param entity The ID of the entity whose signature is to be retrieved.

@return Signature The signature of the specified entity.

Retrieves the signature for the specified entity.
*/
	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return mSignatures[entity];
	}
	/*  _________________________________________________________________________ */
/*! GetEntityCount

@return uint32_t The total number of living entities.

Provides the total count of living entities managed by the EntityManager.
*/

	uint32_t GetEntityCount() const { return mLivingEntityCount; }

private:
	std::queue<Entity> mAvailableEntities{};
	std::array<Signature, MAX_ENTITIES> mSignatures{};
	uint32_t mLivingEntityCount{};
};
