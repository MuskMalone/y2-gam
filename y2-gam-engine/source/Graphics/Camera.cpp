#include "Graphics/Camera.hpp"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float left, float right, float bottom, float top)
	: mProjMtx{ glm::ortho(left, right, bottom, top, -1.f, 1.f) }, mViewMtx{ 1.f } {

	mViewProjMtx = mProjMtx * mViewMtx;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << mViewProjMtx[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Camera::SetPosition(glm::vec3 const& pos) {
	mPos = pos;
	ComputeViewMtx();
}

void Camera::SetRotation(float rot) {
	mRot = rot;
	ComputeViewMtx();
}

glm::vec3 const& Camera::GetPosition() const {
	return mPos;
}

glm::mat4 const& Camera::GetProjMtx() const {
	return mProjMtx;
}

glm::mat4 const& Camera::GetViewMtx() const {
	return mViewMtx;
}

glm::mat4 const& Camera::GetViewProjMtx() const {
	return mViewProjMtx;
}

void Camera::ComputeViewMtx() {
	glm::mat4 translate{ glm::translate(glm::mat4(1.0f), mPos) };
	glm::mat4 rotate{ glm::rotate(glm::mat4(1.f), glm::radians(mRot), glm::vec3(0,0,1)) };
	glm::mat4 transform{ translate * rotate };
	mViewMtx = glm::inverse(transform);
	mViewProjMtx = mProjMtx * mViewMtx;
}