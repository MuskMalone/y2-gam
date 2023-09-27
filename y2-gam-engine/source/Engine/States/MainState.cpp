#include <Engine/States/MainState.hpp>
#include <Core/Coordinator.hpp>
#include <memory>
#include <Core/Serialization/SerializationManager.hpp>
#include <Core/FrameRateController.hpp>

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
	float tdt{ FrameRateController::GetInstance()->GetTargetDT() };
	coordinator->GetSystem<EditorControlSystem>()->Update(dt);

	coordinator->GetSystem<PhysicsSystem>()->PreCollisionUpdate(tdt);

	coordinator->GetSystem<CollisionSystem>()->Update(tdt);

	coordinator->GetSystem<PhysicsSystem>()->PostCollisionUpdate(tdt);

	coordinator->GetSystem<AnimationSystem>()->Update(dt);

	coordinator->GetSystem<RenderSystem>()->Update(dt);
	//mCollisionSystem->Debug(); // for debug
}
void MainState::Render(float dt) {

}