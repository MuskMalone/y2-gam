#include "../include/pch.hpp"

#include <Engine/States/MainState.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Serialization/SerializationManager.hpp>
#include <Core/FrameRateController.hpp>
#include <Systems/InputSystem.hpp>
#include <Engine/SceneManager.hpp>
void MainState::Init() {
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	mIsStep = false;
	using namespace Serializer;
	//coordinator->GetSystem<Serializer::EntitySerializationSystem>()->LoadEntities("LevelData");
	//SceneManager::GetInstance()->LoadScene("Scene1");
}
void MainState::Exit() {
	using namespace Serializer;
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	//coordinator->GetSystem<Serializer::EntitySerializationSystem>()->FlushEntities("LevelData");
	//SceneManager::GetInstance()->ExitScene("Scene1");
}

void MainState::Update(float dt) {
	using namespace Physics;
	using namespace Collision;

	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	auto inputSystem = coordinator->GetSystem<InputSystem>();
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_BACKSPACE))
		mIsStep = !mIsStep;
	float tdt{ FrameRateController::GetInstance()->GetTargetDT() };
	coordinator->GetSystem<EditorControlSystem>()->Update(dt);
	auto renderSystem = coordinator->GetSystem<RenderSystem>();
	//todo tch: hacky way to do this pls change
	if (!renderSystem->IsEditorMode()) {
		//std::cout << renderSystem->IsEditorMode() << std::endl;
	if (mIsStep) {
		if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_0)) {
			coordinator->GetSystem<PhysicsSystem>()->PreCollisionUpdate(tdt);
			coordinator->GetSystem<CollisionSystem>()->Update(tdt);
			coordinator->GetSystem<PhysicsSystem>()->PostCollisionUpdate(tdt);
		}
	}
	else {
		static float accumulatedTime = 0.f;
		const float maxAccumulation{ 0.1f };
		accumulatedTime += dt;
		if (accumulatedTime > maxAccumulation) accumulatedTime = maxAccumulation;
		if (accumulatedTime >= tdt) {
			FrameRateController::GetInstance()->StartSubFrameTime();
			coordinator->GetSystem<PhysicsSystem>()->PreCollisionUpdate(tdt);
			FrameRateController::GetInstance()->EndSubFrameTime(ENGINE_PHYSICS_PROFILE);
			FrameRateController::GetInstance()->StartSubFrameTime();
			coordinator->GetSystem<CollisionSystem>()->Update(tdt);
			FrameRateController::GetInstance()->EndSubFrameTime(ENGINE_COLLISION_PROFILE);
			FrameRateController::GetInstance()->StartSubFrameTime();
			coordinator->GetSystem<PhysicsSystem>()->PostCollisionUpdate(tdt);
			FrameRateController::GetInstance()->EndSubFrameTime(ENGINE_PHYSICS_PROFILE);
			accumulatedTime -= tdt;
		}
	}
	}
	//mCollisionSystem->Debug(); // for debug
}
void MainState::Render(float dt) {
	std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
	FrameRateController::GetInstance()->StartSubFrameTime();
	coordinator->GetSystem<AnimationSystem>()->Update(dt);

	coordinator->GetSystem<RenderSystem>()->Update(dt);
	FrameRateController::GetInstance()->EndSubFrameTime(ENGINE_RENDER_PROFILE);

}