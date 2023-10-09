/******************************************************************************/
/*!
\par        Image Engine
\file       OrthoCamera.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 10, 2023

\brief      Definition of the OrthoCamera component.

			The OrthoCamera component provides functionalities for orthographic projection
			in a 2D space, including zooming, rotation, and position adjustments.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "glm/glm.hpp"
#include "rapidjson/document.h"
#include <Core/Serialization/SerializationManager.hpp>

struct OrthoCamera {
public:
	OrthoCamera() = default;
	OrthoCamera(float ar, float left, float right, float bottom, float top);
	OrthoCamera(rapidjson::Value const& obj);

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

	glm::mat4 mProjMtx{};
	glm::mat4 mViewMtx{};
	glm::mat4 mViewProjMtx{};

	glm::vec3 mPos{};
	float mRot{ 0.f };
	float mZoom{};
	float mMinZoom{10.f}, mMaxZoom{300.f};
	float mAspectRatio{};

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};