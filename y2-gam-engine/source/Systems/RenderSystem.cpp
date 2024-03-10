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
#include "Systems/ParticleSystem.hpp"
#include "Systems/LayeringSystem.hpp"
#include "Core/Coordinator.hpp"
#include "Graphics/Shader.hpp"
#include "Core/Globals.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/FontRenderer.hpp"
#include "Systems/TextSystem.hpp"
#include "Graphics/SpriteManager.hpp"
#include "Components/UIImage.hpp"

#include "../WindowManager.hpp"
#include "Scripting/NodeManager.hpp"
#include "Scripting/ScriptManager.hpp"
#include <Engine/AssetManager.hpp>
#include <Systems/InputSystem.hpp>
#include <Core/FrameRateController.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Engine/SceneManager.hpp>


// Static Initialization
std::vector<std::pair<std::pair<Vec2, Vec2>, glm::vec4>> RenderSystem::mRays;

namespace TimeSlowEffect{
	const float growthRate{ 3.5f };
	const float maxRadius{ 3.f };
	const float duration {0.5f };
	float progress{};
}

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*!
\brief GetCamera Function

Retrieves the camera entity used in the rendering context.

\return The camera entity.
*/
Entity RenderSystem::GetCamera() const { if (mEditorMode)return mCamera; else return mSceneCamera; }

/*  _________________________________________________________________________ */
/*!
\brief GetUICamera Function

Retrieves the UI camera entity used in the rendering context.

\return The UI camera entity.
*/
Entity RenderSystem::GetUICamera() const { return mUICamera; }

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
bool RenderSystem::IsEditorMode() const {
	return mEditorMode == true;
}
/*  _________________________________________________________________________ */
/*!
\brief Init Function

Initializes the rendering system, setting up necessary resources.
*/
void RenderSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();
	::gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::RESIZED, RenderSystem::WindowSizeListener));

	//array to track the existence of different camera types
	bool cameraExists[static_cast<int>(CameraType::NumCameraTypes)] = { false };

	//iterate over entities to check for existing cameras
	for (auto const& entity : mEntities) {
		if (::gCoordinator->HasComponent<Camera>(entity)) {
			Camera& cameraComp = ::gCoordinator->GetComponent<Camera>(entity);
			cameraExists[static_cast<int>(cameraComp.type)] = true;

			//store the entity ID based on camera type
			switch (cameraComp.type) {
			case CameraType::MainCamera: mCamera = entity; break;
			case CameraType::SceneCamera: mSceneCamera = entity; break;
			case CameraType::PrefabEditorCamera: mPrefabEditorCamera = entity; break;
			case CameraType::UICamera: mUICamera = entity; break;
			}
		}
	}

	float aspectRatio{ static_cast<float>(ENGINE_SCREEN_WIDTH) / static_cast<float>(ENGINE_SCREEN_HEIGHT) };

	if (!cameraExists[static_cast<int>(CameraType::MainCamera)]) {
		mCamera = ::gCoordinator->CreateEntity();
		::gCoordinator->AddComponent(
			mCamera,
			Camera{ aspectRatio, static_cast<float>(-WORLD_LIMIT_Y) * aspectRatio, static_cast<float>(WORLD_LIMIT_Y) * aspectRatio, static_cast<float>(-WORLD_LIMIT_Y), static_cast<float>(WORLD_LIMIT_Y) }
		);
	}

	if (!cameraExists[static_cast<int>(CameraType::SceneCamera)]) {
		mSceneCamera = ::gCoordinator->CreateEntity();
		float zoomFactor{ 0.5f };
		::gCoordinator->AddComponent(
			mSceneCamera,
			Camera{ aspectRatio, static_cast<float>(-WORLD_LIMIT_Y) * aspectRatio * zoomFactor, static_cast<float>(WORLD_LIMIT_Y) * aspectRatio * zoomFactor, static_cast<float>(-WORLD_LIMIT_Y) * zoomFactor, static_cast<float>(WORLD_LIMIT_Y) * zoomFactor }
		);
	}

	if (!cameraExists[static_cast<int>(CameraType::PrefabEditorCamera)]) {
		mPrefabEditorCamera = ::gCoordinator->CreateEntity();
		const float pfHeight = 50;
		::gCoordinator->AddComponent(
			mPrefabEditorCamera,
			Camera{ aspectRatio, -pfHeight * aspectRatio, pfHeight * aspectRatio, -pfHeight, pfHeight }
		);
	}

	if (!cameraExists[static_cast<int>(CameraType::UICamera)]) {
		mUICamera = ::gCoordinator->CreateEntity();
		::gCoordinator->AddComponent(
			mUICamera,
			Camera{ aspectRatio, 0.f, ENGINE_SCREEN_WIDTH, 0.f, ENGINE_SCREEN_HEIGHT }
		);
	}


	Renderer::Init();

