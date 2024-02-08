/******************************************************************************/
/*!
\par        Image Engine
\file       AnimationSystem.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 12, 2023

\brief      Implementation file for the AnimationSystem class.

			The AnimationSystem class is responsible for managing the animations
			of entities within the game. It handles the progression of animation
			frames, responds to input events to change the animation state, and
			integrates with other systems to ensure smooth and synchronized
			animations.

			This system also provides functionalities for loading and managing
			animation textures, and it interfaces with the Sprite component to
			display the correct animation frames based on the current animation state.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include "Systems/AnimationSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Components/Animation.hpp"
#include "Components/Sprite.hpp"
#include "Systems/InputSystem.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Graphics/AnimationManager.hpp"
#include "Core/FrameRateController.hpp"
#include "Systems/RenderSystem.hpp"
#include "Engine/SceneManager.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*!
\brief Init Function

Initializes the animation system, setting up necessary resources and loading textures for animations.
*/
void AnimationSystem::Init() {
	::gCoordinator = Coordinator::GetInstance();

}

/*  _________________________________________________________________________ */
/*!
\brief Update Function

Updates the animation system based on the given delta time. This function handles the progression of animation frames and responds to input events to change the animation state.

\param dt The time elapsed since the last frame.
*/
void AnimationSystem::Update(float dt) {
	for (auto const& entity : mEntities)
	{
		auto& sprite = gCoordinator->GetComponent<Sprite>(entity);
		auto& animation = gCoordinator->GetComponent<Animation>(entity);
		if (animation.states.empty()) continue;
		if (animation.currState >= animation.states.size()) animation.currState = animation.states.size() - 1;
		size_t& frameIdx { animation.currFrame };
		//if (!(animation.stateMap[currState]) || animation.stateMap[currState] == static_cast<AssetID>(-1)) continue;
		//quick patch to constcast this

		//this check is to remove any invalid asset ids
		if (!AssetManager::GetInstance()->IsAssetExist(animation.states[animation.currState])) {
			animation.states.erase(animation.states.begin() + animation.currState);
			if (animation.currState != 0) animation.currState--;
			continue;
		}

		AnimationFrames& frameList{ const_cast<AnimationFrames&>(AssetManager::GetInstance()->GetAsset<AnimationManager>(animation.states[animation.currState])) };

		if (frameIdx >= frameList.size())
			frameIdx = 0;

		AnimationFrame & currFrame { frameList[frameIdx] };

		//xavier todo: help me change this to not use elapsed time
		std::shared_ptr<Coordinator> coordinator {Coordinator::GetInstance()};
		auto renderSystem = coordinator->GetSystem<RenderSystem>();
		if (!renderSystem->IsEditorMode()) {
			currFrame.elapsedTime += dt;
		}

		sprite.spriteID = currFrame.spriteID;

		if (currFrame.elapsedTime >= animation.speed) {
			++frameIdx;
			currFrame.elapsedTime = 0.f;
		}
	}
}

void AnimationSystem::ResetFrame(Entity entity) {
	if (gCoordinator->HasComponent<Animation>(entity)) {
		auto& animation = gCoordinator->GetComponent<Animation>(entity);
		animation.currFrame = 0;
		if (!animation.states.empty() && animation.currState < animation.states.size()) {
			auto& frameList = const_cast<AnimationFrames&>(AssetManager::GetInstance()->GetAsset<AnimationManager>(animation.states[animation.currState]));
			if (!frameList.empty()) {
				frameList[0].elapsedTime = 0.0f; // Reset elapsed time of the first frame
			}
		}
	}
}

void AnimationSystem::CheckAssetValidity()
{
	bool changed{ false };
	for (auto const& e : mEntities) {
		auto& animation = gCoordinator->GetComponent<Animation>(e);
		//this check is to remove any invalid asset ids
		if (animation.states.empty()) continue;
		if (!AssetManager::GetInstance()->IsAssetExist(animation.states[animation.currState])) {
			animation.states.erase(animation.states.begin() + animation.currState);
			if (animation.currState != 0) animation.currState--;
			changed = true;
		}
	}
	if (changed) {
		//std::cout << "changed anim\n";
		SceneManager::GetInstance()->SaveScene();
	}
}
