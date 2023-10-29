/******************************************************************************/
/*!
\par        Image Engine
\file       RenderSystem.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 7, 2023

\brief      Implementation file for the RenderSystem class.

			The RenderSystem class is responsible for managing the rendering
			pipeline, including setting up framebuffers, handling debug modes,
			and managing the render queue. It also provides functionalities
			for camera management in the rendering context.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"

#include "Systems/RenderSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Graphics/Shader.hpp"
#include "Core/Globals.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/FontRenderer.hpp"
#include "Systems/TextSystem.hpp"

#include "Scripting/NodeManager.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*!
\brief GetCamera Function

Retrieves the camera entity used in the rendering context.

\return The camera entity.
*/
Entity RenderSystem::GetCamera() { if (mEditorMode)return mCamera; else return mSceneCamera; }

/*  _________________________________________________________________________ */
/*!
\brief GetFramebuffer Function

Retrieves the framebuffer used for rendering.

\return A shared pointer to the framebuffer.
*/
std::shared_ptr<Framebuffer> const& RenderSystem::GetFramebuffer(int idx) const { return mFramebuffers[idx]; }

/*  _________________________________________________________________________ */
/*!
\brief ToggleDebugMode Function

Toggles the debug mode for rendering. Useful for diagnostics.
*/
void RenderSystem::ToggleDebugMode() { mDebugMode = !mDebugMode; }

/*  _________________________________________________________________________ */
/*!
\brief ToggleEditorMode Function

Toggles the editor mode to switch from scene camera/editor camera.
*/
void RenderSystem::ToggleEditorMode() { mEditorMode = !mEditorMode; }

/*  _________________________________________________________________________ */
/*!
\brief IsEditorMode Function

Returns true when editor mode else return false.
*/
bool RenderSystem::IsEditorMode() const{
	return mEditorMode==true;
}
/*  _________________________________________________________________________ */
/*!
\brief Init Function

Initializes the rendering system, setting up necessary resources.
*/
void RenderSystem::Init()
{
	gCoordinator = Coordinator::GetInstance();
	gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::RESIZED, RenderSystem::WindowSizeListener));

	mCamera = gCoordinator->CreateEntity();
	mSceneCamera = gCoordinator->CreateEntity();

	float aspectRatio{ static_cast<float>(ENGINE_SCREEN_WIDTH) / static_cast<float>(ENGINE_SCREEN_HEIGHT) };
	gCoordinator->AddComponent(
		mCamera,
		Camera{aspectRatio, static_cast<float>(-WORLD_LIMIT_X) * aspectRatio, static_cast<float>(WORLD_LIMIT_X) * aspectRatio, static_cast<float>(-WORLD_LIMIT_Y), static_cast<float>(WORLD_LIMIT_Y)}
	);

	gCoordinator->AddComponent(
		mSceneCamera,
		Camera{ aspectRatio, static_cast<float>(-WORLD_LIMIT_X) * aspectRatio * 0.6f, static_cast<float>(WORLD_LIMIT_X) * aspectRatio * 0.6f, static_cast<float>(-WORLD_LIMIT_Y) * 0.6f, static_cast<float>(WORLD_LIMIT_Y) * 0.6f }
	);

	std::shared_ptr<Texture> bgTex = std::make_shared<Texture>( "../assets/textures/blinkbg.png" );
	mBgSubtex = SubTexture::Create(bgTex, { 0, 0 }, { 3497, 1200 });
	Entity bg = gCoordinator->CreateEntity();
	::gCoordinator->AddComponent(
		bg,
		Transform{
			{0, 0, -40.f},
			{0.f,0.f,0.f},
			{350.f, 120.f, 0.f}
		});
	
	::gCoordinator->AddComponent(
		bg,
		Sprite{
			{1.f,1.f,1.f,1.f},
			mBgSubtex,
			Layer::BACKGROUND
		}
	);
	
	Renderer::Init();

	//----------temp------------
	mEditorMode = true;
	FramebufferProps fbProps;
	fbProps.attachments = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::DEPTH };
	fbProps.width = ENGINE_SCREEN_WIDTH;
	fbProps.height = ENGINE_SCREEN_HEIGHT;
	mFramebuffers.push_back(Framebuffer::Create(fbProps));

	fbProps.attachments = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::DEPTH };
	mFramebuffers.push_back(Framebuffer::Create(fbProps));
	//--------------------------
}

