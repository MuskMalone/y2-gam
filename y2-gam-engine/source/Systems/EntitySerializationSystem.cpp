/******************************************************************************/
/*!
\par        Entity serialization System
\file       EntitySerializationSystem.cpp

\author     Ng Wen Wen (wenwen.ng@digipen.edu)
\date       Sep 10, 2023

\brief      Implementation file for entity serialization

			This file contains the Entity Serialization System that facilitates
			the creation, manipulation, and file storage of JSON data for entities
			in a software application or game.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "../include/pch.hpp"

#include <Systems/EntitySerializationSystem.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Serialization/SerializationManager.hpp>
#include <Core/Serialization/SerializerComponent.hpp>
#include <Core/Serialization/Serializer.hpp>
#include "Core/Serialization/Serializer.hpp"

using namespace Serializer;
namespace {
	std::shared_ptr<Coordinator> gCoordinator{};
}
namespace Serializer {
	/*  _________________________________________________________________________ */
	/*! Init

	   This function initializes the Entity Serialization System by setting up event listeners
	   and other necessary components.
	*/
	void EntitySerializationSystem::Init() {
		gCoordinator = Coordinator::GetInstance();
		::gCoordinator->AddEventListener(METHOD_LISTENER(Events::System::ENTITY, EntitySerializationSystem::EntityEventListener));
	}

	/*  _________________________________________________________________________ */
	/*! LoadEntities

	   @param name
	   The name of the JSON file to load entities from.

	   This function loads entities from a specified JSON file.
	*/
	void EntitySerializationSystem::LoadEntities(std::string const& name) {
		std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

		if (!sm->OpenJSON(name)) return;

		if (!sm->At("Entities").IsArray()) return;
		for (auto const& item : sm->At("Entities").GetArray()) {
			if (!item.IsObject()) continue;
			Entity entity{ gCoordinator->CreateEntity() };
			for (auto itr = item.MemberBegin(); itr != item.MemberEnd(); ++itr) {
				auto at{ gComponentSerializer.find(itr->name.GetString()) };
				if (at == gComponentSerializer.end()) continue;
				at->second(entity, itr->value);
			}
		}
	}
	/*  _________________________________________________________________________ */
/*! LoadEntities

   @param name
   The name of the JSON file to load entities from.

   This function loads entities from a specified JSON file.
*/
	void EntitySerializationSystem::LoadEntities(Serializer::JSONObj& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

		if (!obj.IsArray()) return;
		for (auto const& item : obj.GetArray()) {
			if (!item.IsObject()) continue;
			Entity entity{ gCoordinator->CreateEntity() };
			for (auto itr = item.MemberBegin(); itr != item.MemberEnd(); ++itr) {
				auto at{ gComponentSerializer.find(itr->name.GetString()) };
				if (at == gComponentSerializer.end()) continue;
				at->second(entity, itr->value);
			}
		}
	}
	/*  _________________________________________________________________________ */
	/*! FlushEntities

	   @param name
	   The name of the JSON file to flush entities to.

	   This function writes the current state of entities to a specified JSON file.
	   It serializes the entities and their components, then saves the data to the file.
	*/
	void EntitySerializationSystem::FlushEntities(std::string const& name) {
		std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
		sm->ClearJSON(name);
		sm->SetObject(name);
		JSONObj entArr{ JSON_ARR_TYPE };
		for (auto const& entity : mEntities) {
			JSONObj obj{ JSON_OBJ_TYPE };
			SerializeEntity(entity, obj);
			if (!obj.ObjectEmpty())
				sm->PushToArray(name, entArr, obj);
		}

		sm->InsertValue("Entities", entArr);
		SerializationManager::GetInstance()->FlushJSON(name);

	}
	/*  _________________________________________________________________________ */
/*! FlushEntities

   @param name
   The name of the JSON file to flush entities to.

   This function writes the current state of entities to a specified JSON file.
   It serializes the entities and their components, then saves the data to the file.
*/
	void EntitySerializationSystem::FlushEntities(Serializer::JSONObj& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
		if (!obj.IsArray()) return;
		for (auto const& entity : mEntities) {
			JSONObj entityObj{ JSON_OBJ_TYPE };
			SerializeEntity(entity, entityObj);
			if (!entityObj.ObjectEmpty())
				sm->PushToArray(obj, entityObj);
		}
	}
	/*  _________________________________________________________________________ */
	/*! EntityEventListener

	   @param event
	   The event object containing entity creation information.

	   This function is called when an entity is created, and it adds a SerializerComponent
	   to the entity for serialization.
	*/
	void EntitySerializationSystem::EntityEventListener(Event& event) {
		//every item added to the ecs will be marked as an entity to serialize
		Entity e{ event.GetParam<Entity>(Events::System::Entity::CREATE) };
		if (event.GetFail()) return;
		gCoordinator->AddComponent<SerializerComponent>(e, {});

		}
}