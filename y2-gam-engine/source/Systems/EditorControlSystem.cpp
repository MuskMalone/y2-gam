#include "../include/pch.hpp"

#include "Systems/EditorControlSystem.hpp"
#include "Core/Coordinator.hpp"
#include <Systems/InputSystem.hpp>
#include "Systems/RenderSystem.hpp"
#include "Systems/CollisionSystem.hpp"

#include <Core/Globals.hpp>
#include "Components/OrthoCamera.hpp"
#include "Math/MathUtils.h"
#include "DataMgmt/DecisionTree/DecisionTree.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Audio/Sound.hpp"
#include <Engine/PrefabsManager.hpp>
#include <Engine/AssetManager.hpp>
#include "Scripting/NodeManager.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
	Sound soundEffect;
	Sound bgm;
	Image::SoundGroup bgmGroup;
	Image::SoundGroup effectGroup;
}
namespace Testing {
	std::default_random_engine generator;
	Entity lastInserted;
}

void EditorControlSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();

	// TO DELETE
	using namespace std::chrono;
	Testing::generator.seed(static_cast<unsigned int>(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	).count()));

	Entity entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	Vec3 position = Vec3(0, -WORLD_LIMIT_Y,1);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), .0f, Vec2(2.f * WORLD_LIMIT_X, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(2.f * WORLD_LIMIT_X, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{2 * WORLD_LIMIT_X, 5.f, 1.f}
		});

	entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(-WORLD_LIMIT_X, 0, 1);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), .0f, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
				{5.f, 2 * WORLD_LIMIT_Y, 1.f}
		});

	entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(WORLD_LIMIT_X, 0, 1);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), .0f, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{5.f, 2 * WORLD_LIMIT_Y, 1.f}
		});
	
	// Platforms for testing
	// Center
	entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(0.f, static_cast<float>(-WORLD_LIMIT_Y + 10), 1.f);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), 0.f, Vec2(WORLD_LIMIT_X + 50, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(WORLD_LIMIT_X + 50, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{WORLD_LIMIT_X + 50, 5.f, 1.f}
		});
	::gCoordinator->AddComponent(
		entity,
		Sprite{
			{0, 1, 0, 0.7},
			nullptr
		});

	// Left
	entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(static_cast<float>(-WORLD_LIMIT_X + 90), static_cast<float>(-WORLD_LIMIT_Y + 50), 1.f);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), 0.f, Vec2(30.f, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(30.f, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{30.f, 5.f, 1.f}
		});
	::gCoordinator->AddComponent(
		entity,
		Sprite{
			{0, 1, 0, 0.7},
			nullptr
		});

	// Right
	entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(static_cast<float>(WORLD_LIMIT_X - 90), static_cast<float>(-WORLD_LIMIT_Y + 50), 1.f);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), 0.f, Vec2(30.f, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(30.f, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{30.f, 5.f, 1.f}
		});
	::gCoordinator->AddComponent(
		entity,
		Sprite{
			{0, 1, 0, 0.7},
			nullptr
		});
		
	// Creating a sample player entity
	Entity player = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Script>(player, { "ObjectPlayer" });

	position = Vec3(0.f, 0.f, -150.f);
	float scale{ 20.f };
	::gCoordinator->AddComponent<Gravity>(
		player,
		{ Vec2(0.0f, -100.f) });
	::gCoordinator->AddComponent(
		player,
		Collider{
			Vec2(position), 0.f, Vec2(scale, scale)
		});
	::gCoordinator->AddComponent(
		player,
		RigidBody{
			Vec2(position), 0.f, 10.f, Vec2(scale, scale), false
		});
	::gCoordinator->AddComponent(
		player,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{scale, scale, scale}
		});
	::gCoordinator->AddComponent(
		player,
		Sprite{
			{1,1,1,1},
			nullptr,
			Layer::FOREGROUND
		});
	::gCoordinator->AddComponent(
		player,
		Text{
			"Lato",
			0.05f,
			"Player",
			{1, 1, 0}
		});

	::gCoordinator->GetSystem<RenderSystem>()->mPlayer = player;
	//------------TEMPORARY TO BE READ FROM JSON FILES------------------------------------------------------------------/
	std::vector<AnimationFrame> idleFrames{ {0.f, 0}, {0.f, 1}, { 0.f, 2 }, { 0.f, 3 }, { 0.f, 4 }, { 0.f, 5 }, { 0.f, 6 }, { 0.f, 7} };
	std::vector<AnimationFrame> runFrames{ {0.f, 8}, {0.f, 9}, { 0.f, 10 }, { 0.f, 11 }, { 0.f, 12 }, { 0.f, 13 }, { 0.f, 14 }, { 0.f, 15 } };
	std::vector<AnimationFrame> attackFrames{ {0.f, 16}, {0.f, 17}, { 0.f, 18 }, { 0.f, 19 }, { 0.f, 20 }, { 0.f, 21 }, { 0.f, 22 } };
	std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> map{ {ANIM_STATE::IDLE, idleFrames},
																	 {ANIM_STATE::RUN, runFrames},
																	 {ANIM_STATE::ATTACK, attackFrames} };
	::gCoordinator->AddComponent(
		player,
		Animation{
			0.08f,
			0,
			ANIM_STATE::IDLE,
			map
		});

	Image::ScriptManager::OnCreateEntity(player);

	// Sound Testing
	//by right this id value should be passed by the imgui asset browser
	auto am{ AssetManager::GetInstance() };
	soundEffect = am->GetAsset<Image::SoundManager>(am->LoadAsset<Image::SoundManager>(1698506092882384300)); 
	bgm = am->GetAsset<Image::SoundManager>(am->LoadAsset<Image::SoundManager>(1698506092882391200));
	bgmGroup = Image::SoundManager::AudioCreateGroup();
	effectGroup = Image::SoundManager::AudioCreateGroup();

	Image::SoundManager::AudioPlay(bgm, bgmGroup, -1);
	Image::SoundManager::AudioPauseGroup(bgmGroup);
}

