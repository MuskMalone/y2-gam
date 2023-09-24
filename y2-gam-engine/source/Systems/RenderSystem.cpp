#include "Systems/RenderSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Components/Camera.hpp"
#include "Core/Coordinator.hpp"
#include "Graphics/Shader.hpp"
#include <Graphics/GLSLShader.hpp>
#include <cmath>
#include "Core/Globals.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Renderer.hpp"
#include "Components/OrthoCamera.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/RigidBody.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

Entity RenderSystem::GetCamera() { return mCamera; }
std::shared_ptr<Framebuffer> const& RenderSystem::GetFramebuffer() const { return mFramebuffer; }

void RenderSystem::ToggleDebugMode() { mDebugMode = !mDebugMode; }

void RenderSystem::Init()
{
	gCoordinator = Coordinator::GetInstance();
	gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::RESIZED, RenderSystem::WindowSizeListener));

	mCamera = gCoordinator->CreateEntity();
	gCoordinator->AddComponent(
		mCamera,
		Transform{
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(),
			Vec3()
		});
	float aspectRatio{ static_cast<float>(ENGINE_SCREEN_WIDTH) / static_cast<float>(ENGINE_SCREEN_HEIGHT) };
	gCoordinator->AddComponent(
		mCamera,
		Camera{});
	gCoordinator->AddComponent(
		mCamera,
		OrthoCamera{aspectRatio, -WORLD_LIMIT_X * aspectRatio, WORLD_LIMIT_X * aspectRatio, -WORLD_LIMIT_Y, WORLD_LIMIT_Y}
	);

	std::shared_ptr<Texture> bgTex = std::make_shared<Texture>( "../Textures/blinkbg.png" );
	mBgSubtex = SubTexture::Create(bgTex, { 0, 0 }, { 3497, 1200 });
	Entity bg = gCoordinator->CreateEntity();
	::gCoordinator->AddComponent(
		bg,
		Transform{
			Vec3(0, 0, -40.f),
			Vec3(),
			Vec3(350.f, 120.f, 0.f)
		});
	/*
	::gCoordinator->AddComponent(
		bg,
		Sprite{
			Vec4{1.f,1.f,1.f,1.f},
			mBgSubtex,
			Layer::BACKGROUND
		}
	);
	*/
	Renderer::Init();

	//----------temp------------

	FramebufferProps fbProps;
	fbProps.width = ENGINE_SCREEN_WIDTH;
	fbProps.height = ENGINE_SCREEN_HEIGHT;
	mFramebuffer = Framebuffer::Create(fbProps);

	//--------------------------

}


void RenderSystem::Update(float dt)
{
	//mFramebuffer->Bind();

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

	auto const& camera = ::gCoordinator->GetComponent<OrthoCamera>(mCamera);
	Renderer::RenderSceneBegin(camera);
	for (auto const& entry : mRenderQueue)
	{

		if (entry.sprite->texture) {
			Renderer::DrawSprite(*entry.transform, entry.sprite->texture, entry.sprite->color);
		}
		else {
			Renderer::DrawQuad(entry.transform->position, entry.transform->scale, entry.sprite->color, entry.transform->rotation.z);
		}
	}
	if (mDebugMode) {
		::gCoordinator->GetSystem<Collision::CollisionSystem>()->Debug();
	}
	
	Renderer::RenderSceneEnd();
	//mFramebuffer->Unbind();
}

void RenderSystem::WindowSizeListener(Event& event)
{
	auto windowWidth = event.GetParam<unsigned int>(Events::Window::Resized::WIDTH);
	auto windowHeight = event.GetParam<unsigned int>(Events::Window::Resized::HEIGHT);

	//auto& camera = gCoordinator->GetComponent<tCamera>(mCamera);
	//camera.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, windowWidth, windowHeight);
	//camera.projectionTransform = tCamera::MakeProjectionTransform(-50, 50, -50, 50, 0, -100);
}

