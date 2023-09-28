#include "Components/OrthoCamera.hpp"
#include "Core/Globals.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

OrthoCamera::OrthoCamera(float ar, float left, float right, float bottom, float top)
	: mProjMtx{ glm::ortho(left, right, bottom, top, static_cast<float>(-WORLD_LIMIT_DEPTH), static_cast<float>(WORLD_LIMIT_DEPTH)) }, mViewMtx{ 1.f }, mAspectRatio { ar } {

	mZoom = WORLD_LIMIT_Y;
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);

	mViewProjMtx = mProjMtx * mViewMtx;
}

OrthoCamera::OrthoCamera([[maybe_unused]] rapidjson::Value const& obj) {

}

void OrthoCamera::SetProjectionMtx(float left, float right, float bottom, float top){
	mProjMtx = glm::ortho(left, right, bottom, top, static_cast<float>(-WORLD_LIMIT_DEPTH), static_cast<float>(WORLD_LIMIT_DEPTH));
	ComputeViewProjMtx();
}

void OrthoCamera::SetPosition(glm::vec3 const& pos) {
	mPos = pos;
	ComputeViewProjMtx();
}

void OrthoCamera::SetRotation(float rot) {
	mRot = rot;
	ComputeViewProjMtx();
}

void OrthoCamera::ZoomIn() {
	mZoom = std::clamp(mZoom, mMinZoom, mMaxZoom);
	SetProjectionMtx(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
	ComputeViewProjMtx();
}

void OrthoCamera::ZoomOut() {
	mZoom = std::clamp(mZoom, mMinZoom, mMaxZoom);
	SetProjectionMtx(-mAspectRatio * mZoom, mAspectRatio * mZoom, -mZoom, mZoom);
	ComputeViewProjMtx();
}

glm::vec3 const& OrthoCamera::GetPosition() const { return mPos; }
glm::mat4 const& OrthoCamera::GetProjMtx() const { return mProjMtx; }
glm::mat4 const& OrthoCamera::GetViewMtx() const { return mViewMtx; }
glm::mat4 const& OrthoCamera::GetViewProjMtx() const { return mViewProjMtx;}

void OrthoCamera::ComputeViewProjMtx() {
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);
	mViewProjMtx = mProjMtx * mViewMtx;
}