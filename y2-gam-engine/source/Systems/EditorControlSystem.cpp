#include "../include/pch.hpp"

#include "Systems/EditorControlSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Engine/SceneManager.hpp"

#include <Core/Globals.hpp>
#include "Math/MathUtils.h"
#include "DataMgmt/DecisionTree/DecisionTree.hpp"
#include "Scripting/ScriptManager.hpp"
#include "Audio/Sound.hpp"
#include "Engine/PrefabsManager.hpp"
#include "Engine/AssetManager.hpp"
#include "Scripting/NodeManager.hpp"

static bool isClicked = false;

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
namespace Testing {
	std::default_random_engine generator;
	Entity lastInserted;
}

void EditorControlSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();
	/*
	using namespace std::chrono;
	Testing::generator.seed(static_cast<unsigned int>(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	).count()));
	*/
	//Vec3 position = Vec3(0, -WORLD_LIMIT_Y,1);

	//::gCoordinator->GetSystem<RenderSystem>()->mPlayer = 3; // i think player will always be 3?
}

void EditorControlSystem::Update(float dt)
{
	// Code to run the 'on update' function on entities with script components
	if (SceneManager::GetInstance()->IsSceneActive()) {
		//trying out smth
		std::vector<std::pair<Entity, ScriptInstance>> vscripts{};
		auto const& mscripts{ Image::ScriptManager::GetEntityInstances() };

		std::copy(mscripts.cbegin(), mscripts.cend(), std::back_inserter(vscripts));
		//for (auto const& e : Image::ScriptManager::GetEntityInstances()) {
		for (size_t i{}; i < vscripts.size(); ++i){
			auto const& e{ vscripts[i] };
			if (e.first >= 0 && e.first < MAX_ENTITIES) {
				if (e.second.GetUpdateMethod() != nullptr && e.second.GetInstance()) {
					Image::ScriptManager::OnUpdateEntity(e.first, dt);
				}
			}
		}
	}

	auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();

#ifndef _INSTALLER
	/*
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_1)) {
		//Image::ScriptManager::PrintEntityInstances();
		SoundManager::AudioPlay("PM_Menu_Music_Loop.wav", -1);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_2)) {
		SoundManager::AudioStopGroup(SoundManager::musicGroup);
	}
	
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_3)) {
		SoundManager::AudioPlay("Poltergeist'sMansionTheme_BGM_Loop.wav", -1);
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_4)) {
		SoundManager::AudioResumeGroup(SoundManager::musicGroup);
	}
	*/
#endif

#ifndef _INSTALLER
	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::RB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {
		::gCoordinator->CloneEntity(Testing::lastInserted);
	}
#endif
	// NODE RELATED START
	// Temporarily Disabled
	/*
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


	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_M) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_ALT))) {
		//gCoordinator->GetComponent<Animation>(3).assetID = 1698954259805648300;
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
	*/
	// NODE RELATED END

	//if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
	//	inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {
	//	Testing::lastInserted = PrefabsManager::GetInstance()->SpawnPrefab("Circle");
	//	for (int i{}; i < 10; ++i) {
	//		//std::cout << i << std::endl;
	//		Testing::lastInserted = ::gCoordinator->CloneEntity(Testing::lastInserted);

	//	}

	//}
	//if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
	//	inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {

	//	Testing::lastInserted = PrefabsManager::GetInstance()->SpawnPrefab("Box");
	//	for (int i{}; i < 10; ++i) {
	//		//std::cout << i << std::endl;
	//		Testing::lastInserted = ::gCoordinator->CloneEntity(Testing::lastInserted);
}
