/******************************************************************************/
/*!
\par        Image Engine
\file       Animation.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 10, 2023

\brief      Defines the Animation component for entities.

			This component manages the animation states and frames for entities,
			providing functionalities for serialization and state management.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <unordered_map>
#include <rapidjson/document.h>

enum class ANIM_STATE {
	NONE = 0,
	IDLE,
	RUN,
	ATTACK
};

struct AnimationFrame {
	float elapsedTime;
	int spriteID;
};

struct Animation {
	float speed{};
	size_t currFrame{};
	ANIM_STATE currState{};
	std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> stateMap;
	Animation() = default;
	Animation(float s, size_t cf, ANIM_STATE cs, std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> sm)
		: speed{ s }, currFrame{ cf }, currState{ cs }, stateMap{ sm } {}
	Animation([[maybe_unused]] rapidjson::Value const& obj) {

	}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};