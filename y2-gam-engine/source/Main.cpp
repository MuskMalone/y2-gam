//#define _CRTDBG_MAP_ALLOC	
#include "Components/BoxCollider.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/Sprite.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/Editor.hpp"
#include "Components/Animation.hpp"
#include "Core/Serialization/SerializerComponent.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EntitySerializationSystem.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/AnimationSystem.hpp"
#include "WindowManager.hpp"
#include <chrono>
#include <random>
#include <Core/Globals.hpp>
#include "Graphics/Renderer.hpp"
#include <Core/FrameRateController.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "IMGUI/ImguiComponent.hpp"
#include "Systems/ImguiSystem.hpp"
#include <Engine/StateManager.hpp>
#include <Engine/States/MainState.hpp>
#include <memory>

#include "Audio/Sound.hpp"
#include "Graphics/FontRenderer.hpp"
#include "Scripting/ScriptManager.hpp"
#include <functional>


namespace {
	static bool quit = false;
}


void QuitHandler(Event& event)
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
	coordinator->RegisterComponent<ImguiComponent>();
	coordinator->RegisterComponent<Serializer::SerializerComponent>();



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

	StateManager::GetInstance()->PushState<MainState>();
	float dt = frameController->GetDeltaTime();

	// Font Testing
	Image::FontRenderer::Init();
	Image::FontRenderer::LoadFont("../assets/fonts/arial/arial.ttf", "Arial");
	Image::FontRenderer::SetFontSize("Arial", 100);
	Image::FontRenderer::GenerateBitmap("Arial", 100);

	Image::FontRenderer::LoadFont("../assets/fonts/lato/Lato-Bold.ttf", "Lato");
	Image::FontRenderer::SetFontSize("Lato", 100);
	Image::FontRenderer::GenerateBitmap("Lato", 100);

	Image::FontRenderer::LoadFont("../assets/fonts/getho/GethoLight-7Gal.ttf", "Getho");
	Image::FontRenderer::SetFontSize("Getho", 100);
	Image::FontRenderer::GenerateBitmap("Getho", 100);

	// Audio Testing
	while (!quit && !windowManager->ShouldClose())
	{
		//Renderer::SetClearColor({ 0.f, 1.f, 0.f, 1.f });
		//Renderer::ClearColor();
		//Renderer::ClearDepth();

		Image::SoundManager::AudioUpdate();
		frameController->StartFrameTime();
		inputSystem->Update();
		imguiSystem->Update(windowManager->GetContext());

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
		/*
		std::string title = "FPS: " + std::to_string(frameController->GetFps()) + " | Entities: " + std::to_string(coordinator->GetEntityCount());
		windowManager->UpdateWindowTitle(title);
		*/
		std::string title = "Image Engine";
		windowManager->UpdateWindowTitle(title);

		// Font Testing
		Image::FontRenderer::RenderText("Arial", "Hello World in Arial", -100.f, 100.f, 0.1f, glm::vec3(0.f, 1.f, 1.f));
		Image::FontRenderer::RenderText("Getho", "Hello World in Getho", -100.f, 90.f, 0.1f, glm::vec3(1.f, 0.f, 0.f));
		std::vector<std::string> diagnostics{};
		diagnostics.emplace_back("FPS: " + std::to_string(frameController->GetFps()));
		diagnostics.emplace_back("Entities: " + std::to_string(coordinator->GetEntityCount()));
		diagnostics.emplace_back("Physics: " + std::to_string(frameController->GetProfilerValue(ENGINE_PHYSICS_PROFILE) * 100) + "%");
		diagnostics.emplace_back("Collision: " + std::to_string(frameController->GetProfilerValue(ENGINE_COLLISION_PROFILE) * 100) + "%");
		diagnostics.emplace_back("Render: " + std::to_string(frameController->GetProfilerValue(ENGINE_RENDER_PROFILE) * 100) + "%");

		for (int i{}; i < diagnostics.size(); ++i) {
			Image::FontRenderer::RenderText("Lato", diagnostics[i],
				-WORLD_LIMIT_X, WORLD_LIMIT_Y - (10 + (i * 5)), 0.05f, glm::vec3(0.f, 1.f, 0.f));
		}

	}
	StateManager::GetInstance()->Clear();
	imguiSystem->Destroy();
	Renderer::Shutdown();
	windowManager->Shutdown();

	Image::FontRenderer::Exit();
	Image::SoundManager::AudioExit();
	Image::ScriptManager::Exit();
	return 0;
}
