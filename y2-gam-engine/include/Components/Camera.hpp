/******************************************************************************/
/*!
\par        Image Engine
\file       Camera.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 22, 2023

\brief      Header file for the Camera Component.

			The Camera component provides functionalities for managing the view
			and projection matrices. It includes features for
			adjusting the camera's position, rotation, zoom level, and for
			computing the view-projection matrix.

\copyright  Copyright (C) 2023 [Your Company or Institution]. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of [Your Company or Institution] is prohibited.
*/
/******************************************************************************/

#pragma once

#include "iostream"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <Core/Globals.hpp>
#include <rapidjson/document.h>
#include "Core/Types.hpp"

enum class CameraType {
	MainCamera,
	SceneCamera,
	PrefabEditorCamera,
	UICamera,
	NumCameraTypes
};

struct Camera
{
public:
	void SetProjectionMtx(float left, float right, float bottom, float top);

	glm::vec3 const& GetPosition() const;

	void UpdatePosition(glm::vec3 const& targetPos, bool isFacingRight);
	void SetPosition(glm::vec3 const& pos);
	void SetRotation(float rot);
	void ZoomIn();
	void ZoomOut();

	glm::mat4 const& GetProjMtx() const;
	glm::mat4 const& GetViewMtx() const;
	glm::mat4 const& GetViewProjMtx() const;
	float Lerp(float lhs, float rhs, float t);

	void UpdateProjectionMtx();
	void ComputeViewProjMtx();
private:
	glm::mat4 mProjMtx{};
	glm::mat4 mViewMtx{};
	glm::mat4 mViewProjMtx{};


public:
	CameraType type;
	Entity mTargetEntity;

	//CAMERA SETTINGS
	//struct CameraSettings {
	float offsetX{ 0.f };
	float offsetY{ 0.f };
	float velocityThreshold{ 0.2f };
	float cameraSpeed{ 0.05f };
	glm::vec2 horizontalBoundary{ -310.f, 310.f };
	glm::vec2 verticalBoundary{ 0.0f, 1.f };

	//} mSettings;

	glm::vec3 mOffset{ offsetX, offsetY, 0.f };
	glm::vec3 mPos{};

	float mRot{ 0.f };
	float mZoomLevel{ 1.f };
	float mMinZoom{ 0.1f }, mMaxZoom{ 3.f };
	float mAspectRatio{ 1.f };

	Camera() = default;
	Camera(float ar, float left, float right, float bottom, float top);
	Camera(rapidjson::Value const& obj) {

		type = static_cast<CameraType>(obj["type"].GetInt());
		mZoomLevel = obj["mZoomLevel"].GetFloat();
		offsetX = obj["offsetX"].GetFloat();
		offsetY = obj["offsetY"].GetFloat();
		velocityThreshold = obj["velocityThreshold"].GetFloat();
		cameraSpeed = obj["cameraSpeed"].GetFloat();
		horizontalBoundary = glm::vec2{ obj["horizontalBoundaryX"].GetFloat(), obj["horizontalBoundaryY"].GetFloat()};
		verticalBoundary = glm::vec2{ obj["verticalBoundaryX"].GetFloat(), obj["verticalBoundaryY"].GetFloat() };

	}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
		sm->InsertValue(obj, "type", static_cast<int>(type));
		sm->InsertValue(obj, "mZoomLevel", mZoomLevel);
		sm->InsertValue(obj, "offsetX", offsetX );
		sm->InsertValue(obj, "offsetY", offsetY );
		sm->InsertValue(obj, "velocityThreshold", velocityThreshold);
		sm->InsertValue(obj, "cameraSpeed", cameraSpeed );
		sm->InsertValue(obj, "horizontalBoundaryX", horizontalBoundary.x);
		sm->InsertValue(obj, "horizontalBoundaryY", horizontalBoundary.y);
		sm->InsertValue(obj, "verticalBoundaryX", verticalBoundary.x);
		sm->InsertValue(obj, "verticalBoundaryY", verticalBoundary.y);
		return true;
	}
};
