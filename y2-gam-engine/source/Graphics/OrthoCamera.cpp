#include "Components/OrthoCamera.hpp"
#include "Core/Globals.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

OrthoCamera::OrthoCamera(float left, float right, float bottom, float top)
	: mProjMtx{ glm::ortho(left, right, bottom, top, static_cast<float>(- WORLD_LIMIT_DEPTH), static_cast<float>(WORLD_LIMIT_DEPTH))}, mViewMtx{1.f} {

	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);

	mViewProjMtx = mProjMtx * mViewMtx;
}

OrthoCamera::OrthoCamera(rapidjson::Value const& obj) {

}

//void OrthoCamera::SetProjectionMtx(float left, float right, float bottom, float top){
//	mProjMtx = glm::ortho(left, right, bottom, top, -1.f, 1.f);
//	mViewMtx = glm::mat4{1.f};
//	mViewProjMtx = mProjMtx * mViewMtx;
//}

void OrthoCamera::SetPosition(glm::vec3 const& pos) {
	mPos = pos;
	ComputeViewMtx();
}

void OrthoCamera::SetRotation(float rot) {
	mRot = rot;
	ComputeViewMtx();
}

glm::vec3 const& OrthoCamera::GetPosition() const {
	return mPos;
}

glm::mat4 const& OrthoCamera::GetProjMtx() const {
	return mProjMtx;
}

glm::mat4 const& OrthoCamera::GetViewMtx() const {
	return mViewMtx;
}

glm::mat4 const& OrthoCamera::GetViewProjMtx() const {
	return mViewProjMtx;
}

void OrthoCamera::ComputeViewMtx() {
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);
	mViewProjMtx = mProjMtx * mViewMtx;
}