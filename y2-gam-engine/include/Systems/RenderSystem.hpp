/******************************************************************************/
/*!
\par        Image Engine
\file       RenderSystem.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 7, 2023

\brief      Header file for the RenderSystem class.

			The RenderSystem class is responsible for managing the rendering
			pipeline, including setting up framebuffers, handling debug modes,
			and managing the render queue. It also provides functionalities
			for camera management in the rendering context.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Core/System.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Framebuffer.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"
#include "Components/Transform.hpp"
#include "Components/Sprite.hpp"

class Event;


class RenderSystem : public System
{
public:
	void Init();

	void Update(float dt);

	void RenderPrefab(Entity prefab);
	void RenderUI();

	Entity GetCamera() const;
	Entity GetUICamera() const;

	std::shared_ptr<Framebuffer> const& GetFramebuffer(int) const;
	//for debug
	void ToggleDebugMode();
	void ToggleEditorMode();
	bool GetDebugMode() const { return mDebugMode; }
	bool IsEditorMode()const;
	bool GetPPMode() const { return mPPMode; }
	static void DebugRay(Event& event);
	void CheckAssetValidity();
	void SetSceneCameraZoom(float zoom);
	void SetPPMode(bool b) { mPPMode = b; }
	void SetTimeSlow(bool b) { mIsTimeSlow = b;  }
	void UpdateRadius(float dt);

private:
	bool mDebugMode{ false };
	bool mEditorMode{ true };
	bool mPPMode{ false };

	bool mIsTimeSlow{ false };
	void WindowSizeListener(Event& event);

	float mRadius{};
	
	std::vector<std::shared_ptr<Framebuffer>> mFramebuffers; //TEMP
	std::shared_ptr<SubTexture> mBgSubtex; // TEMP

	struct RenderEntry {
		Entity entity;
		Transform* transform;
		Sprite* sprite;
	};

	std::vector<RenderEntry> mRenderQueue;

	Entity mCamera{}; //Editor Camera
	Entity mSceneCamera{};
	Entity mPrefabEditorCamera{};
	Entity mUICamera{};

	static std::vector<std::pair<std::pair<Vec2, Vec2>, glm::vec4>> mRays;

public:
	Entity mPlayer{}; // TEMP TO BE REMOVED

};
