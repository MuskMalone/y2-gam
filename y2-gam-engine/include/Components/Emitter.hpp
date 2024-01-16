#pragma once
#include <rapidjson/document.h>
#include <pch.hpp>
#define EMT_TYPE_SMOKE 0
#define EMT_TYPE_FIRE 1
#define EMT_TYPE_BURST 2
#define EMT_TYPE_BURSTGRAV 3
#define EMT_TYPE_GRADUAL 4

#define EMT_SHAPE_POINT 1
#define EMT_SHAPE_LINE 2
#define EMT_SHAPE_RECT 3
struct Emitter
{
	Emitter() = default;
	Emitter([[maybe_unused]] rapidjson::Value const& obj) {}
	~Emitter() {}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
	GLuint emitterIdx;
	std::array<glm::vec4, 4> vertices;
	glm::vec4 color;
	glm::vec2 gravity;
	int vCount;
	int type;
	float frequency;
public:
	GLuint EmitterIdx() const { return emitterIdx; }
	GLuint& EmitterIdx() { return emitterIdx; }

};