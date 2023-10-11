#include "../include/pch.hpp"

#include "Core/Serialization/SerializerComponent.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EntitySerializationSystem.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/AnimationSystem.hpp"
#include "Systems/TextSystem.hpp"
#include "WindowManager.hpp"
#include <Core/Globals.hpp>
#include "Graphics/Renderer.hpp"
#include <Core/FrameRateController.hpp>
#include "IMGUI/ImguiComponent.hpp"
#include "Systems/ImguiSystem.hpp"
#include <Engine/StateManager.hpp>
#include <Engine/States/MainState.hpp>

#include "Audio/Sound.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Scripting/NodeManager.hpp"

#include "Logging/LoggingSystem.hpp"
#include "Logging/backward.hpp"
#include "Engine/PrefabsManager.hpp"


namespace {
	static bool quit = false;
}


void QuitHandler([[maybe_unused]] Event& event)
{
	quit = true;
}
std::shared_ptr<Globals::GlobalValContainer>  Globals::GlobalValContainer::_mSelf = 0;

int main()
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Globals::GlobalValContainer::GetInstance()->ReadGlobalInts();
	// Mono Testing
	Image::ScriptManager::Init();
	MonoAssembly* ma{ Image::ScriptManager::LoadCSharpAssembly("../assets/scripts/y2-gam-script.dll") };
	Image::ScriptManager::PopulateEntityClassesFromAssembly(ma);

	Image::SoundManager::AudioInit();

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
	coordinator->RegisterComponent<Sprite>();
	coordinator->RegisterComponent<RigidBody>();
	coordinator->RegisterComponent<Transform>();
	coordinator->RegisterComponent<Animation>();
	coordinator->RegisterComponent<OrthoCamera>();
	coordinator->RegisterComponent<Script>();
	coordinator->RegisterComponent<Node>();
	coordinator->RegisterComponent<Text>();
	coordinator->RegisterComponent<ImguiComponent>();
	coordinator->RegisterComponent<Serializer::SerializerComponent>();

	auto textSystem = coordinator->RegisterSystem<TextSystem>();
	{
		Signature signature{};
		signature.set(coordinator->GetComponentType<Text>());
		signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<TextSystem>(signature);
	}

	textSystem->Init();

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

	auto renderSystem = coordinator->RegisterSystem<RenderSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Sprite>());
		signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<RenderSystem>(signature);
	}

	renderSystem->Init();

	auto imguiSystem = coordinator->RegisterSystem<ImGuiSystem>();
	{
		Signature signature;
		//signature.flip();
		signature.set(coordinator->GetComponentType<ImguiComponent>());
		//signature.set(coordinator->GetComponentType<Sprite>());
		//signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<ImGuiSystem>(signature);
	}
	imguiSystem->Init(windowManager->GetContext());

	auto animationSystem = coordinator->RegisterSystem<AnimationSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Sprite>());
		signature.set(coordinator->GetComponentType<Animation>());
		coordinator->SetSystemSignature<AnimationSystem>(signature);
	}

	animationSystem->Init();

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

	auto entitySerializationSystem = coordinator->RegisterSystem<Serializer::EntitySerializationSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Serializer::SerializerComponent>());
		coordinator->SetSystemSignature<Serializer::EntitySerializationSystem>(signature);
	}

	entitySerializationSystem->Init();

	PrefabsManager::GetInstance()->Init();

	StateManager::GetInstance()->PushState<MainState>();
	float dt = frameController->GetDeltaTime();

	std::vector<std::string> diagnostics{};
	diagnostics.emplace_back("FPS");
	diagnostics.emplace_back("Entities");
	diagnostics.emplace_back("Physics");
	diagnostics.emplace_back("Collision");
	diagnostics.emplace_back("Render");
	std::vector<Entity> diagnosticsList{};

	for (int i{}; i < diagnostics.size(); ++i) {
		Entity textEnt = coordinator->CreateEntity();
		Vec2 position = Vec2(-WORLD_LIMIT_X, WORLD_LIMIT_Y - static_cast<float>((10 + (i * 5))));
		coordinator->AddComponent(
			textEnt,
			Transform{
				{position.x,position.y,0},
				{0.f,0.f,0.f},
				{0, 0, 0}
			});
		coordinator->AddComponent(
			textEnt,
			Text{
				"Lato",
				0.05f,
				"",
				{1, 1, 0}
			});
		diagnosticsList.push_back(textEnt);
	}

	// Audio Testing
	while (!quit && !windowManager->ShouldClose())
	{
		Image::SoundManager::AudioUpdate();
		frameController->StartFrameTime();
		inputSystem->Update();
		imguiSystem->Update();

		windowManager->ProcessEvents();
		StateManager::GetInstance()->Update(dt);
		StateManager::GetInstance()->Render(dt);

		//physicsSystem->PreCollisionUpdate(dt);

		//collisionSystem->Update(dt);

		//physicsSystem->PostCollisionUpdate(dt);

		//animationSystem->Update(dt);

		//renderSystem->Update(dt);

		windowManager->Update();

		auto stopTime = std::chrono::high_resolution_clock::now();

		dt = frameController->EndFrameTime();
		std::string title = "Image Engine";
		windowManager->UpdateWindowTitle(title);

		int count{};
		for (auto& ent : diagnosticsList) {
			switch (count) {
			case 0:
				coordinator->GetComponent<Text>(ent).text = "FPS: " + std::to_string(frameController->GetFps());
				break;
			case 1:
				coordinator->GetComponent<Text>(ent).text = "Entities: " + std::to_string(coordinator->GetEntityCount());
				break;
			case 2:
				coordinator->GetComponent<Text>(ent).text = "Physics: " + std::to_string(frameController->GetProfilerValue(ENGINE_PHYSICS_PROFILE) * 100) + "%";
				break;
			case 3:
				coordinator->GetComponent<Text>(ent).text = "Collision: " + std::to_string(frameController->GetProfilerValue(ENGINE_COLLISION_PROFILE) * 100) + "%";
				break;
			case 4:
				coordinator->GetComponent<Text>(ent).text = "Render: " + std::to_string(frameController->GetProfilerValue(ENGINE_RENDER_PROFILE) * 100) + "%";
				break;
			}
			
			count++;
		}

		textSystem->Update();

		NodeManager::DisplayDebugLines();
	}
	StateManager::GetInstance()->Clear();
	imguiSystem->Destroy();
	Renderer::Shutdown();
	windowManager->Shutdown();
	textSystem->Exit();

	Image::SoundManager::AudioExit();
	Image::ScriptManager::Exit();
	return 0;
}
