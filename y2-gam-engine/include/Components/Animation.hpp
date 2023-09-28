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
	unsigned int spriteIdx;
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