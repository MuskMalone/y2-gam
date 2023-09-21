#include <Engine/States/MainState.hpp>
#include <Core/Coordinator.hpp>
#include <memory>

void MainState::Init() {
	using namespace Physics;
	using namespace Collision;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	mPhysicsSystem = coordinator->GetSystem<PhysicsSystem>();
	mCollisionSystem = coordinator->GetSystem<CollisionSystem>();
	mEditorControlSystem = coordinator->GetSystem<EditorControlSystem>();
	mRenderSystem = coordinator->GetSystem<RenderSystem>();
	mAnimationSystem = coordinator->GetSystem<AnimationSystem>();
}
void MainState::Exit() {

}

void MainState::Update(float dt) {
	mEditorControlSystem->Update(dt);

	mPhysicsSystem->PreCollisionUpdate(dt);

	mCollisionSystem->Update(dt);

	mPhysicsSystem->PostCollisionUpdate(dt);

	mAnimationSystem->Update(dt);

	mRenderSystem->Update(dt);
	//collisionSystem->Debug(); // for debug
}
void MainState::Render(float dt) {

}