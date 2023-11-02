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
#include <Engine/AssetManager.hpp>

enum class ANIM_STATE {
	NONE = 0,
	IDLE,
	RUN,
	ATTACK
};

struct AnimationFrame {
	float elapsedTime;
	ResourceID spriteID;
};

struct Animation {
	float speed{};
	size_t currFrame{};
	//ResourceID animationID;
	AssetID assetID;
	ANIM_STATE currState{};
	//std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> stateMap;
	Animation() = default;
	Animation(float s, size_t cf, ANIM_STATE cs)
		: speed{ s }, currFrame{ cf }, currState{ cs } {}//, stateMap{ sm } {}
	Animation([[maybe_unused]] rapidjson::Value const& obj) {
		assetID = { obj["assetID"].GetUint64() };
		//texture = nullptr;
		currState = static_cast<ANIM_STATE>(obj["currState"].GetInt());
		speed = { obj["speed"].GetFloat() };
		currFrame = { obj["currFrame"].GetUint64() };
	}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};

		sm->InsertValue(obj, "assetID", assetID);
		sm->InsertValue(obj, "currFrame", currFrame);
		sm->InsertValue(obj, "speed", speed);
		sm->InsertValue(obj, "currState", static_cast<int>(currState));
		return true;
	}
	ResourceID GetAnimationID() {
		return AssetManager::GetInstance()->GetResourceID(assetID);
	}
};