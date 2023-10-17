#pragma once

#include "iostream"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <Core/Globals.hpp>
#include <rapidjson/document.h>

struct Camera
{
public:
	Camera() = default;
	Camera(float ar, float left, float right, float bottom, float top);
	Camera(rapidjson::Value const& obj);

	void SetProjectionMtx(float left, float right, float bottom, float top);

	glm::vec3 const& GetPosition() const;

	void SetPosition(glm::vec3 const& pos);
	void SetRotation(float rot);
	void ZoomIn();
	void ZoomOut();

	glm::mat4 const& GetProjMtx() const;
	glm::mat4 const& GetViewMtx() const;
	glm::mat4 const& GetViewProjMtx() const;

	void ComputeViewProjMtx();
private:
	glm::mat4 mProjMtx{};
	glm::mat4 mViewMtx{};
	glm::mat4 mViewProjMtx{};
public:
	glm::vec3 mPos{};
	float mRot{ 0.f };
	float mZoom{};
	float mMinZoom{ 10.f }, mMaxZoom{ 300.f };
	float mAspectRatio{};

	//Camera([[maybe_unused]] rapidjson::Value const& obj) {}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};
