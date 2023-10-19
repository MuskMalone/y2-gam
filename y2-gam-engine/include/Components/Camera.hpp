#pragma once

#include "iostream"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <Core/Globals.hpp>
#include <rapidjson/document.h>
#include "Core/Types.hpp"

struct Camera
{
public:
	Camera() = default;
	Camera(float ar, float left, float right, float bottom, float top);
	Camera(rapidjson::Value const& obj);

	void SetProjectionMtx(float left, float right, float bottom, float top);

	glm::vec3 const& GetPosition() const;

	void UpdatePosition(glm::vec3 const& targetPos, Vec2 const& targetVel);
	void SetPosition(glm::vec3 const& pos);
	void SetRotation(float rot);
	void ZoomIn();
	void ZoomOut();

	glm::mat4 const& GetProjMtx() const;
	glm::mat4 const& GetViewMtx() const;
	glm::mat4 const& GetViewProjMtx() const;
	float Lerp(float lhs, float rhs, float t);

	void ComputeViewProjMtx();
private:
	glm::mat4 mProjMtx{};
	glm::mat4 mViewMtx{};
	glm::mat4 mViewProjMtx{};

public:
	Entity mTargetEntity;

	struct CameraSettings{
		float offsetX{30.f};
		float offsetY{20.f};
		float velocityThreshold{0.2f};
		float cameraSpeed{0.1f};

		//function to Load settings TODO IMPLEMENT
	} mSettings;

	glm::vec3 mOffset{};
	glm::vec3 mPos{};

	float mRot{ 0.f };
	float mZoom{};
	float mMinZoom{ 10.f }, mMaxZoom{ 300.f };
	float mAspectRatio{};

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};
