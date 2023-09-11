#pragma once

#include "glm/glm.hpp"
class Camera {
public:
	Camera(float left, float right, float bottom, float top);

	glm::vec3 const& GetPosition() const;

	void SetPosition(glm::vec3 const& pos);
	void SetRotation(float rot);

	glm::mat4 const& GetProjMtx() const;
	glm::mat4 const& GetViewMtx() const;
	glm::mat4 const& GetViewProjMtx() const;
private:
	void ComputeViewMtx();

	glm::mat4 mProjMtx;
	glm::mat4 mViewMtx;
	glm::mat4 mViewProjMtx;

	glm::vec3 mPos{};
	float mRot{ 0.f };
};