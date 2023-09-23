#include <Systems/EntitySerializationSystem.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Serialization/SerializationManager.hpp>
#include <Core/Serialization/SerializerComponent.hpp>
#include <Core/Serialization/Serializer.hpp>
#include <memory>
#include "Core/Serialization/Serializer.hpp"
#include <exception>
using namespace Serializer;
namespace {
	std::shared_ptr<Coordinator> gCoordinator{};
}
namespace Serializer {
	void EntitySerializationSystem::Init() {
		gCoordinator = Coordinator::GetInstance();
		::gCoordinator->AddEventListener(METHOD_LISTENER(Events::System::ENTITY, EntitySerializationSystem::EntityEventListener));
	}
	void EntitySerializationSystem::LoadEntities(std::string const& name) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

		if (!sm->OpenJSON(name)) return;

		if (!sm->At("Entities").IsObject()) return;
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
	void EntitySerializationSystem::FlushEntities(std::string const& name) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
		sm->ClearJSON(name);
		sm->SetObject(name);
		JSONObj entArr{ JSON_ARR_TYPE };
		for (auto const& entity : mEntities) {
			JSONObj obj{ JSON_OBJ_TYPE };
			SerializeEntity(entity, obj);
			sm->PushToArray(name, entArr, obj);
		}
		sm->InsertValue("Entities", entArr);
		SerializationManager::GetInstance()->FlushJSON(name);

	}

	void EntitySerializationSystem::EntityEventListener(Event& event) {
		//every item added to the ecs will be marked as an entity to serialize
		Entity e{ event.GetParam<Entity>(Events::System::Entity::CREATE) };
		if (event.GetFail()) return;
		gCoordinator->AddComponent<SerializerComponent>(e, {});
	
	}
}