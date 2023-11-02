#include "../include/pch.hpp"

#include "Systems/EditorControlSystem.hpp"
#include "Core/Coordinator.hpp"
#include <Systems/InputSystem.hpp>
#include "Systems/RenderSystem.hpp"
#include "Systems/CollisionSystem.hpp"

#include <Core/Globals.hpp>
#include "Math/MathUtils.h"
#include "DataMgmt/DecisionTree/DecisionTree.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Audio/Sound.hpp"
#include <Engine/PrefabsManager.hpp>
#include <Engine/AssetManager.hpp>
#include "Scripting/NodeManager.hpp"

static bool isClicked = false;

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
	Image::Sound sampleEffect;
	Image::Sound jumpEffect;
	Image::Sound attackEffect;
	Image::Sound bgm;
	Image::SoundGroup bgmGroup;
	Image::SoundGroup effectGroup;

	float gravity = -90.f;
	float massOfPlayer = 10.f;
}
namespace Testing {
	std::default_random_engine generator;
	Entity lastInserted;
	Entity enemy;

	Vec2 confirmNodesButtonPos{ 50.f, 20.f };
	Vec2 resetNodesButtonPos{ 90.f, 20.f };
}

void EditorControlSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();

	// TO DELETE
	using namespace std::chrono;
	Testing::generator.seed(static_cast<unsigned int>(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	).count()));
	Entity entity;
	//Entity entity = ::gCoordinator->CreateEntity();
	//::gCoordinator->AddComponent<Gravity>(
	//	entity,
	//	{ Vec2(0.0f, -10.f) });
	Vec3 position = Vec3(0, -WORLD_LIMIT_Y,1);
	//::gCoordinator->AddComponent(
	//	entity,
	//	Collider{
	//		Vec2(position), .0f, Vec2(2.f * WORLD_LIMIT_X, 5.f)
	//	});
	//::gCoordinator->AddComponent(
	//	entity,
	//	RigidBody{
	//		Vec2(position), .0f, FLOAT_MAX, Vec2(2.f * WORLD_LIMIT_X, 5.f)
	//	});
	//::gCoordinator->AddComponent(
	//	entity,
	//	Transform{
	//		{position.x,position.y,position.z},
	//		{0.f,0.f,0.f},
	//		{2 * WORLD_LIMIT_X, 5.f, 1.f}
	//	});

	//entity = ::gCoordinator->CreateEntity();
	//::gCoordinator->AddComponent<Gravity>(
	//	entity,
	//	{ Vec2(0.0f, -10.f) });
	//position = Vec3(-WORLD_LIMIT_X, 0, 1);
	//::gCoordinator->AddComponent(
	//	entity,
	//	Collider{
	//		Vec2(position), .0f, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
	//	});
	//::gCoordinator->AddComponent(
	//	entity,
	//	RigidBody{
	//		Vec2(position), .0f, FLOAT_MAX, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
	//	});
	//::gCoordinator->AddComponent(
	//	entity,
	//	Transform{
	//		{position.x,position.y,position.z},
	//		{0.f,0.f,0.f},
	//			{5.f, 2 * WORLD_LIMIT_Y, 1.f}
	//	});

	//entity = ::gCoordinator->CreateEntity();
	//::gCoordinator->AddComponent<Gravity>(
	//	entity,
	//	{ Vec2(0.0f, -10.f) });
	//position = Vec3(WORLD_LIMIT_X, 0, 1);
	//::gCoordinator->AddComponent(
	//	entity,
	//	Collider{
	//		Vec2(position), .0f, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
	//	});
	//::gCoordinator->AddComponent(
	//	entity,
	//	RigidBody{
	//		Vec2(position), .0f, FLOAT_MAX, Vec2(5.f, 1.95f * WORLD_LIMIT_Y)
	//	});
	//::gCoordinator->AddComponent(
	//	entity,
	//	Transform{
	//		{position.x,position.y,position.z},
	//		{0.f,0.f,0.f},
	//		{5.f, 2 * WORLD_LIMIT_Y, 1.f}
	//	});
	
	// Platforms for testing
	// Center
	/*entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(0.f, static_cast<float>(-WORLD_LIMIT_Y + 5.f), 1.f);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), 0.f, Vec2( 2.f * WORLD_LIMIT_X - 5.f, 3.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), 0.f, FLOAT_MAX, Vec2(2.f * WORLD_LIMIT_X - 5.f, 3.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{2.f * WORLD_LIMIT_X - 5.f, 3.f, 1.f}
		});
	::gCoordinator->AddComponent(
		entity,
		Sprite{
			{0, 0, 1, 0.7}
		});
	::gCoordinator->AddComponent(
		entity,
		Tag{
      "Platform"
    });*/

	// Left
	/*entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(static_cast<float>(-WORLD_LIMIT_X + 90.f), static_cast<float>(-WORLD_LIMIT_Y + 50.f), 1.f);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), 0.f, Vec2(20.f, 30.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(20.f, 30.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{20.f, 30.f, 1.f}
		});
	::gCoordinator->AddComponent(
		entity,
		Sprite{
			{1, 1, 0, 0.7}
		});
	::gCoordinator->AddComponent(
		entity,
		Tag{
			"Platform"
		});*/

	// Right
	/*entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(static_cast<float>(WORLD_LIMIT_X - 90.f), static_cast<float>(-WORLD_LIMIT_Y + 30.f), 1.f);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), 0.f, Vec2(90.f, 3.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(90.f, 3.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{90.f, 3.f, 1.f}
		});
	::gCoordinator->AddComponent(
		entity,
		Sprite{
			{1, 0, 0, 0.7}
		});
	::gCoordinator->AddComponent(
		entity,
		Tag{
			"Platform"
		});*/

	// Top
	/*entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(static_cast<float>(WORLD_LIMIT_X - 90.f), static_cast<float>(-WORLD_LIMIT_Y + 80.f), 1.f);
	::gCoordinator->AddComponent(
		entity,
		Collider{
			Vec2(position), 0.f, Vec2(90.f, 3.f)
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(90.f, 3.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			{position.x,position.y,position.z},
			{0.f,0.f,0.f},
			{90.f, 3.f, 1.f}
		});
	::gCoordinator->AddComponent(
		entity,
		Sprite{
			{1, 0, 0, 0.7}
		});
	::gCoordinator->AddComponent(
		entity,
		Tag{
			"Platform"
		});*/
		
	// Creating a sample player entity
	//Entity player = ::gCoordinator->CreateEntity();
	//::gCoordinator->AddComponent<Script>(player, { "ObjectPlayer" });

	//position = Vec3(-70.f, -105.f, -150.f);
	//float scale{ 15.f };
	//::gCoordinator->AddComponent<Gravity>(
	//	player,
	//	{ Vec2(0.0f, ::gravity) });
	//::gCoordinator->AddComponent(
	//	player,
	//	Collider{
	//		Vec2(position), 0.f, Vec2(scale / 2.f, scale)
	//	});
	//::gCoordinator->AddComponent(
	//	player,
	//	RigidBody{
	//		Vec2(position), 0.f, ::massOfPlayer, Vec2(scale, scale), true
	//	});
	//::gCoordinator->AddComponent(
	//	player,
	//	Transform{
	//		{position.x,position.y,position.z},
	//		{0.f,0.f,0.f},
	//		{scale, scale, scale}
	//	});
	//::gCoordinator->AddComponent(
	//	player,
	//	Sprite{
	//		{1,1,1,1},
	//		0,
	//		Layer::FOREGROUND
	//	});
	//::gCoordinator->AddComponent(
	//	player,
	//	Text{
	//		"Lato",
	//		0.06f,
	//		"Player",
	//		{1, 1, 0}
	//	});
	//::gCoordinator->AddComponent(
	//	player,
	//	Tag{
	//		"Player"
	//	});
	//Image::ScriptManager::OnCreateEntity(player);

	//// Creating a sample enemy entity
	//Testing::enemy = ::gCoordinator->CreateEntity();
	//::gCoordinator->AddComponent<Script>(Testing::enemy, { "ObjectBasicEnemy" });

	//position = Vec3(40.f, -88.f, -150.f);
	//scale = 15.f;
	//::gCoordinator->AddComponent<Gravity>(
	//	Testing::enemy,
	//	{ Vec2(0.0f, ::gravity) });
	//::gCoordinator->AddComponent(
	//	Testing::enemy,
	//	Collider{
	//		Vec2(position), 0.f, Vec2(scale / 2.f, scale)
	//	});
	//
	//::gCoordinator->AddComponent(
	//	Testing::enemy,
	//	RigidBody{
	//		Vec2(position), 0.f, ::massOfPlayer, Vec2(scale, scale), true
	//	});
	//
	//::gCoordinator->AddComponent(
	//	Testing::enemy,
	//	Transform{
	//		{position.x,position.y,position.z},
	//		{0.f,0.f,0.f},
	//		{scale, scale, scale}
	//	});
	//::gCoordinator->AddComponent(
	//	Testing::enemy,
	//	Sprite{
	//		{1,0,0,1},
	//		0,
	//		Layer::FOREGROUND
	//	});
	//::gCoordinator->AddComponent(
	//	Testing::enemy,
	//	Text{
	//		"Getho",
	//		0.06f,
	//		"Enemy",
	//		{1, 1, 0}
	//	});
	//::gCoordinator->AddComponent(
	//	Testing::enemy,
	//	Tag{
	//		"Enemy"
	//	});
	//Image::ScriptManager::OnCreateEntity(Testing::enemy);


	::gCoordinator->GetSystem<RenderSystem>()->mPlayer = 3; // i think player will always be 3?

	//------------TEMPORARY TO BE READ FROM JSON FILES------------------------------------------------------------------/
	//std::vector<AnimationFrame> idleFrames{ {0.f, 0}, {0.f, 1}, { 0.f, 2 }, { 0.f, 3 }, { 0.f, 4 }, { 0.f, 5 }, { 0.f, 6 }, { 0.f, 7} };
	//std::vector<AnimationFrame> runFrames{ {0.f, 8}, {0.f, 9}, { 0.f, 10 }, { 0.f, 11 }, { 0.f, 12 }, { 0.f, 13 }, { 0.f, 14 }, { 0.f, 15 } };
	//std::vector<AnimationFrame> attackFrames{ {0.f, 16}, {0.f, 17}, { 0.f, 18 }, { 0.f, 19 }, { 0.f, 20 }, { 0.f, 21 }, { 0.f, 22 } };
	//std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> map{ {ANIM_STATE::IDLE, idleFrames},
	//																 {ANIM_STATE::RUN, runFrames},
	//																 {ANIM_STATE::ATTACK, attackFrames} };
	// 
	//std::vector<AnimationFrame> idleFrames{ {0.f, 0}, {0.f, 0}, { 0.f, 0}, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 } };
	//std::vector<AnimationFrame> runFrames{ {0.f, 0}, {0.f, 0}, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 } };
	//std::vector<AnimationFrame> attackFrames{ {0.f, 0}, {0.f, 0}, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 }, { 0.f, 0 } };
	//std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> map{ {ANIM_STATE::IDLE, idleFrames},
	//																 {ANIM_STATE::RUN, runFrames},
	//																 {ANIM_STATE::ATTACK, attackFrames} };
	//::gCoordinator->AddComponent(
	//	player,
	//	Animation{
	//		0.08f,
	//		0,
	//		ANIM_STATE::IDLE
	//	});

	//::gCoordinator->AddComponent(
	//	Testing::enemy,
	//	Animation{
	//		0.08f,
	//		0,
	//		ANIM_STATE::IDLE
	//	});

	// Create a button for FinishAddingNodes
	//Entity button = ::gCoordinator->CreateEntity();
	//float scaleButton{ 5.f };
	//::gCoordinator->AddComponent(
 //   button,
 //   Transform{
 //     {Testing::confirmNodesButtonPos.x, Testing::confirmNodesButtonPos.y, 1.f},
 //     {0.f, 0.f, 0.f},
 //     {scaleButton * 6, scaleButton, scaleButton}
 //   });
	//::gCoordinator->AddComponent(
	//	button,
 //   Sprite{
 //     {1, 1, 1, 1}
 //   });
	//::gCoordinator->AddComponent(
	//	button,
 //   Text{
 //     "Lato",
 //     0.03f,
 //     "Finish Adding Nodes",
 //     {0, 0, 0}
 //   });
	//::gCoordinator->AddComponent(
	//	button,
	//	Collider{
	//	  Vec2(Testing::confirmNodesButtonPos), 0.f, Vec2(scaleButton * 6, scaleButton)
 //   });

	//::gCoordinator->AddComponent(
	//	button,
	//	Tag{
	//		"FinishAddingNodes"
	//	});

	// Create a button for Resetting Nodes
	/*button = ::gCoordinator->CreateEntity();
	scaleButton = 5.f;
	::gCoordinator->AddComponent(
		button,
		Transform{
			{Testing::resetNodesButtonPos.x, Testing::resetNodesButtonPos.y, 1.f},
			{0.f, 0.f, 0.f},
			{scaleButton * 6, scaleButton, scaleButton}
		});
	::gCoordinator->AddComponent(
		button,
		Sprite{
			{1, 1, 1, 1}
		});
	::gCoordinator->AddComponent(
		button,
		Text{
			"Lato",
			0.03f,
			"Reset Nodes",
			{0, 0, 0}
		});
	::gCoordinator->AddComponent(
		button,
		Collider{
			Vec2(Testing::resetNodesButtonPos), 0.f, Vec2(scaleButton * 6, scaleButton)
		});

	::gCoordinator->AddComponent(
		button,
		Tag{
			"ResetNodes"
		});*/

	// Sound Testing
	::sampleEffect = Image::SoundManager::AudioLoadSound("../assets/audio/teleport.wav");
	::bgm = Image::SoundManager::AudioLoadSound("../assets/audio/bgm.wav");
	::attackEffect = Image::SoundManager::AudioLoadSound("../assets/audio/playerAttack.wav");
	::jumpEffect = Image::SoundManager::AudioLoadSound("../assets/audio/playerJump.wav");
	::bgmGroup = Image::SoundManager::AudioCreateGroup();
	::effectGroup = Image::SoundManager::AudioCreateGroup();
	//by right this id value should be passed by the imgui asset browser
	auto am{ AssetManager::GetInstance() };
	//soundEffect = am->GetAsset<Image::SoundManager>(am->LoadAsset<Image::SoundManager>(1698648904161047500));
	bgm = am->GetAsset<Image::SoundManager>(am->LoadAsset<Image::SoundManager>(1698648904161071400));
	bgmGroup = Image::SoundManager::AudioCreateGroup();
	effectGroup = Image::SoundManager::AudioCreateGroup();

	Image::SoundManager::AudioPlay(::bgm, ::bgmGroup, -1);
	Image::SoundManager::AudioPauseGroup(::bgmGroup);
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

	auto& camera = ::gCoordinator->GetComponent<Camera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera());
	auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_1)) {
		Image::SoundManager::AudioResumeGroup(::bgmGroup);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_2)) {
		Image::SoundManager::AudioPauseGroup(::bgmGroup);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_3)) {
		Image::SoundManager::AudioPlay(::sampleEffect, ::effectGroup, 0);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_4)) {
		Image::SoundManager::AudioPlay(::attackEffect, ::effectGroup, 0);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_SPACE)) {
		Image::SoundManager::AudioPlay(::jumpEffect, ::effectGroup, 0);
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
		::gCoordinator->CloneEntity(Testing::lastInserted);
	}

	// NODE RELATED START

	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::RB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		Physics::RayHit rh{};
		Vec2 mousePos{ inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Raycast(mousePos, mousePos, rh, std::nullopt);
		if (!::gCoordinator->HasComponent<Node>(rh.entityID)) {
			NodeManager::AddNode();
		}
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {

		Physics::RayHit rh{};

		Vec2 mousePos{ inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Raycast(mousePos, mousePos, rh, std::nullopt);
		
		if (::gCoordinator->HasComponent<Node>(rh.entityID)) {
			::gCoordinator->GetComponent<Node>(rh.entityID).selected = !(::gCoordinator->GetComponent<Node>(rh.entityID).selected);
		}

		if (::gCoordinator->HasComponent<Tag>(rh.entityID)) {
      if (::gCoordinator->GetComponent<Tag>(rh.entityID).tag == "FinishAddingNodes") {
				//::gCoordinator->GetComponent<Text>(rh.entityID).text = "Nodes Set!";
        NodeManager::FinishAddingNodes();
      }
      else if (::gCoordinator->GetComponent<Tag>(rh.entityID).tag == "ResetNodes") {
        NodeManager::ClearAllNodes();
      }
    }
	}
	// Temporarily Disabled
	/*
	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) && !isClicked) {
		Physics::RayHit rh{};

		Vec2 mousePos{ inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Raycast(mousePos, mousePos, rh, std::nullopt);

		if (::gCoordinator->HasComponent<Node>(rh.entityID)) {
			NodeManager::ChangeNodeType(rh.entityID);
		}

		isClicked = true;
	}
	else {
		isClicked = false;
	}
	*/

	/*
	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_RELEASED, static_cast<size_t>(MouseButtons::RB)) &&
    inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		Physics::RayHit rh{};
		Vec2 mousePos{ inputSystem->GetWorldMousePos().first, inputSystem->GetWorldMousePos().second };
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Raycast(mousePos, mousePos, rh);

		if (::gCoordinator->HasComponent<Node>(rh.entityID)) {
			//NodeManager::RemoveNode(rh.entityID);
		}
  }
	*/

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_M) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		NodeManager::ClearAllNodes();
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_L) &&
    inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		Entity playerID = ::gCoordinator->GetSystem<RenderSystem>()->mPlayer;
		std::cout << "Closest node to player: " << NodeManager::FindClosestNodeToEntity(playerID) << std::endl;
		std::cout << "Closest node to enemy: " << NodeManager::FindClosestNodeToEntity(Testing::enemy) << std::endl;
    NodeManager::Path path = NodeManager::DjkstraAlgorithm(NodeManager::FindClosestNodeToEntity(Testing::enemy),
			NodeManager::FindClosestNodeToEntity(playerID));

		// Print the shortest path
    NodeManager::PrintPath(path);
  }

	// NODE RELATED END

	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {
		Testing::lastInserted = PrefabsManager::GetInstance()->SpawnPrefab("Circle");
		for (int i{}; i < 10; ++i) {
			//std::cout << i << std::endl;
			Testing::lastInserted = ::gCoordinator->CloneEntity(Testing::lastInserted);

		}

	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {

		Testing::lastInserted = PrefabsManager::GetInstance()->SpawnPrefab("Box");
		for (int i{}; i < 10; ++i) {
			//std::cout << i << std::endl;
			Testing::lastInserted = ::gCoordinator->CloneEntity(Testing::lastInserted);

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
						0,
						Layer::FOREGROUND
					}
				);
				::gCoordinator->AddComponent(
					entity,
					Animation{
						0.08f,
						0,
						ANIM_STATE::IDLE,
				});
			}
		}

	}

}