void EditorControlSystem::Update(float dt)
{
	// Code to run the 'on update' function on entities with script components
	for (auto const& e : Image::ScriptManager::GetEntityInstances()) {
		Image::ScriptManager::OnUpdateEntity(e.first, dt);
	}

	//TODO REMOVE TEMP
	float moveSpeed = 100.f;
	float rotSpeed = 80.f;
	float zoomSpeed = 100.f;

	auto& camera = ::gCoordinator->GetComponent<OrthoCamera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera());
	auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_1)) {
		Image::SoundManager::AudioResumeGroup(bgmGroup);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_2)) {
		Image::SoundManager::AudioPauseGroup(bgmGroup);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_3)) {
		Image::SoundManager::AudioPlay(soundEffect, effectGroup, 0);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_W)) {
		camera.mPos.y += moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_S)) {
		camera.mPos.y -= moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_A)) {
		camera.mPos.x -= moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_D)) {
		camera.mPos.x += moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_Q)) {
		camera.mRot -= rotSpeed * dt;
		camera.SetRotation(camera.mRot);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_E)) {
		camera.mRot += rotSpeed * dt;
		camera.SetRotation(camera.mRot);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_R)) {
		camera.mZoom += zoomSpeed * dt;
		camera.ZoomIn();
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_F)) {
		camera.mZoom -= zoomSpeed * dt;
		camera.ZoomOut();
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_X)) {
		::gCoordinator->GetSystem<RenderSystem>()->ToggleDebugMode();
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_M)) {
		::gCoordinator->GetSystem<RenderSystem>()->ToggleEditorMode();
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::RB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {
		gCoordinator->CloneEntity(Testing::lastInserted);
	}

	// NODE RELATED START
	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::RB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		NodeManager::AddNode();
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		Physics::RayHit rh{};
		Vec2 mousePos{ inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Raycast(mousePos, mousePos, rh);
		
		//std::cout << "Entity Hit " << rh.entityID << "\n";
		
		if (::gCoordinator->HasComponent<Node>(rh.entityID)) {
			::gCoordinator->GetComponent<Node>(rh.entityID).selected = !(::gCoordinator->GetComponent<Node>(rh.entityID).selected);
		}
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_M) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		NodeManager::ClearAllNodes();
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_N) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		//NodeManager::FillCostMap();
		NodeManager::PrintCostMap();
	}

	// NODE RELATED END

	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {
		Testing::lastInserted = PrefabsManager::GetInstance()->SpawnPrefab("Circle");
		for (int i{}; i < 10; ++i) {
			//std::cout << i << std::endl;
			Testing::lastInserted = gCoordinator->CloneEntity(Testing::lastInserted);

		}

	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {

		Testing::lastInserted = PrefabsManager::GetInstance()->SpawnPrefab("Box");
		for (int i{}; i < 10; ++i) {
			//std::cout << i << std::endl;
			Testing::lastInserted = gCoordinator->CloneEntity(Testing::lastInserted);

		}
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_P)) {

		using namespace Testing;
		std::uniform_real_distribution<float> randPositionY(0.f, 100.f);
		std::uniform_real_distribution<float> randPosition(-WORLD_LIMIT_X, WORLD_LIMIT_X);

		std::uniform_real_distribution<float> randDepth(-1.0f, 0.0f);
		std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
		std::uniform_real_distribution<float> randGravity(-100.f, -50.f);
		std::uniform_real_distribution<float> randVelocity(-10.f, 10.f);

		//------------TEMPORARY TO BE READ FROM JSON FILES------------------------------------------------------------------/
		std::vector<AnimationFrame> idleFrames{ {0.f, 0}, {0.f, 1}, { 0.f, 2 }, { 0.f, 3 }, { 0.f, 4 }, { 0.f, 5 }, { 0.f, 6 }, { 0.f, 7} };
		std::vector<AnimationFrame> runFrames{ {0.f, 8}, {0.f, 9}, { 0.f, 10 }, { 0.f, 11 }, { 0.f, 12 }, { 0.f, 13 }, { 0.f, 14 }, { 0.f, 15 } };
		std::vector<AnimationFrame> attackFrames{ {0.f, 16}, {0.f, 17}, { 0.f, 18 }, { 0.f, 19 }, { 0.f, 20 }, { 0.f, 21 }, { 0.f, 22 } };
		std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> map{ {ANIM_STATE::IDLE, idleFrames},
																		 {ANIM_STATE::RUN, runFrames},
																		 {ANIM_STATE::ATTACK, attackFrames} };

		float scale = 10.f;
		float spacing = 5.f;
		float offsetX = -250.f / 2;
		float offsetY = -250.f / 2; 

		for (int i = 0; i < 50; ++i) {
			for (int j = 0; j < 50; ++j) {
				Entity entity = ::gCoordinator->CreateEntity();
				Vec3 position = Vec3(i * spacing + offsetX, j * spacing + offsetY, randDepth(generator));
				::gCoordinator->AddComponent(
					entity,
					Transform{
						{position.x, position.y, position.z},
						{0.f, 0.f, 0.f},
						{scale, scale, scale}
					}
				);
				::gCoordinator->AddComponent(
					entity,
					Sprite{
						{randColor(generator), randColor(generator), randColor(generator), 1},
						nullptr,
						Layer::FOREGROUND
					}
				);
				::gCoordinator->AddComponent(
					entity,
					Animation{
						0.08f,
						0,
						ANIM_STATE::IDLE,
						map
				});
			}
		}

	}

}
