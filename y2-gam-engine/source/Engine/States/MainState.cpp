#include <Engine/States/MainState.hpp>
#include <Core/Coordinator.hpp>
#include <memory>
#include <Core/Serialization/SerializationManager.hpp>

void MainState::Init() {
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};

	using namespace Serializer;
	coordinator->GetSystem<Serializer::EntitySerializationSystem>()->LoadEntities("LevelData");
}
void MainState::Exit() {
	using namespace Serializer;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	coordinator->GetSystem<Serializer::EntitySerializationSystem>()->FlushEntities("LevelData");
}

void MainState::Update(float dt) {
	using namespace Physics;
	using namespace Collision;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};

	coordinator->GetSystem<EditorControlSystem>()->Update(dt);

	coordinator->GetSystem<PhysicsSystem>()->PreCollisionUpdate(dt);

	coordinator->GetSystem<CollisionSystem>()->Update(dt);

	coordinator->GetSystem<PhysicsSystem>()->PostCollisionUpdate(dt);

	coordinator->GetSystem<AnimationSystem>()->Update(dt);

	coordinator->GetSystem<RenderSystem>()->Update(dt);
	//mCollisionSystem->Debug(); // for debug
}
void MainState::Render(float dt) {

}