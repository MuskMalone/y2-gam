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
#include "Components/BoxCollider.hpp"
#include "Components/RigidBody.hpp"

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
			.position = Vec3(0.0f, 0.0f, 0.0f),
			.rotation = Vec3(),
			.scale = Vec3()
		});
	//float aspectRatio{ static_cast<float>(ENGINE_SCREEN_WIDTH) / static_cast<float>(ENGINE_SCREEN_HEIGHT) };

	gCoordinator->AddComponent(
		mCamera,
		Camera{});

	Renderer::Init();

}


void RenderSystem::Update(float dt)
{
	//struct RenderEntry {
	//	Entity entity;
	//	Transform* transform;
	//	Sprite* sprite;
	//};

	//std::vector<RenderEntry> renderQueue;
	//for (auto const& entity : mEntities) {
	//	RenderEntry entry{
	//		.entity = entity,
	//		.transform = &gCoordinator->GetComponent<Transform>(entity),
	//		.sprite = &gCoordinator->GetComponent<Sprite>(entity)
	//	};
	//	renderQueue.push_back(entry);
	//}

	//std::sort(renderQueue.begin(), renderQueue.end(),
	//	[](RenderEntry const& rhs, RenderEntry const& lhs) {
	//		return rhs.transform->position.z < lhs.transform->position.z;
	//	});

	//OrthoCamera cam{ -WORLD_LIMIT_X, WORLD_LIMIT_X, -WORLD_LIMIT_Y, WORLD_LIMIT_Y };
	//Renderer::RenderSceneBegin(cam);


	//for (auto const& entry : renderQueue)
	//{

	//	if (entry.sprite->texture) {
	//		Renderer::DrawQuad(entry.transform->position, entry.transform->scale, entry.sprite->texture, entry.transform->rotation.z);
	//	}
	//	else {
	//		Renderer::DrawQuad(entry.transform->position, entry.transform->scale, entry.sprite->color, entry.transform->rotation.z);
	//	}

	//}

	//Renderer::RenderSceneEnd();

	//auto& cameraTransform = gCoordinator->GetComponent<Transform>(mCamera);
	//auto& camera = gCoordinator->GetComponent<Camera>(mCamera);

	////lambda function
	//auto EntityCompareZ = [&](Entity a, Entity b)-> bool {
	//	auto& transformA = gCoordinator->GetComponent<Transform>(a);
	//	auto& transformB = gCoordinator->GetComponent<Transform>(b);
	//	return transformA.position.z < transformB.position.z;
	//};

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

