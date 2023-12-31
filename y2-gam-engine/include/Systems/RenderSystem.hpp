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

	Entity GetCamera();

	std::shared_ptr<Framebuffer> const& GetFramebuffer() const;
	//for debug
	void ToggleDebugMode();

private:
	bool mDebugMode{false};

	void WindowSizeListener(Event& event);
	std::shared_ptr<Framebuffer> mFramebuffer; //TEMP
	std::shared_ptr<SubTexture> mBgSubtex; // TEMP

	struct RenderEntry {
		Entity entity;
		Transform* transform;
		Sprite* sprite;
	};

	std::vector<RenderEntry> mRenderQueue;

	Entity mCamera{};

};
