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
#include "Systems/LayeringSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Graphics/Shader.hpp"
#include "Core/Globals.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/FontRenderer.hpp"
#include "Systems/TextSystem.hpp"
#include "Graphics/SpriteManager.hpp"

#include "Scripting/NodeManager.hpp"
#include <Engine/AssetManager.hpp>
#include <Systems/InputSystem.hpp>

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
		Camera{aspectRatio, static_cast<float>(-WORLD_LIMIT_Y) * aspectRatio, static_cast<float>(WORLD_LIMIT_Y) * aspectRatio, static_cast<float>(-WORLD_LIMIT_Y), static_cast<float>(WORLD_LIMIT_Y)}
	);

	float const zoomFactor{ 0.4f };
	gCoordinator->AddComponent(
		mSceneCamera,
		Camera{ aspectRatio, static_cast<float>(-WORLD_LIMIT_Y) * aspectRatio * zoomFactor, static_cast<float>(WORLD_LIMIT_Y) * aspectRatio * zoomFactor, static_cast<float>(-WORLD_LIMIT_Y) * zoomFactor, static_cast<float>(WORLD_LIMIT_Y) * zoomFactor }
	);

	//Create prefab editor camera
	const float pfHeight{50};

	mPrefabEditorCamera = gCoordinator->CreateEntity();
	gCoordinator->AddComponent(
		mPrefabEditorCamera,
		Camera{ aspectRatio, -pfHeight * aspectRatio, pfHeight * aspectRatio, -pfHeight, pfHeight }
	);
	auto& pbCam = gCoordinator->GetComponent<Camera>(mPrefabEditorCamera);
	
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
	static bool showEditor{ true };
	mFramebuffers[0]->ClearAttachmentInt(1, -1);
	auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_K)) {
		showEditor = !showEditor;
	}
	if (showEditor) {
		mFramebuffers[0]->Bind();
	}
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
		for (auto const& e : mEntities) {
			if (!gCoordinator->HasComponent<Tag>(e)) continue;
			auto const& tag = gCoordinator->GetComponent<Tag>(e);
			if (tag.tag == "Player") {
				mPlayer = e;
			}
		}

		Transform const& playerTransform{ gCoordinator->GetComponent<Transform>(mPlayer) };
		RigidBody const& playerRigidBody{ gCoordinator->GetComponent<RigidBody>(mPlayer) };
		glm::vec3 playerPosition{ playerTransform.position };
		Vec2 playerVel{ playerRigidBody.velocity };

		Camera& sceneCamera{ gCoordinator->GetComponent<Camera>(mSceneCamera) };
		sceneCamera.mTargetEntity = mPlayer;
		sceneCamera.UpdatePosition(playerPosition, playerVel);

	}

	glm::mat4 viewProjMtx = mEditorMode ? ::gCoordinator->GetComponent<Camera>(mCamera).GetViewProjMtx() :
		::gCoordinator->GetComponent<Camera>(mSceneCamera).GetViewProjMtx();
	glDisable (GL_DEPTH_TEST);

	//auto const& camera = mEditorMode ? ::gCoordinator->GetComponent<OrthoCamera>(mCamera) : ::gCoordinator->GetComponent<Camera>(mSceneCamera);
	Renderer::RenderSceneBegin(viewProjMtx);
	for (auto const& entry : mRenderQueue)
	{
		// Added this flag for toggle visibility
		if (!LayeringSystem::IsLayerVisible(gCoordinator->GetComponent<Layering>(entry.entity).assignedLayer)) continue;

		if (entry.sprite->GetSpriteID()) {
			//std::cout << entry.sprite->spriteID << std::endl;
			Renderer::DrawSprite(*entry.transform, SpriteManager::GetSprite(entry.sprite->GetSpriteID()), entry.sprite->color, entry.entity);
		}
		else {
			if (entry.transform->elipse)
				Renderer::DrawCircle(entry.transform->position, entry.transform->scale, entry.sprite->color);
			else
				Renderer::DrawQuad(entry.transform->position, entry.transform->scale, entry.sprite->color, entry.transform->rotation.z, entry.entity);
		}
	}

	if (mDebugMode) {
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Debug();
		NodeManager::DisplayDebugLines();
	}

	Renderer::RenderSceneEnd();
	glEnable(GL_DEPTH_TEST);

	::gCoordinator->GetSystem<TextSystem>()->Update();
	if (showEditor) {

		mFramebuffers[0]->Unbind();
	}
}

void RenderSystem::RenderPrefab(Entity prefab) {
	//tch: hack to check if its a valid entity for drawing
	//xavier todo: pls change this to a more ecs implementation in the future!!!
	if (!gCoordinator->HasComponent<Sprite>(prefab) || !gCoordinator->HasComponent<Transform>(prefab)) return;
	//Prefab Editor
	mFramebuffers[1]->Bind();

	Renderer::SetClearColor({ 0.1f, 0.1f, 0.3f, 1.f });
	Renderer::ClearColor();
	Renderer::ClearDepth();
	Renderer::RenderSceneBegin(::gCoordinator->GetComponent<Camera>(mPrefabEditorCamera).GetViewProjMtx());

	const auto& sprite = gCoordinator->GetComponent<Sprite>(prefab);
	const auto& transform = gCoordinator->GetComponent<Transform>(prefab);

	if(sprite.spriteID)
		Renderer::DrawSprite({}, transform.scale, SpriteManager::GetSprite(sprite.spriteID), sprite.color, transform.rotation.z, prefab);
	else {
		if (transform.elipse)
			Renderer::DrawCircle(transform.position, transform.scale, sprite.color);
		else
			Renderer::DrawQuad(transform.position, transform.scale, sprite.color, transform.rotation.z, prefab);
	}

	Renderer::RenderSceneEnd();
	mFramebuffers[1]->Unbind();
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

	Renderer::SetViewport(0, 0, windowWidth, windowHeight);

	//float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	//float left = -WORLD_LIMIT_X * aspectRatio;
	//float right = WORLD_LIMIT_X * aspectRatio;
	//float bottom = -WORLD_LIMIT_Y;
	//float top = WORLD_LIMIT_Y;

	auto& camera = gCoordinator->GetComponent<Camera>(mCamera);

	camera.mAspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	camera.UpdateProjectionMtx();
	//camera.SetProjectionMtx(left, right, bottom, top);
}


