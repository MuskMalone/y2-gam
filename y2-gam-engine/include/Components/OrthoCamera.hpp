#pragma once

#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include <Core/Serialization/SerializationManager.hpp>
struct OrthoCamera {
public:
	OrthoCamera() = default;
	OrthoCamera(float left, float right, float bottom, float top);
	OrthoCamera(rapidjson::Value const& obj);

	//void SetProjectionMtx(float left, float right, float bottom, float top);

	glm::vec3 const& GetPosition() const;

	void SetPosition(glm::vec3 const& pos);
	void SetRotation(float rot);

	glm::mat4 const& GetProjMtx() const;
	glm::mat4 const& GetViewMtx() const;
	glm::mat4 const& GetViewProjMtx() const;

	void ComputeViewMtx();

	glm::mat4 mProjMtx;
	glm::mat4 mViewMtx;
	glm::mat4 mViewProjMtx;

	glm::vec3 mPos{};
	float mRot{ 0.f };

	void Serialize(rapidjson::Value& obj) {
		std::shared_ptr< Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

	}
};