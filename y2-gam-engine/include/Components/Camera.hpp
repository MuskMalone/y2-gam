#pragma once

#include "iostream"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <Core/Globals.hpp>
#include <rapidjson/document.h>

#define M_PI (22.f/7.f)

struct Camera
{
	glm::vec3 eye{};
	glm::vec3 up{};
	glm::vec3 tgt{};
	glm::mat4 projectionTransform{};
	void UpdatePos(float x, float y) {
		eye = glm::vec3{ x,y,0 };
		tgt = eye + glm::vec3{0, 0, 1};
	}
	static glm::mat4 MakeProjectionTransform(float l, float r, float b, float t, float n, float f)
	{
		return glm::ortho(l, r, b, t, n, f);
		//return glm::ortho(fov, static_cast<float>(viewWidth), static_cast<float>(viewHeight), nearClip, farClip);//transform;
	}
	static glm::mat4 MakeViewTransform(glm::vec3 const& eye, glm::vec3 const& tgt, glm::vec3 const& up) {
		return glm::lookAt(eye, tgt, up);
	}
	Camera() : eye{}, up{ glm::vec3{0,1,0} }, tgt{ eye + glm::vec3{0,0,1} }, projectionTransform{ MakeProjectionTransform(-WORLD_LIMIT_X, WORLD_LIMIT_X, -WORLD_LIMIT_Y, WORLD_LIMIT_Y, -WORLD_LIMIT_DEPTH, WORLD_LIMIT_DEPTH) } {

		
	}
	Camera([[maybe_unused]] rapidjson::Value const& obj) {}

	bool Serialize([[maybe_unused]]  rapidjson::Value& obj) {
		return false;
	}
};
