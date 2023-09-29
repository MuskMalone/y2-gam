#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       ComponentManager.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      rigidbody ecs cmpnt

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "ComponentArray.hpp"
#include "Types.hpp"
#include <any>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <type_traits>

class ComponentManager
{
public:
	/*  _________________________________________________________________________ */
/*! RegisterComponent

@return none.

Registers a new component type. Ensures that the component type is not
already registered.
*/

/*  _________________________________________________________________________ */
	template<typename T>
	void RegisterComponent()
	{
		//static_assert(&T::Serialize); //all components need to have a create function
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		mComponentTypes.insert({typeName, mNextComponentType});
		mComponentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		++mNextComponentType;
	}

	/*  _________________________________________________________________________ */
	/*! GetComponentType

	@return ComponentType corresponding to the template type T.

	Retrieves the component type for the specified template type T. Ensures that
	the component type has been registered.
	*/
	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();
		#ifdef _DEBUG
				//std::cout << "Type Name: " << typeName << std::endl;
		#endif
		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return mComponentTypes[typeName];
	}
	/*  _________________________________________________________________________ */
/*! AddComponent

@param entity The entity to which the component will be added.
@param component The component data to be added.

@return none.

Adds a component of type T to the specified entity.
*/
	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}
	/*  _________________________________________________________________________ */
/*! RemoveComponent

@param entity The entity from which the component will be removed.

@return none.

Removes a component of type T from the specified entity.
*/
	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}
	/*  _________________________________________________________________________ */
/*! GetComponent

@param entity The entity whose component data of type T is to be retrieved.

@return Reference to the component data of type T of the specified entity.

Retrieves the component data of type T for the specified entity.
*/
	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}
	void CloneComponents(Entity from, Entity to) {
		for (auto const& pair : mComponentArrays)
			if (pair.second->FindData(from))
				pair.second->CloneData(from, to);
		/*  _________________________________________________________________________ */
/*! EntityDestroyed

@param entity The entity that has been destroyed.

@return none.

Handles the scenario when an entity is destroyed. If the entity has components,
they will be notified of the entity's destruction.
*/

	}

private:
	std::unordered_map<const char*, ComponentType> mComponentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};
	ComponentType mNextComponentType{};


	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};