/*  _________________________________________________________________________ */
/*!
\brief Update Function

Updates the rendering system based on the given delta time.

\param dt The time elapsed since the last frame.
*/
void RenderSystem::Update([[maybe_unused]] float dt)
{
	mFramebuffers[0]->ClearAttachmentInt(1, -1);
	for (auto const& fb : mFramebuffers) {
		fb->Bind();
		Renderer::SetClearColor({ 0.1f, 0.1f, 0.2f, 1.f });
		Renderer::ClearColor();
		Renderer::ClearDepth();

		mRenderQueue.clear();

		for (auto const& entity : mEntities) {
			RenderEntry entry{
				.entity = entity,
				.transform = &::gCoordinator->GetComponent<Transform>(entity),
				.sprite = &::gCoordinator->GetComponent<Sprite>(entity)
			};
			mRenderQueue.push_back(entry);
		}

		std::sort(mRenderQueue.begin(), mRenderQueue.end(),
			[](RenderEntry const& lhs, RenderEntry const& rhs) {
				// First, sort by layer
				if (lhs.sprite->layer != rhs.sprite->layer) {
					return static_cast<int>(lhs.sprite->layer) < static_cast<int>(rhs.sprite->layer);
				}
				// If they are in the same layer, sort by z-position
				return lhs.transform->position.z < rhs.transform->position.z;
			});

		if (!mEditorMode) {
			Transform const& playerTransform{ gCoordinator->GetComponent<Transform>(mPlayer) };
			RigidBody const& playerRigidBody{ gCoordinator->GetComponent<RigidBody>(mPlayer) };
			glm::vec3 playerPosition{ playerTransform.position };
			Vec2 playerVel{ playerRigidBody.velocity };

			Camera& sceneCamera{ gCoordinator->GetComponent<Camera>(mSceneCamera) };
			sceneCamera.UpdatePosition(playerPosition, playerVel);
		}

		glm::mat4 viewProjMtx = mEditorMode ? ::gCoordinator->GetComponent<Camera>(mCamera).GetViewProjMtx() :
			::gCoordinator->GetComponent<Camera>(mSceneCamera).GetViewProjMtx();

		//auto const& camera = mEditorMode ? ::gCoordinator->GetComponent<OrthoCamera>(mCamera) : ::gCoordinator->GetComponent<Camera>(mSceneCamera);
		Renderer::RenderSceneBegin(viewProjMtx);
		for (auto const& entry : mRenderQueue)
		{
			if (entry.sprite->texture) {
				Renderer::DrawSprite(*entry.transform, entry.sprite->texture, entry.sprite->color, entry.entity);
			}
			else {
				if (entry.transform->elipse)
					Renderer::DrawCircle(entry.transform->position, entry.transform->scale, entry.sprite->color);
				else
					Renderer::DrawQuad(entry.transform->position, entry.transform->scale, entry.sprite->color, entry.transform->rotation.z, entry.entity);
			}
		}

		glDepthMask(GL_TRUE);
		if (mDebugMode) {
			::gCoordinator->GetSystem<Collision::CollisionSystem>()->Debug();
			NodeManager::DisplayDebugLines();
		}
		glDepthMask(GL_FALSE);

		Renderer::RenderSceneEnd();
		::gCoordinator->GetSystem<TextSystem>()->Update();

		fb->Unbind();
	}
}

/*  _________________________________________________________________________ */
/*!
\brief WindowSizeListener Function

Listens for window size changes and adjusts rendering parameters accordingly.

\param event The event data related to the window size change.
*/
void RenderSystem::WindowSizeListener(Event& event)
{
	[[maybe_unused]] auto windowWidth = event.GetParam<unsigned int>(Events::Window::Resized::WIDTH);
	[[maybe_unused]] auto windowHeight = event.GetParam<unsigned int>(Events::Window::Resized::HEIGHT);

	//auto& camera = gCoordinator->GetComponent<tCamera>(mCamera);
	//camera.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, windowWidth, windowHeight);
	//camera.projectionTransform = tCamera::MakeProjectionTransform(-50, 50, -50, 50, 0, -100);
}

