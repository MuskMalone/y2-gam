#include "Components/AABBCollider.hpp"
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


namespace {
	static bool quit = false;
}


void QuitHandler(Event& event)
{
	quit = true;
}

int main()
{
	std::shared_ptr<Coordinator> coordinator{ Coordinator::GetCoordinator() };

	coordinator->Init();


	std::shared_ptr<WindowManager> windowManager{WindowManager::GetInstance()};
	windowManager->Init("ENGINE", ENGINE_SCREEN_WIDTH, ENGINE_SCREEN_HEIGHT, 0, 0);


	coordinator->AddEventListener(FUNCTION_LISTENER(Events::Window::QUIT, QuitHandler));

	coordinator->RegisterComponent<Editor>();
	coordinator->RegisterComponent<AABBCollider>();
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
		signature.set(coordinator->GetComponentType<Transform>());
		signature.set(coordinator->GetComponentType<AABBCollider>());
		coordinator->SetSystemSignature<PhysicsSystem>(signature);
	}

	physicsSystem->Init();

	auto collisionSystem = coordinator->RegisterSystem<CollisionSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<RigidBody>());
		signature.set(coordinator->GetComponentType<AABBCollider>());
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

	float dt = 0.0f;

	while (!quit && !windowManager->ShouldClose())
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		inputSystem->Update();

		windowManager->ProcessEvents();

		editorControlSystem->Update(dt);

		physicsSystem->PreCollisionUpdate(dt);

		collisionSystem->Update(dt);

		physicsSystem->PostCollisionUpdate(dt);

		collisionSystem->GetQuadtree()->Draw(); // for debug

		renderSystem->Update(dt);

		windowManager->Update();

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
		std::string title = "FPS: " + std::to_string(1.0 / dt) + " | Entities: " + std::to_string(coordinator->GetEntityCount());
		windowManager->UpdateWindowTitle(title);

	}

	Renderer::Shutdown();
	windowManager->Shutdown();

	return 0;
}
