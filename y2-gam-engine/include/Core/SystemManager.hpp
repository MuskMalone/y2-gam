#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       SystemManager.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief		System manager for all systems

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "System.hpp"
#include "Types.hpp"
#include <cassert>
#include <memory>
#include <unordered_map>


class SystemManager
{
public:/*  _________________________________________________________________________ */
/*! RegisterSystem

@return Shared pointer to the newly registered system of type T.

Registers a new system of type T. Ensures that the system type is not 
already registered.
*/
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({typeName, system});
		return system;
	}
	/*  _________________________________________________________________________ */
/*! SetSignature

@param signature The signature to set for the system.

@return none.

Sets the signature for a system of type T. Ensures that the system has 
been registered before setting its signature.
*/

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) != mSystems.end() && "System used before registered.");

		mSignatures.insert({typeName, signature});
	}
	/*  _________________________________________________________________________ */
/*! EntityDestroyed

@param entity The ID of the entity that has been destroyed.

@return none.

Notifies all systems of the destruction of an entity. Each system will
remove the entity from its set of managed entities.
*/

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;


			system->mEntities.erase(entity);
		}
	}
	/*  _________________________________________________________________________ */
/*! GetSystem

@return Shared pointer to the system of type _system.

Provides access to a system of type _system.
*/

	template <typename _system>
	std::shared_ptr<_system> GetSystem() {
		return std::reinterpret_pointer_cast<_system>(mSystems[typeid(_system).name()]);
	}
	/*  _________________________________________________________________________ */
/*! RemoveSystem

@return none.

Removes a system of type _system from the manager. Also removes its
associated signature.
*/
	template <typename _system>
	void RemoveSystem() {
		
		const char* typeName = typeid(_system).name();
		
		mSystems.erase(typeName);
		mSignatures.erase(typeName);
	}

	/*  _________________________________________________________________________ */
	/*! EntitySignatureChanged

	@param entity The ID of the entity whose signature has changed.
	@param entitySignature The new signature of the entity.

	@return none.

	Updates the systems about a change in an entity's signature. Systems will
	either add or remove the entity based on its new signature.
	*/

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}

private:
	std::unordered_map<const char*, Signature> mSignatures{};
	std::unordered_map<const char*, std::shared_ptr<System>> mSystems{};
};
