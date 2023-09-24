#pragma once

#include "iostream"
#include "Math/Mat44.hpp"
#include "Math/Vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <Core/Globals.hpp>
#include <rapidjson/document.h>
#define M_PI (22.f/7.f)

struct Camera
{
	Vec3 eye;
	Vec3 up;
	Vec3 tgt;
	Mat44 projectionTransform;
	void UpdatePos(float x, float y) {
		eye = Vec3{ x,y,0 };
		tgt = eye + Vec3{0, 0, 1};
	}
	static Mat44 MakeProjectionTransform(float l, float r, float b, float t, float n, float f)
	{
		return glm::ortho(l, r, b, t, n, f);
		//return glm::ortho(fov, static_cast<float>(viewWidth), static_cast<float>(viewHeight), nearClip, farClip);//transform;
	}
	static Mat44 MakeViewTransform(Vec3 const& eye, Vec3 const& tgt, Vec3 const& up) {
		return glm::lookAt(eye, tgt, up);
	}
	Camera() : eye{}, up{ Vec3{0,1,0} }, tgt{ eye + Vec3{0,0,1} }, projectionTransform{ MakeProjectionTransform(-WORLD_LIMIT_X, WORLD_LIMIT_X, -WORLD_LIMIT_Y, WORLD_LIMIT_Y, -WORLD_LIMIT_DEPTH, WORLD_LIMIT_DEPTH) } {

		
	}
	Camera(rapidjson::Value const& obj) {}
	bool Serialize(rapidjson::Value& obj) {
		return false;
	}
};
