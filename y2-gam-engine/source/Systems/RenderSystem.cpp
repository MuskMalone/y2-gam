#include "Systems/RenderSystem.hpp"

#include "Components/Camera.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Core/Coordinator.hpp"
#include "Graphics/Shader.hpp"
#include <Graphics/GLSLShader.hpp>
#include <cmath>
#include "Core/Globals.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Renderer.hpp"
#include "Graphics/OrthoCamera.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

Entity RenderSystem::GetCamera() { return mCamera; }

void RenderSystem::Init()
{
	gCoordinator = Coordinator::GetInstance();
	gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::RESIZED, RenderSystem::WindowSizeListener));

	shader = std::make_unique<Shader>("../Shaders/vertex.glsl", "../Shaders/fragment.glsl");

	mCamera = gCoordinator->CreateEntity();
	gCoordinator->AddComponent(
		mCamera,
		Transform{
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(),
			Vec3()
		});
	//float aspectRatio{ static_cast<float>(ENGINE_SCREEN_WIDTH) / static_cast<float>(ENGINE_SCREEN_HEIGHT) };

	gCoordinator->AddComponent(
		mCamera,
		Camera{});

	Renderer::Init();

}


void RenderSystem::Update(float dt)
{
	Renderer::ClearColor();
	auto& cameraTransform = gCoordinator->GetComponent<Transform>(mCamera);
	auto& camera = gCoordinator->GetComponent<Camera>(mCamera);

	OrthoCamera cam{ -WORLD_LIMIT_X, WORLD_LIMIT_X, -WORLD_LIMIT_Y, WORLD_LIMIT_Y };
	Renderer::RenderSceneBegin(cam);


	for (auto const& entity : mEntities)
	{
		auto const& transform = gCoordinator->GetComponent<Transform>(entity);
		auto const& renderable = gCoordinator->GetComponent<Sprite>(entity);

		if (renderable.texture) {
			Renderer::DrawQuad(transform.position, transform.scale, renderable.texture, transform.rotation.z);
		}
		else {
			Renderer::DrawQuad(transform.position, transform.scale, renderable.color, transform.rotation.z);
		}


	}


	Renderer::RenderSceneEnd();

}

void RenderSystem::WindowSizeListener(Event& event)
{
	auto windowWidth = event.GetParam<unsigned int>(Events::Window::Resized::WIDTH);
	auto windowHeight = event.GetParam<unsigned int>(Events::Window::Resized::HEIGHT);

	//auto& camera = gCoordinator->GetComponent<tCamera>(mCamera);
	//camera.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, windowWidth, windowHeight);
	//camera.projectionTransform = tCamera::MakeProjectionTransform(-50, 50, -50, 50, 0, -100);
}

