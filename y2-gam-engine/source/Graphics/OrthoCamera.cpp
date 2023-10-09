/******************************************************************************/
/*!
\par        Image Engine
\file       OrthoCamera.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 10, 2023

\brief      Implementation file for the OrthoCamera Component.

			The OrthoCamera component provides functionalities for orthographic projection
			in a 2D space, including zooming, rotation, and position adjustments.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Components/OrthoCamera.hpp"
#include "Core/Globals.hpp"

/* _________________________________________________________________________ */
/*!
\brief OrthoCamera Constructor

Initializes the orthographic camera with the given aspect ratio and view boundaries.
Configures the projection matrix using the orthographic view and sets up the view matrix
based on the camera's position and rotation. Finally, computes the combined view-projection matrix.
\param ar Aspect ratio of the camera.
\param left Left boundary of the view.
\param right Right boundary of the view.
\param bottom Bottom boundary of the view.
\param top Top boundary of the view.
*/
OrthoCamera::OrthoCamera(float ar, float left, float right, float bottom, float top)
	: mProjMtx{ glm::ortho(left, right, bottom, top, static_cast<float>(-WORLD_LIMIT_DEPTH), static_cast<float>(WORLD_LIMIT_DEPTH)) }, mViewMtx{ 1.f }, mAspectRatio { ar } {

	mZoom = WORLD_LIMIT_Y;
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);

	mViewProjMtx = mProjMtx * mViewMtx;
}

/*  _________________________________________________________________________ */
/*!
\brief OrthoCamera Constructor (JSON)

Constructs an OrthoCamera object using parameters from a JSON object.
\param obj JSON object containing camera parameters.
*/
OrthoCamera::OrthoCamera([[maybe_unused]] rapidjson::Value const& obj) {

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
void OrthoCamera::SetProjectionMtx(float left, float right, float bottom, float top){
	mProjMtx = glm::ortho(left, right, bottom, top, static_cast<float>(-WORLD_LIMIT_DEPTH), static_cast<float>(WORLD_LIMIT_DEPTH));
	ComputeViewProjMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief SetPosition

Sets the camera's position in the world and updates the view-projection matrix.
\param pos New position for the camera.
*/
void OrthoCamera::SetPosition(glm::vec3 const& pos) {
	mPos = pos;
	ComputeViewProjMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief SetRotation

Sets the camera's rotation angle and updates the view-projection matrix.
\param rot New rotation angle for the camera (in degrees).
*/
void OrthoCamera::SetRotation(float rot) {
	mRot = rot;
	ComputeViewProjMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief ZoomIn

Increases the camera's zoom level within the defined limits and updates the matrices.
*/
void OrthoCamera::ZoomIn() {
	mZoom = std::clamp(mZoom, mMinZoom, mMaxZoom);
	SetProjectionMtx(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
	ComputeViewProjMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief ZoomOut

Decreases the camera's zoom level within the defined limits and updates the matrices.
*/
void OrthoCamera::ZoomOut() {
	mZoom = std::clamp(mZoom, mMinZoom, mMaxZoom);
	SetProjectionMtx(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
	ComputeViewProjMtx();
}

/*  _________________________________________________________________________ */
/*!
\brief GetPosition

\return The current position of the camera in the world.
*/
glm::vec3 const& OrthoCamera::GetPosition() const { return mPos; }

/*  _________________________________________________________________________ */
/*!
\brief GetProjMtx

\return The camera's current projection matrix.
*/
glm::mat4 const& OrthoCamera::GetProjMtx() const { return mProjMtx; }

/*  _________________________________________________________________________ */
/*!
\brief GetViewMtx

\return The camera's current view matrix.
*/
glm::mat4 const& OrthoCamera::GetViewMtx() const { return mViewMtx; }

/*  _________________________________________________________________________ */
/*!
\brief GetViewProjMtx

\return The camera's combined view-projection matrix.
*/
glm::mat4 const& OrthoCamera::GetViewProjMtx() const { return mViewProjMtx;}

/*  _________________________________________________________________________ */
/*!
\brief ComputeViewProjMtx

Computes the camera's view matrix based on its position and rotation, and then
calculates the combined view-projection matrix.
*/
void OrthoCamera::ComputeViewProjMtx() {
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);
	mViewProjMtx = mProjMtx * mViewMtx;
}