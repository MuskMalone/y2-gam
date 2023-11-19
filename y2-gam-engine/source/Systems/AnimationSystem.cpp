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

	//-------------TEMPORARY-------------------------------------------------------------/

	//SpriteManager::LoadTexture("../assets/textures/Idle.png", 0);
	//SpriteManager::LoadTexture("../assets/textures/Run.png", 1);
	//SpriteManager::LoadTexture("../assets/textures/Attack_1.png", 2);
	//AnimationManager::LoadAnimation("../assets/textures/ROBIN_ANIM_Spritesheet.png",
	//	0, 7, 1, { 256, 256 });
	//AnimationManager::LoadAnimation("../assets/textures/ROBIN_ANIM_Spritesheet.png",
	//	1, 16, 2, { 256, 256 });
	//AnimationManager::LoadAnimation("../assets/textures/ROBIN_ANIM_Spritesheet.png",
	//	2, 23, 0, { 256, 256 });

	//ResourceID texrid{ SpriteManager::LoadTexture("../assets/textures/ROBIN_ANIM_Spritesheet.png") };

	////TEMP
	//for (float i{}; i < 7; ++i)
	//	SpriteManager::CreateSubTexture(texrid, SpriteProperties{ GetTimestampNano(), {i, 1}, {256, 256} });
	//

	//for (float i{ 7 }; i < 16; ++i)
	//	SpriteManager::CreateSubTexture(texrid, SpriteProperties{ GetTimestampNano(), { i, 2 }, { 256, 256 } });

	//for (float i{ 16 }; i < 23; ++i)
	//	SpriteManager::CreateSubTexture(texrid, SpriteProperties{ GetTimestampNano(), { i, 0 }, { 256, 256 } });

	//------------------------------------------------------------------------------------/

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
		size_t& frameIdx { animation.currFrame };
		//if (!(animation.stateMap[currState]) || animation.stateMap[currState] == static_cast<AssetID>(-1)) continue;
		//quick patch to constcast this
		std::vector<AnimationFrame>& frameList{ const_cast<std::vector<AnimationFrame>&>(AssetManager::GetInstance()->GetAsset<AnimationManager>(animation.states[animation.currState])) };

		if (frameIdx >= frameList.size())
			frameIdx = 0;

		AnimationFrame & currFrame { frameList[frameIdx] };

		//xavier todo: help me change this to not use elapsed time
		currFrame.elapsedTime += dt;

		sprite.spriteID = currFrame.spriteID;

		if (currFrame.elapsedTime >= animation.speed) {
			++frameIdx;
			currFrame.elapsedTime = 0.f;
		}
	}
}
