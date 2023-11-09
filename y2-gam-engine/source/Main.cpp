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
#include <Engine/AssetManager.hpp>
#include <Graphics/SpriteManager.hpp>

#include "Audio/Sound.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Scripting/NodeManager.hpp"
#include "Graphics/FontRenderer.hpp"

#include "Logging/LoggingSystem.hpp"
#include "Logging/backward.hpp"
#include "Engine/PrefabsManager.hpp"
#include "DataMgmt/DecisionTree/DecisionTree.hpp"

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

	std::shared_ptr<Coordinator> coordinator{ Coordinator::GetInstance() };
	coordinator->Init();

	// Mono Testing
	Image::ScriptManager::Init();
	MonoAssembly* ma{ Image::ScriptManager::LoadCSharpAssembly("../assets/scripts/y2-gam-script.dll") };
	Image::ScriptManager::PopulateEntityClassesFromAssembly(ma);

	Image::SoundManager::AudioInit();

	using namespace Physics;
	using namespace Collision;

	//assetManager->AddAsset<SoundManager>("../assets/audio/teleport.wav");
	//assetManager->AddAsset<SoundManager>("../assets/audio/bgm.wav");
	//assetManager->AddAsset<SpriteManager>("../assets/textures/blinkbg.png");
	std::shared_ptr<WindowManager> windowManager{WindowManager::GetInstance()};
	windowManager->Init("ENGINE", ENGINE_SCREEN_WIDTH, ENGINE_SCREEN_HEIGHT, 0, 0);
	std::shared_ptr<FrameRateController> frameController {FrameRateController::GetInstance()};
	frameController->Init(60, true);
	coordinator->AddEventListener(FUNCTION_LISTENER(Events::Window::QUIT, QuitHandler));
	coordinator->RegisterComponent<Editor>();
	coordinator->RegisterComponent<Collider>();
	coordinator->RegisterComponent<Camera>();
	coordinator->RegisterComponent<Gravity>();
	coordinator->RegisterComponent<Sprite>();
	coordinator->RegisterComponent<RigidBody>();
	coordinator->RegisterComponent<Transform>();
	coordinator->RegisterComponent<Animation>();
	coordinator->RegisterComponent<Script>();
	coordinator->RegisterComponent<Node>();
	coordinator->RegisterComponent<Text>();
	coordinator->RegisterComponent<ImguiComponent>();
	coordinator->RegisterComponent<Tag>();
	coordinator->RegisterComponent<Serializer::SerializerComponent>();

	auto assetManager{ AssetManager::GetInstance() };
	assetManager->Init();
	PrefabsManager::GetInstance()->Init();

	auto entitySerializationSystem = coordinator->RegisterSystem<Serializer::EntitySerializationSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Serializer::SerializerComponent>());
		coordinator->SetSystemSignature<Serializer::EntitySerializationSystem>(signature);
	}

	auto imguiSystem = coordinator->RegisterSystem<ImGuiSystem>();
	{
		Signature signature;
		//signature.flip();
		signature.set(coordinator->GetComponentType<ImguiComponent>());
		//signature.set(coordinator->GetComponentType<Tag>());
		//signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<ImGuiSystem>(signature);
	}

	entitySerializationSystem->Init();

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
		signature.set(coordinator->GetComponentType<Collider>());
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


	StateManager::GetInstance()->PushState<MainState>();
	float dt = frameController->GetDeltaTime();

	NodeManager::Initialize();

	/*
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
	*/
	while (!quit && !windowManager->ShouldClose())
	{
		Image::SoundManager::AudioUpdate();
		
		inputSystem->Update();

		windowManager->ProcessEvents();
		//gGameLoop.CheckToggleKey();
		frameController->StartFrameTime();
		StateManager::GetInstance()->Update(dt);
		//if (gGameLoop.GetCurrentMode() == DecisionResults::IMGUI_MODE || gGameLoop.GetCurrentMode() == DecisionResults::IMGUI_PLAY_MODE) {
		//}
	//gGameLoop.Evaluate();
		StateManager::GetInstance()->Render(dt);

		NodeManager::Update();
			
		


		windowManager->Update();

		auto stopTime = std::chrono::high_resolution_clock::now();


		imguiSystem->Update(dt);
		dt = frameController->EndFrameTime();
		std::string title = "Image Engine";
		windowManager->UpdateWindowTitle(title);

	}

	
	StateManager::GetInstance()->Clear();
	imguiSystem->Destroy();
	Renderer::Shutdown();
	windowManager->Shutdown();
	textSystem->Exit();

	Image::SoundManager::AudioExit();
	Image::ScriptManager::ExitMono();
	assetManager->Exit();
	return 0;
}
