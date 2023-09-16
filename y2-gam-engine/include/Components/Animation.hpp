#pragma once
#include <unordered_map>

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
	float speed;
	size_t currFrame;
	ANIM_STATE currState;
	std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> stateMap;
};