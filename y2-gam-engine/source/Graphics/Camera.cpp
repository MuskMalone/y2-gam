/******************************************************************************/
/*!
\par        Image Engine
\file       Camera.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 22, 2023

\brief      Implementation file for the Camera Component.

			The Camera component provides functionalities for managing the view
			and projection matrices. It includes features for
			adjusting the camera's position, rotation, zoom level, and for
			computing the view-projection matrix.

\copyright  Copyright (C) 2023 [Your Company or Institution]. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of [Your Company or Institution] is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Components/Camera.hpp"
#include "Core/Globals.hpp"

/*  _________________________________________________________________________ */
/*! Camera

@param ar
The aspect ratio of the camera.

@param left
The left clipping plane.

@param right
The right clipping plane.

@param bottom
The bottom clipping plane.

@param top
The top clipping plane.

This constructor initializes a Camera object with the specified parameters. It sets up the projection matrix using an orthographic projection and initializes the view matrix and aspect ratio. The camera's position, rotation, and zoom level are also set, and the view-projection matrix is calculated accordingly.
*/
Camera::Camera(float ar, float left, float right, float bottom, float top)
	: mProjMtx{ glm::ortho(left, right, bottom, top, static_cast<float>(-WORLD_LIMIT_DEPTH), static_cast<float>(WORLD_LIMIT_DEPTH)) }, mViewMtx{ 1.f }, mAspectRatio{ ar } {

	//mZoom = WORLD_LIMIT_Y;
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);

	mViewProjMtx = mProjMtx * mViewMtx;
}

/*  _________________________________________________________________________ */
/*!
\brief SetProjectionMtx

Updates the camera's projection matrix using the given view boundaries.
\param left Left boundary of the view.
\param right Right boundary of the view.
\param bottom Bottom boundary of the view.
\param top Top boundary of the view.
*/
void Camera::SetProjectionMtx(float left, float right, float bottom, float top) {
	mProjMtx = glm::ortho(left, right, bottom, top, static_cast<float>(-WORLD_LIMIT_DEPTH), static_cast<float>(WORLD_LIMIT_DEPTH));
	ComputeViewProjMtx();
}

void Camera::UpdateProjectionMtx() {
	float left = -WORLD_LIMIT_X * mAspectRatio * mZoomLevel;
	float right = WORLD_LIMIT_X * mAspectRatio * mZoomLevel;
	float bottom = -WORLD_LIMIT_Y * mZoomLevel;
	float top = WORLD_LIMIT_Y * mZoomLevel;
	SetProjectionMtx(left, right, bottom, top);
}

/*  _________________________________________________________________________ */
/*!
\brief UpdateCameraPosition

Updates the camera's position based on the target's position and velocity. Adjusts the camera's offset based on the target's velocity direction and interpolates the camera's position towards the target using the camera's speed setting.
\param targetPos Position of the target (e.g., player) in the world.
\param targetVel Velocity of the target (e.g., player).
*/
void Camera::UpdatePosition(const glm::vec3& targetPosition, bool isFacingRight) {
	float direction = isFacingRight ? 1.0f : -1.0f;
	mOffset.x = direction * offsetX;

	//clamping
	mPos.x = std::max(horizontalBoundary.x, std::min(mPos.x, horizontalBoundary.y));
	mPos.y = std::max(verticalBoundary.x, std::min(mPos.y, verticalBoundary.y));

	float camSpeed = cameraSpeed;
	glm::vec3 currentCamPos = GetPosition();
	glm::vec3 targetCamPos = targetPosition + glm::vec3{offsetX, offsetY, 0.f};
	glm::vec3 newCamPos = currentCamPos;

	newCamPos.x = Lerp(currentCamPos.x, targetCamPos.x, camSpeed);
	newCamPos.y = Lerp(currentCamPos.y, targetCamPos.y, camSpeed);
	SetPosition(newCamPos);
	//Update Zoom

	UpdateProjectionMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief SetPosition

Sets the camera's position in the world and updates the view-projection matrix.
\param pos New position for the camera.
*/
void Camera::SetPosition(glm::vec3 const& pos) {
	mPos = pos;
	ComputeViewProjMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief SetRotation

Sets the camera's rotation angle and updates the view-projection matrix.
\param rot New rotation angle for the camera (in degrees).
*/
void Camera::SetRotation(float rot) {
	mRot = rot;
	ComputeViewProjMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief ZoomIn

Increases the camera's zoom level within the defined limits and updates the matrices.
*/
void Camera::ZoomIn() {

	mZoomLevel = std::min(mZoomLevel, mMaxZoom);
	UpdateProjectionMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief ZoomOut

Decreases the camera's zoom level within the defined limits and updates the matrices.
*/
void Camera::ZoomOut() {
	mZoomLevel = std::max(mZoomLevel, mMinZoom);
	UpdateProjectionMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief GetPosition

\return The current position of the camera in the world.
*/
glm::vec3 const& Camera::GetPosition() const { return mPos; }

/*  _________________________________________________________________________ */
/*!
\brief GetProjMtx

\return The camera's current projection matrix.
*/
glm::mat4 const& Camera::GetProjMtx() const { return mProjMtx; }

/*  _________________________________________________________________________ */
/*!
\brief GetViewMtx

\return The camera's current view matrix.
*/
glm::mat4 const& Camera::GetViewMtx() const { return mViewMtx; }

/*  _________________________________________________________________________ */
/*!
\brief GetViewProjMtx

\return The camera's combined view-projection matrix.
*/
glm::mat4 const& Camera::GetViewProjMtx() const { return mViewProjMtx; }

/*  _________________________________________________________________________ */
/*!
\brief ComputeViewProjMtx

Computes the camera's view matrix based on its position and rotation, and then
calculates the combined view-projection matrix.
*/
void Camera::ComputeViewProjMtx() {
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);
	mViewProjMtx = mProjMtx * mViewMtx;
}

/*  _________________________________________________________________________ */
/*! Lerp

@param lhs
The starting value.

@param rhs
The ending value.

@param t
The interpolation factor.

@return
The linearly interpolated value between lhs and rhs based on the factor t.

This function performs linear interpolation between two values (lhs and rhs) based on the interpolation factor t.
*/
float Camera::Lerp(float lhs, float rhs, float t) {
	return lhs + t * (rhs - lhs);
}