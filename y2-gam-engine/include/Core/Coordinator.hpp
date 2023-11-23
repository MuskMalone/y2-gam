#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       Coordinator.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      coordinator for systemmanager, componentmanager, 

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "SystemManager.hpp"
#include "Types.hpp"
#include <memory>


class Coordinator
{
public:
	static std::shared_ptr<Coordinator> GetInstance();
	/*  _________________________________________________________________________ */
/*! Init

@return none.

Initializes the Coordinator by creating instances of the ComponentManager,
EntityManager, EventManager, and SystemManager.
*/

	void Init()
	{
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mEventManager = std::make_unique<EventManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}
	/*  _________________________________________________________________________ */
/*! AddEventListener

@param eventId The ID of the event to listen for.
@param listener The function to be called when the event is triggered.

@return none.

Adds an event listener for a specific event.
*/

	// Event methods
	void AddEventListener(EventId eventId, std::function<void(Event&)> const& listener)
	{
		mEventManager->AddListener(eventId, listener);
	}
	/*  _________________________________________________________________________ */
/*! SendEvent

@param event The event to be sent.

@return none.

Sends an event to all registered listeners.
*/
	void SendEvent(Event& event)
	{
		mEventManager->SendEvent(event);
	}
	/*  _________________________________________________________________________ */
/*! SendEvent

@param eventId The ID of the event to be sent.

@return none.

Sends an event with a specific ID to all registered listeners.
*/

	void SendEvent(EventId eventId)
	{
		mEventManager->SendEvent(eventId);
	}

	void BlockEvent(EventId eventId) {
		mEventManager->BlockEvent(eventId);
	}
	void UnblockEvent(EventId eventId) {
		mEventManager->UnblockEvent(eventId);
	}
	/*  _________________________________________________________________________ */
/*! CreateEntity

@return Entity The newly created entity.

Creates a new entity and notifies all systems of its creation.
*/
	// Entity methods
	Entity CreateEntity()
	{
		Entity out{ mEntityManager->CreateEntity() };
		Event event(Events::System::ENTITY);
		event.SetParam(Events::System::Entity::CREATE, out);
		SendEvent(event);
		return out;
	}
	/*  _________________________________________________________________________ */
/*! CloneEntity

@param entity The entity to be cloned.

@return Entity The cloned entity.

Creates a clone of the specified entity, including all its components.
*/

	Entity CloneEntity(Entity entity) {
		Entity clone = mEntityManager->CreateEntity();
		mComponentManager->CloneComponents(entity, clone);

		auto signature = mEntityManager->GetSignature(entity);
		mEntityManager->SetSignature(clone, signature);
		mSystemManager->EntitySignatureChanged(clone, signature);

		Event event(Events::System::ENTITY);
		event.SetParam(Events::System::Entity::CREATE, clone);
		SendEvent(event);
		return clone;
	}
	/*  _________________________________________________________________________ */
/*! DestroyEntity

@param entity The entity to be destroyed.

@return none.

Destroys the specified entity and notifies all systems of its destruction.
*/

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);

		mComponentManager->EntityDestroyed(entity);

		mSystemManager->EntityDestroyed(entity);

		Event event(Events::System::ENTITY);
#undef DELETE
		event.SetParam(Events::System::Entity::DESTROYED, entity);
#define DELETE                           (0x00010000L)
		SendEvent(event);
	}
	/*  _________________________________________________________________________ */
/*! GetEntityCount

@return uint32_t The total number of entities.

Provides the total count of entities managed by the Coordinator.
*/

	uint32_t GetEntityCount() const {
		return mEntityManager->GetEntityCount();
	}

	/*  _________________________________________________________________________ */
/*! RegisterComponent

@return none.

Registers a new component type with the ComponentManager.
*/
	// Component methods
	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}
	/*  _________________________________________________________________________ */
/*! AddComponent

@param entity The entity to which the component will be added.
@param component The component data to be added.

@return none.

Adds a component of type T to the specified entity and updates its signature.
*/
	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		if (HasComponent<T>(entity)) return;
		mComponentManager->AddComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);
		mSystemManager->EntitySignatureChanged(entity, signature);

		Event event(Events::System::ENTITY);
		event.SetParam(Events::System::Entity::COMPONENT_ADD, entity);
		SendEvent(event);
	}
	/*  _________________________________________________________________________ */
/*! RemoveComponent

@param entity The entity from which the component will be removed.

@return none.

Removes a component of type T from the specified entity and updates its signature.
*/

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);

		Event event(Events::System::ENTITY);
		event.SetParam(Events::System::Entity::COMPONENT_REMOVE, entity);
		SendEvent(event);
	}
	/*  _________________________________________________________________________ */
/*! HasComponent

@param entity The entity to check for the component.

@return bool Whether the entity has the component of type T.

Checks if the specified entity has a component of type T.
*/

	template <typename T>
	bool HasComponent(Entity entity) {
		auto signature = mEntityManager->GetSignature(entity);
		return signature.test(mComponentManager->GetComponentType<T>());
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
		return mComponentManager->GetComponent<T>(entity);
	}
	/*  _________________________________________________________________________ */
/*! GetComponentType

@return ComponentType corresponding to the template type T.

Retrieves the component type for the specified template type T.
*/
	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}
	/*  _________________________________________________________________________ */
/*! RegisterSystem

@return Shared pointer to the newly registered system of type T.

Registers a new system of type T with the SystemManager.
*/

	// System methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}
	/*  _________________________________________________________________________ */
/*! SetSystemSignature

@param signature The signature to set for the system.

@return none.

Sets the signature for a system of type T.
*/

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}
	/*  _________________________________________________________________________ */
/*! GetSystem

@return Shared pointer to the system of type T.

Provides access to a system of type T.
*/
	template<typename T>
	std::shared_ptr<T> GetSystem() {
		return mSystemManager->GetSystem<T>();
	}
	/*  _________________________________________________________________________ */
/*! GetSystem

@return Shared pointer to the system of type T.

Provides access to a system of type T.
*/
	template<typename T>
	std::shared_ptr<T> RemoveSystem() {
		return mSystemManager->RemoveSystem<T>();
	}


private:
	std::unique_ptr<ComponentManager> mComponentManager;
	std::unique_ptr<EntityManager> mEntityManager;
	std::unique_ptr<EventManager> mEventManager;
	std::unique_ptr<SystemManager> mSystemManager;

	static std::shared_ptr<Coordinator> _mSelf;
};