#ifndef _INSTALLER
	mEditorMode = true;
#else
	mEditorMode = false;
#endif

	FramebufferProps fbProps;
	fbProps.attachments = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::DEPTH };
	fbProps.width = ENGINE_SCREEN_WIDTH;
	fbProps.height = ENGINE_SCREEN_HEIGHT;
	mFramebuffers.push_back(Framebuffer::Create(fbProps));

	fbProps.attachments = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::DEPTH };
	mFramebuffers.push_back(Framebuffer::Create(fbProps));

	//postprocessing
	fbProps.attachments = { FramebufferTexFormat::RGBA8, FramebufferTexFormat::RED_INTEGER, FramebufferTexFormat::DEPTH };
	mFramebuffers.push_back(Framebuffer::Create(fbProps));

}

/*  _________________________________________________________________________ */
/*!
\brief Update Function

Updates the rendering system based on the given delta time.

\param dt The time elapsed since the last frame.
*/
void RenderSystem::Update([[maybe_unused]] float dt)
{
#ifndef _INSTALLER
	static bool showEditor{ true };
	auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_K)) {
		showEditor = !showEditor;
	}
#else 
	static bool showEditor{ false };
#endif

	mFramebuffers[0]->ClearAttachmentInt(1, -1);

	//if (showEditor) {
	mFramebuffers[0]->Bind();
	//}
	Renderer::SetClearColor({ 0.1f, 0.1f, 0.2f, 1.f });

	Renderer::ClearColor();
	Renderer::ClearDepth();

	mRenderQueue.clear();

	for (auto const& entity : mEntities) {
		if (::gCoordinator->HasComponent<UIImage>(entity)) continue;
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

	bool playerFound{ false };
	bool cameraFound{ false };
	Entity camSettings{};

	glm::vec4 playerScreenPos{};
	if (!mEditorMode) {
		for (auto const& e : mEntities) {
			if (::gCoordinator->HasComponent<Camera>(e) && ::gCoordinator->HasComponent<Tag>(e)) {
				camSettings = e;
				cameraFound = true;

			}

			if (!::gCoordinator->HasComponent<Tag>(e)) continue;
			auto const& tag = ::gCoordinator->GetComponent<Tag>(e);
			if (tag.tag == "Player") {
				mPlayer = e;
				playerFound = true;
			}

		}

		if (playerFound) {

			Transform const& playerTransform{ ::gCoordinator->GetComponent<Transform>(mPlayer) };

			//Script const& playerScript{ ::gCoordinator->GetComponent<Script>(mPlayer) };
			glm::vec3 playerPosition{ playerTransform.position };

			Camera& sceneCamera{ ::gCoordinator->GetComponent<Camera>(mSceneCamera) };
			sceneCamera.mTargetEntity = mPlayer;

			// Get Player Script Instance
			std::map<Entity, ScriptInstance> instanceMap{ ScriptManager::GetEntityInstances() };
			bool facingRight{ instanceMap[mPlayer].GetFieldValueFromName<bool>("IsFacingRight") };

			if (cameraFound) {

				Camera& cam{ ::gCoordinator->GetComponent<Camera>(camSettings) };

				sceneCamera.horizontalBoundary = cam.horizontalBoundary;
				sceneCamera.verticalBoundary = cam.verticalBoundary;
				sceneCamera.mZoomLevel = cam.mZoomLevel;
			}

			sceneCamera.UpdatePosition(playerPosition, facingRight);

			playerScreenPos = sceneCamera.GetViewProjMtx() * glm::vec4(playerPosition, 1.f);
		}

	}

	glm::mat4 viewProjMtx = mEditorMode ? ::gCoordinator->GetComponent<Camera>(mCamera).GetViewProjMtx() :
		::gCoordinator->GetComponent<Camera>(mSceneCamera).GetViewProjMtx();

	glDisable(GL_DEPTH_TEST);

	Renderer::RenderSceneBegin(viewProjMtx);
	for (auto const& entry : mRenderQueue)
	{
		// Added this flag for toggle visibility
		if (::gCoordinator->HasComponent<Layering>(entry.entity)) {
			if (!LayeringSystem::IsLayerVisible(::gCoordinator->GetComponent<Layering>(entry.entity).assignedLayer)) continue;
		}

		if (entry.sprite->GetSpriteID()) {
			Renderer::DrawSprite(*entry.transform, SpriteManager::GetSprite(entry.sprite->GetSpriteID()), entry.sprite->color, entry.entity);
		}
		else {
			if (entry.transform->elipse)
				Renderer::DrawCircle(entry.transform->position, entry.transform->scale, entry.sprite->color);
			else
				Renderer::DrawQuad(entry.transform->position, entry.transform->scale, entry.sprite->color, entry.transform->rotation.z, entry.entity);
		}
	}
	::gCoordinator->GetSystem<ParticleSystem>()->DrawDebug();
	if (mDebugMode) {
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Debug();
		NodeManager::DisplayDebugLines();

		for (auto const& ray : mRays) {
			glm::vec3 firstPos{ glm::vec3(ray.first.first.x, ray.first.first.y, 0.f) };
			glm::vec3 secondPos{ glm::vec3(ray.first.second.x, ray.first.second.y, 0.f) };
			Renderer::DrawLine(firstPos, secondPos, ray.second);
		}
		mRays.clear();
	}

	Renderer::RenderSceneEnd();
	::gCoordinator->GetSystem<ParticleSystem>()->Draw();

	if (showEditor)
		RenderUI();

	glEnable(GL_DEPTH_TEST);
	::gCoordinator->GetSystem<TextSystem>()->Update();


	//if (showEditor) {
	mFramebuffers[0]->Unbind();
	//}
	if (showEditor) {
		mFramebuffers[2]->Bind();
	}
	else {
		int width, height;
		glfwGetFramebufferSize(WindowManager::GetInstance()->GetContext(), &width, &height);
		glViewport(0, 0, width, height);
	}
	glDisable(GL_DEPTH_TEST);

	playerScreenPos /= playerScreenPos.w;
	glm::vec2 playerCenter = glm::vec2(playerScreenPos.x,playerScreenPos.y);

	UpdateRadius(dt);

	float time = static_cast<float>(glfwGetTime());
	std::vector<UniformData> uniforms;
	uniforms.push_back(UniformData{ "time", UniformData::Type::FLOAT , time });
	int width, height;
	glfwGetFramebufferSize(WindowManager::GetInstance()->GetContext(), &width, &height);
	glm::vec2 res {width, height};
	uniforms.push_back(UniformData{ "resolution", UniformData::Type::VEC2, res });
	uniforms.push_back(UniformData{ "radius", UniformData::Type::FLOAT, mRadius });
	uniforms.push_back(UniformData{ "circleCenter", UniformData::Type::VEC2, playerCenter});

	Renderer::ApplyPostProcessing(mFramebuffers[0]->GetColorAttachmentID(), uniforms);

	//glEnable(GL_DEPTH_TEST);
	if (showEditor) {
		mFramebuffers[2]->Unbind();
	}
	else {
		RenderUI();
	}
	glEnable(GL_DEPTH_TEST);

}

 /*  _________________________________________________________________________ */
 /*!
	 \brief RenderPrefab

	The function binds a specific framebuffer, sets clear color, and if the prefab has transform and sprite components, it processes animations and draws the sprite.
	Additionally, if a collider component is present, it draws the collider shape. The function also handles custom drawing for different sprite states and collider types.

	 \param prefab entity to be rendered
 */

