#include "Components/BoxCollider.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/Player.hpp"
#include "Components/Renderable.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Thrust.hpp"
#include "Components/Transform.hpp"
#include "Components/Editor.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "WindowManager.hpp"
#include <chrono>
#include <random>
#include <Core/Globals.hpp>
#include "Graphics/Renderer.hpp"
#include <Core/FrameRateController.hpp>

#include <memory>


namespace {
	static bool quit = false;
}


void QuitHandler(Event& event)
{
	quit = true;
}

int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	using namespace Physics;
	using namespace Collision;
	std::shared_ptr<Coordinator> coordinator{ Coordinator::GetInstance() };
	coordinator->Init();


	std::shared_ptr<WindowManager> windowManager{WindowManager::GetInstance()};
	windowManager->Init("ENGINE", ENGINE_SCREEN_WIDTH, ENGINE_SCREEN_HEIGHT, 0, 0);
	
	std::shared_ptr<FrameRateController> frameController {FrameRateController::GetInstance()};
	frameController->Init(60, true);
	coordinator->AddEventListener(FUNCTION_LISTENER(Events::Window::QUIT, QuitHandler));

	coordinator->RegisterComponent<Editor>();
	coordinator->RegisterComponent<BoxCollider>();
	coordinator->RegisterComponent<Camera>();
	coordinator->RegisterComponent<Gravity>();
	coordinator->RegisterComponent<Renderable>();
	coordinator->RegisterComponent<RigidBody>();
	coordinator->RegisterComponent<Thrust>();
	coordinator->RegisterComponent<Transform>();


	auto physicsSystem = coordinator->RegisterSystem<PhysicsSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Gravity>());
		signature.set(coordinator->GetComponentType<RigidBody>());
		coordinator->SetSystemSignature<PhysicsSystem>(signature);
	}

	physicsSystem->Init();

	auto collisionSystem = coordinator->RegisterSystem<CollisionSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<RigidBody>());
		signature.set(coordinator->GetComponentType<BoxCollider>());
		coordinator->SetSystemSignature<CollisionSystem>(signature);
	}

	collisionSystem->Init();


	auto editorControlSystem = coordinator->RegisterSystem<EditorControlSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Editor>());
		signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<EditorControlSystem>(signature);
	}

	editorControlSystem->Init();

	auto inputSystem = coordinator->RegisterSystem<InputSystem>();
	{
		Signature signature{};
		coordinator->SetSystemSignature<EditorControlSystem>(signature);
	}

	inputSystem->Init();


	auto renderSystem = coordinator->RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Renderable>());
		signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<RenderSystem>(signature);
	}

	renderSystem->Init();

	float dt = frameController->GetDeltaTime();

	while (!quit && !windowManager->ShouldClose())
	{
		frameController->StartFrameTime();
		inputSystem->Update();

		windowManager->ProcessEvents();

		editorControlSystem->Update(dt);

		physicsSystem->PreCollisionUpdate(dt);

		collisionSystem->Update(dt);

		physicsSystem->PostCollisionUpdate(dt);

		collisionSystem->Debug(); // for debug

		renderSystem->Update(dt);

		windowManager->Update();

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = frameController->EndFrameTime();
		std::string title = "FPS: " + std::to_string(frameController->GetFps()) + " | Entities: " + std::to_string(coordinator->GetEntityCount());
		windowManager->UpdateWindowTitle(title);

	}

	Renderer::Shutdown();
	windowManager->Shutdown();

	return 0;
}
