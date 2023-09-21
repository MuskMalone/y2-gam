#include <Systems/EntitySerializationSystem.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Serialization/SerializationManager.hpp>
#include <memory>
namespace {
	std::shared_ptr<Coordinator> gCoordinator{};
}
namespace Serializer {
	void EntitySerializationSystem::Init() {
		gCoordinator = Coordinator::GetInstance();
	}
	void EntitySerializationSystem::LoadEntities() {
		SerializationManager::GetInstance()->OpenJSON("Data/Level1.json");
	}
	void EntitySerializationSystem::FlushEntities() {
		SerializationManager::GetInstance()->FlushJSON("Data/Level1.json");
	}
}