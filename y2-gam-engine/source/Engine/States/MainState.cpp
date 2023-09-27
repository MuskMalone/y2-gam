#include <Engine/States/MainState.hpp>
#include <Core/Coordinator.hpp>
#include <memory>
#include <Core/Serialization/SerializationManager.hpp>
#include <Core/FrameRateController.hpp>

void MainState::Init() {
	using namespace Physics;
	using namespace Collision;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	mPhysicsSystem = coordinator->GetSystem<PhysicsSystem>();
	mCollisionSystem = coordinator->GetSystem<CollisionSystem>();
	mEditorControlSystem = coordinator->GetSystem<EditorControlSystem>();
	mRenderSystem = coordinator->GetSystem<RenderSystem>();
	mAnimationSystem = coordinator->GetSystem<AnimationSystem>();
	mEntitySerializationSystem = coordinator->GetSystem<Serializer::EntitySerializationSystem>();


	using namespace Serializer;
	mEntitySerializationSystem->LoadEntities("LevelData");
}
void MainState::Exit() {
	using namespace Serializer;
	mEntitySerializationSystem->FlushEntities("LevelData");
}

void MainState::Update(float dt) {
	mEditorControlSystem->Update(dt);

	mPhysicsSystem->PreCollisionUpdate(FrameRateController::GetInstance()->GetTargetDT());

	mCollisionSystem->Update(FrameRateController::GetInstance()->GetTargetDT());

	mPhysicsSystem->PostCollisionUpdate(FrameRateController::GetInstance()->GetTargetDT());

	//mCollisionSystem->Debug(); // for debug
}
void MainState::Render(float dt) {
	mAnimationSystem->Update(dt);

	mRenderSystem->Update(dt);
}