void RenderSystem::RenderPrefab(Entity prefab) {
	//tch: hack to check if its a valid entity for drawing
	//if (!gCoordinator->HasComponent<Sprite>(prefab) || !gCoordinator->HasComponent<Transform>(prefab)) return;
	//Prefab Editor
	mFramebuffers[1]->Bind();

	Renderer::SetClearColor({ 0.1f, 0.1f, 0.3f, 1.f });
	Renderer::ClearColor();
	Renderer::ClearDepth();
	Renderer::RenderSceneBegin(::gCoordinator->GetComponent<Camera>(mPrefabEditorCamera).GetViewProjMtx());


	if (gCoordinator->HasComponent<Transform>(prefab)) {
		const auto& transform = gCoordinator->GetComponent<Transform>(prefab);
		decltype(transform.position) drawnPos{0, 0, 0};
		if (gCoordinator->HasComponent<Sprite>(prefab)) {
			auto& sprite = gCoordinator->GetComponent<Sprite>(prefab);

			if (gCoordinator->HasComponent<Animation>(prefab)) {
				auto& animation = gCoordinator->GetComponent<Animation>(prefab);
				if (!animation.states.empty()) {
					float dt = FrameRateController::GetInstance()->GetDeltaTime();
					if (animation.currState >= animation.states.size()) animation.currState = animation.states.size() - 1;
					size_t& frameIdx{ animation.currFrame };
					//if (!(animation.stateMap[currState]) || animation.stateMap[currState] == static_cast<AssetID>(-1)) continue;
					//quick patch to constcast this
					std::vector<AnimationFrame>& frameList{ const_cast<std::vector<AnimationFrame>&>(AssetManager::GetInstance()->GetAsset<AnimationManager>(animation.states[animation.currState])) };

					if (frameIdx >= frameList.size())
						frameIdx = 0;

					AnimationFrame& currFrame{ frameList[frameIdx] };

					//xavier todo: help me change this to not use elapsed time
					currFrame.elapsedTime += dt;

					sprite.spriteID = currFrame.spriteID;

					if (currFrame.elapsedTime >= animation.speed) {
						++frameIdx;
						currFrame.elapsedTime = 0.f;
					}
				}
			}
			if (sprite.GetSpriteID())
				Renderer::DrawSprite({}, transform.scale, SpriteManager::GetSprite(sprite.GetSpriteID()), sprite.color, transform.rotation.z, prefab);
			else {
				if (transform.elipse)
					Renderer::DrawCircle(drawnPos, transform.scale, sprite.color);
				else
					Renderer::DrawQuad(drawnPos, transform.scale, sprite.color, transform.rotation.z, prefab);
			}


		}
		if (gCoordinator->HasComponent<Collider>(prefab)) {
			const auto& c = gCoordinator->GetComponent<Collider>(prefab);
			decltype(transform.position) colliderPos{ c.position.x, c.position.y, 1.f };
			auto relativePos{ colliderPos - transform.position };
			if (c.type == ColliderType::BOX) {
				Renderer::DrawLineRect(relativePos, { c.dimension.x,c.dimension.y }, { 1.f, 1.f, 1.f ,1.f }, Image::Degree(c.rotation));
			}
			else {
				Renderer::DrawCircle(relativePos, { c.dimension.x, c.dimension.x }, { 1.f,1.f,1.f, 1.f }, .1f);
			}
		}
	}

	Renderer::RenderSceneEnd();
	mFramebuffers[1]->Unbind();
}

