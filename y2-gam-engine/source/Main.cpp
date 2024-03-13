#include "../include/pch.hpp"

#include "Core/Serialization/SerializerComponent.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EntitySerializationSystem.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/UISystem.hpp"
#include "Systems/AnimationSystem.hpp"
#include "Systems/TextSystem.hpp"
#include "Systems/LayeringSystem.hpp"
#include "Systems/ParticleSystem.hpp"
#include "Systems/LightingSystem.hpp"
#include "WindowManager.hpp"
#include <Core/Globals.hpp>
#include "Graphics/Renderer.hpp"
#include <Core/FrameRateController.hpp>
#include <Engine/StateManager.hpp>
#include <Engine/States/MainState.hpp>
#include <Engine/AssetManager.hpp>
#include <Engine/SceneManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include "Audio/Sound.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Scripting/NodeManager.hpp"
#include "Graphics/FontRenderer.hpp"
#include "Engine/PrefabsManager.hpp"

#ifndef _INSTALLER
#include "DataMgmt/DecisionTree/DecisionTree.hpp"
#include "IMGUI/ImguiComponent.hpp"
#include "Systems/ImguiSystem.hpp"

#else
#include <Windows.h>
#endif
extern "C" { _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; }
namespace {
	static bool quit = false;
}


void QuitHandler([[maybe_unused]] Event& event)
{
	quit = true;
}
std::shared_ptr<Globals::GlobalValContainer>  Globals::GlobalValContainer::_mSelf = 0;

#ifndef _DEBUG
#pragma warning(push)
#pragma warning(disable:28251)
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
		{
#pragma warning(pop)
#else
		int main() {
#endif

	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	
	Globals::GlobalValContainer::GetInstance()->ReadGlobalInts();

	std::shared_ptr<Coordinator> coordinator{ Coordinator::GetInstance() };
	coordinator->Init();
	Image::ScriptManager::Init();
	Image::SoundManager::AudioInit();


	using namespace Physics;
	using namespace Collision;

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
	coordinator->RegisterComponent<Node>();
	coordinator->RegisterComponent<Text>();
	coordinator->RegisterComponent<Prefab>();
	coordinator->RegisterComponent<UIImage>();
	coordinator->RegisterComponent<Swappable>();
	coordinator->RegisterComponent<EmitterSystem>();
	coordinator->RegisterComponent<Light>();
	coordinator->RegisterComponent<LightBlocker>();
#ifndef _INSTALLER
	coordinator->RegisterComponent<ImguiComponent>();
#endif
	coordinator->RegisterComponent<Tag>();
	coordinator->RegisterComponent<Script>();
	coordinator->RegisterComponent<Layering>();
	coordinator->RegisterComponent<Serializer::SerializerComponent>();

	auto assetManager{ AssetManager::GetInstance() };
	assetManager->Init();
	PrefabsManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();


	auto entitySerializationSystem = coordinator->RegisterSystem<Serializer::EntitySerializationSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Serializer::SerializerComponent>());
		coordinator->SetSystemSignature<Serializer::EntitySerializationSystem>(signature);
	}
#ifndef _INSTALLER
	auto imguiSystem = coordinator->RegisterSystem<ImGuiSystem>();
	{
		Signature signature;
		//signature.flip();
		signature.set(coordinator->GetComponentType<ImguiComponent>());
		//signature.set(coordinator->GetComponentType<Tag>());
		//signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<ImGuiSystem>(signature);
	}
#endif
	entitySerializationSystem->Init();

	auto layeringSystem = coordinator->RegisterSystem<LayeringSystem>();
  {
    Signature signature;
    signature.set(coordinator->GetComponentType<Layering>());
    coordinator->SetSystemSignature<LayeringSystem>(signature);
  }

	layeringSystem->Init();

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

	auto uiSystem = coordinator->RegisterSystem<UISystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<UIImage>());
		coordinator->SetSystemSignature<UISystem>(signature);
	}

	uiSystem->Init();

#ifndef _INSTALLER
	imguiSystem->Init(windowManager->GetContext());
#endif
	auto animationSystem = coordinator->RegisterSystem<AnimationSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Sprite>());
		signature.set(coordinator->GetComponentType<Animation>());
		coordinator->SetSystemSignature<AnimationSystem>(signature);
	}

	animationSystem->Init();

	auto particleSystem = coordinator->RegisterSystem<ParticleSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<EmitterSystem>());
		coordinator->SetSystemSignature<ParticleSystem>(signature);
	}

	particleSystem->Init();

	auto lightingSystem = coordinator->RegisterSystem<LightingSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Light>());
		//signature.set(coordinator->GetComponentType<Collider>());
		//signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<LightingSystem>(signature);
	}

	lightingSystem->Init();

	auto lightblockingSystem = coordinator->RegisterSystem<LightBlockingSystem>(); 
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<LightBlocker>());
		//signature.set(coordinator->GetComponentType<Collider>());
		//signature.set(coordinator->GetComponentType<Transform>());
		coordinator->SetSystemSignature<LightBlockingSystem>(signature);
	}


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

	auto prefabSystem = coordinator->RegisterSystem<PrefabsSystem>();
	{
		Signature signature;
		signature.set(coordinator->GetComponentType<Prefab>());
		coordinator->SetSystemSignature<PrefabsSystem>(signature);
	}
	inputSystem->Init();
	
	StateManager::GetInstance()->PushState<MainState>();
	float dt = frameController->GetDeltaTime();

#ifdef _INSTALLER
	SceneManager::GetInstance()->LoadScene("MainMenu");
	ShowCursor(false);
#endif

	while (!quit && !windowManager->ShouldClose())
	{
		dt = frameController->GetDeltaTime();
		float tdt{ FrameRateController::GetInstance()->GetTargetDT() };
		static float accumulatedTime = 0.f;
		const float maxAccumulation{ 0.1f };
		accumulatedTime += dt;
		//LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, std::to_string(accumulatedTime) +" " + std::to_string(dt), __FUNCTION__);
		if (accumulatedTime > maxAccumulation) accumulatedTime = maxAccumulation;
		if (accumulatedTime >= tdt) {
		frameController->StartFrameTime();
			//LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "running" + std::to_string(accumulatedTime) +"," + std::to_string(dt) +"fps:" + std::to_string(frameController->GetFps()), __FUNCTION__);
			inputSystem->Update();
			windowManager->ProcessEvents();
			StateManager::GetInstance()->Update(tdt);
			StateManager::GetInstance()->Render(tdt);

			uiSystem->Update();
			//NodeManager::Update();

			windowManager->Update();
			Image::SoundManager::AudioUpdate();
			auto stopTime = std::chrono::high_resolution_clock::now();

#ifndef _INSTALLER
			FrameRateController::GetInstance()->StartSubFrameTime();

			static bool isEditor{ true };
			if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_K)) {
				isEditor = !isEditor;
			}
			if (isEditor) {
				imguiSystem->Update(tdt,windowManager->GetContext());
				renderSystem->CheckAssetValidity();
				animationSystem->CheckAssetValidity();
				particleSystem->CheckAssetValidity();
			}
			FrameRateController::GetInstance()->EndSubFrameTime(ENGINE_GUI_PROFILE);

#endif

			windowManager->UpdateWindowTitle(WINDOW_TITLE);
			accumulatedTime -= tdt;
			dt = frameController->EndFrameTime(true);
		}
		else if (dt < tdt) {
			frameController->StartFrameTime();
			float dtDiff = tdt - dt;
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(dtDiff * 1000.f)));
			dt = frameController->EndFrameTime();

		}
		//dt = frameController->GetDeltaTime();
		//LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, std::to_string(dt), __FUNCTION__);

		/*if (dt < tdt) {
			std::this_thread::sleep_for(std::chrono::duration<float>(tdt - dt));
			
			LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Hello sleep", __FUNCTION__);
		}*/
	}
	StateManager::GetInstance()->Clear();
#ifndef _INSTALLER
	imguiSystem->Destroy();
#endif
	Renderer::Shutdown();
	particleSystem->Destroy();
	windowManager->Shutdown();
	textSystem->Exit();
	Image::SoundManager::AudioExit();
	assetManager->Exit();
	PrefabsManager::GetInstance()->Exit();
	layeringSystem->Exit();
	Image::ScriptManager::ExitMono();
	return 0;
}