/*  _________________________________________________________________________ */
/*!
\brief RenderUI

Renders UI elements by collecting entities with UIImage and Transform components, sorting them, and rendering each UI component.

\param event The event data that includes the positions the raycast were fired.
*/
void RenderSystem::RenderUI() {
	Renderer::RenderSceneBegin(::gCoordinator->GetComponent<Camera>(mUICamera).GetViewProjMtx());

	// Create a vector to store entities with UIImage components
	std::vector<Entity> uiEntities;

	// Populate the vector with entities that have UIImage and Transform components
	for (auto const& entity : mEntities) {
		if (::gCoordinator->HasComponent<UIImage>(entity) && ::gCoordinator->HasComponent<Transform>(entity)) {
			uiEntities.push_back(entity);
		}
	}

	// Sort the entities based on the z value of their Transform component
	std::sort(uiEntities.begin(), uiEntities.end(), [](const Entity& a, const Entity& b) {
		auto& transformA = ::gCoordinator->GetComponent<Transform>(a);
		auto& transformB = ::gCoordinator->GetComponent<Transform>(b);
		return transformA.position.z < transformB.position.z; // Sort in ascending order
		});

	// Render sorted UI elements
	for (auto const& entity : uiEntities) {

		if (::gCoordinator->HasComponent<Layering>(entity)) {
			if (!LayeringSystem::IsLayerVisible(::gCoordinator->GetComponent<Layering>(entity).assignedLayer)) continue;
		}

		auto const& ui = ::gCoordinator->GetComponent<UIImage>(entity);
		auto& sprite = ::gCoordinator->GetComponent<Sprite>(entity);
		auto& transform = ::gCoordinator->GetComponent<Transform>(entity);

		if (ui.enabled) {
			// Constrain position within screen bounds
			/*
			float minX = 0 + transform.scale.x / 2.0f; // Left boundary
			float maxX = ENGINE_SCREEN_WIDTH - transform.scale.x / 2.0f; // Right boundary
			float minY = 0 + transform.scale.y / 2.0f; // Bottom boundary
			float maxY = ENGINE_SCREEN_HEIGHT - transform.scale.y / 2.0f; // Top boundary

			transform.position.x = std::max(minX, std::min(transform.position.x, maxX));
			transform.position.y = std::max(minY, std::min(transform.position.y, maxY));
			*/
			if (!::gCoordinator->HasComponent<Text>(entity)) {
				if (sprite.GetSpriteID())
					Renderer::DrawSprite(transform, SpriteManager::GetSprite(sprite.GetSpriteID()), sprite.color, entity);
				else {
					if (transform.elipse)
						Renderer::DrawCircle(transform.position, transform.scale, sprite.color);
					else
						Renderer::DrawQuad(transform.position, transform.scale, sprite.color, transform.rotation.z, entity);
				}
			}

		}
	}

	Renderer::RenderSceneEnd();

	// For text with UI component
	for (auto const& entity : uiEntities) {
		auto& transform = ::gCoordinator->GetComponent<Transform>(entity);
		/*
		// Constrain position within screen bounds
		float minX = 0 + transform.scale.x / 2.0f; // Left boundary
		float maxX = ENGINE_SCREEN_WIDTH - transform.scale.x / 2.0f; // Right boundary
		float minY = 0 + transform.scale.y / 2.0f; // Bottom boundary
		float maxY = ENGINE_SCREEN_HEIGHT - transform.scale.y / 2.0f; // Top boundary

		transform.position.x = std::max(minX, std::min(transform.position.x, maxX));
		transform.position.y = std::max(minY, std::min(transform.position.y, maxY));
		*/
		if (::gCoordinator->HasComponent<Layering>(entity)) {
			if (!LayeringSystem::IsLayerVisible(::gCoordinator->GetComponent<Layering>(entity).assignedLayer)) continue;
		}

		if (::gCoordinator->HasComponent<Text>(entity)) {
			auto const& textToPrint{ Coordinator::GetInstance()->GetComponent<Text>(entity) };
			float lengthOfText{ Image::FontRenderer::GetTextWidth(textToPrint.fontName, textToPrint.text, textToPrint.scale) };
			Image::FontRenderer::RenderTextUI(textToPrint.fontName, textToPrint.text, transform.position.x - (lengthOfText / 2.f),
				transform.position.y, textToPrint.scale, textToPrint.color);
		}
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

	Renderer::SetViewport(0, 0, windowWidth, windowHeight);

	auto& camera = ::gCoordinator->GetComponent<Camera>(mCamera);

	camera.mAspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
	camera.UpdateProjectionMtx();
	//camera.SetProjectionMtx(left, right, bottom, top);
}

/*  _________________________________________________________________________ */
/*!
\brief DebugRay Function

Listens for the raycast event.

\param event The event data that includes the positions the raycast were fired.
*/
void RenderSystem::DebugRay(Event& event) {
	[[maybe_unused]] auto pos = event.GetParam<std::pair<std::pair<Vec2, Vec2>, 
		glm::vec4>>(Events::Physics::Raycast::Debug::RAYCAST_DEBUGGED);

	mRays.push_back(pos);
}

void RenderSystem::CheckAssetValidity()
{
	bool changed{ false };
	for (auto const& e : mEntities) {
		auto& sprite = gCoordinator->GetComponent<Sprite>(e);
		//this check is to remove any invalid asset ids
		if (!AssetManager::GetInstance()->IsAssetExist(sprite.spriteAssetID) && sprite.spriteAssetID != 0) {
			sprite.spriteAssetID = 0;
			
			//set the anim spriteid just to be safe
			sprite.spriteID = 0;
			changed = true;
		}
	}
	if (changed) {
		//std::cout << "changed sprite\n";
		SceneManager::GetInstance()->SaveScene();
	}
}

/*  _________________________________________________________________________ */
/*!
\brief SetSceneCameraZoom Function

Set zoom for scene camera

\param zoom amount to set the zoom
*/
void RenderSystem::SetSceneCameraZoom(float zoom) {
	//Entity camSettings{};
	for (auto const& e : mEntities) {
		if (::gCoordinator->HasComponent<Camera>(e) && ::gCoordinator->HasComponent<Tag>(e)) {

			auto& cam = gCoordinator->GetComponent<Camera>(e);
			zoom = std::max(cam.mMinZoom, std::min(zoom, cam.mMaxZoom));
			cam.UpdateZoom(zoom);
			break;
		}

	}
}

void RenderSystem::UpdateRadius(float dt) {
	float changeRate = TimeSlowEffect::maxRadius / TimeSlowEffect::duration;

	if (mIsTimeSlow) {
		// Linearly increase the radius until it reaches maxRadius
		mRadius += changeRate * dt;
		mRadius = std::min(mRadius, TimeSlowEffect::maxRadius);
	}
	else {
		// Linearly decrease the radius until it reaches 0
		mRadius -= changeRate * dt;
		mRadius = std::max(mRadius, 0.0f);
	}
}