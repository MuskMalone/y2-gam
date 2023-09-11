#include "Systems/EditorControlSystem.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Components/Camera.hpp"
#include "Components/Editor.hpp"
#include "Components/Thrust.hpp"
#include "Components/Transform.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/AABBCollider.hpp"
#include "Components/Renderable.hpp"
#include "Components/Gravity.hpp"
#include "Core/Coordinator.hpp"
#include <Systems/InputSystem.hpp>
#include "Systems/RenderSystem.hpp"
#include <Core/Globals.hpp>
#include <random>
#include <chrono>

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
namespace Testing {
	std::default_random_engine generator;
}

void EditorControlSystem::Init()
{
	::gCoordinator = Coordinator::GetCoordinator();

	// TO DELETE
	using namespace std::chrono;
	Testing::generator.seed(static_cast<unsigned int>(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	).count()));
}

void EditorControlSystem::Update(float dt)
{
	
	auto& camera = ::gCoordinator->GetComponent<tCamera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera());
	auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
	if (inputSystem->CheckKey(InputSystem::KeyState::KEY_PRESSED, GLFW_KEY_W)){
		camera.UpdatePos(camera.eye.x, camera.eye.y + dt);
	}

	else if (inputSystem->CheckKey(InputSystem::KeyState::KEY_PRESSED, GLFW_KEY_S)){
		camera.UpdatePos(camera.eye.x, camera.eye.y - dt);
	}

	if (inputSystem->CheckKey(InputSystem::KeyState::KEY_PRESSED, GLFW_KEY_A)){
		camera.UpdatePos(camera.eye.x - dt, camera.eye.y);
	}
	
	else if (inputSystem->CheckKey(InputSystem::KeyState::KEY_PRESSED, GLFW_KEY_D)){
		camera.UpdatePos(camera.eye.x + dt, camera.eye.y);
	}

	else if (inputSystem->CheckKey(InputSystem::KeyState::KEY_CLICKED, GLFW_KEY_Q)) {
		//std::vector<Entity> entities(1);
		using namespace Testing;

		std::uniform_real_distribution<float> randPositionY(0.f, 100.f);
		std::uniform_real_distribution<float> randPosition(-WORLD_LIMIT_X, WORLD_LIMIT_X);

		std::uniform_real_distribution<float> randDepth(-1.0f, 0.0f);
		//std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
		std::uniform_real_distribution<float> randScale(5.f, 10.f);
		std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
		std::uniform_real_distribution<float> randGravity(-10.f, -1.f);
		std::uniform_real_distribution<float> randVelocity(-10.f, 10.f);

		for (int i{}; i < 10; ++i) {
			float scale = randScale(generator);
			Entity entity = ::gCoordinator->CreateEntity();

			//for (auto& entity : entities)
			//{
			::gCoordinator->AddComponent<Gravity>(
				entity,
				//{Vec3(0.0f, randGravity(generator), 0.0f)});
				{ Vec2(0.0f, randGravity(generator)) });
			::gCoordinator->AddComponent(
				entity,
				RigidBody{
					.mass = 0.f,
					.velocity = Vec2(randVelocity(generator), randVelocity(generator)),
					.acceleration = Vec3(0.0f, 0.0f, 0.0f)
				});
			Vec3 position = Vec3(randPosition(generator), randPositionY(generator), randDepth(generator));
			::gCoordinator->AddComponent(
				entity,
				Transform{
					.position = Vec3(position),
					.rotation = Vec3(),
					.scale = Vec3(scale, scale, scale)
				});
			::gCoordinator->AddComponent(
				entity,
				AABBCollider{
					.dimension = Vec2(scale, scale),
					.position = Vec2(position)
				});
			::gCoordinator->AddComponent(
				entity,
				Renderable{
					.color = Vec3(randColor(generator), randColor(generator), randColor(generator)),
					.drawMode = GL_FILL
				});

		}
	}
	else if (inputSystem->CheckKey(InputSystem::KeyState::KEY_CLICKED, GLFW_KEY_E)) {
		//spawn platform
		Entity entity = ::gCoordinator->CreateEntity();
		using namespace Testing;

		std::uniform_real_distribution<float> randPosition(-WORLD_LIMIT_X, WORLD_LIMIT_X);
		std::uniform_real_distribution<float> randDepth(-1.0f, 0.0f);
		//std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
		std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
		//for (auto& entity : entities)
		//{
		::gCoordinator->AddComponent<Gravity>(
			entity,
			{ Vec2(0.0f, 0) });

		::gCoordinator->AddComponent(
			entity,
			RigidBody{
				.mass = 0.f,
				.velocity = Vec2(0.0f, 0.0f),
				.acceleration = Vec3(0.0f, 0.0f, 0.0f)
			});
		Vec3 position = Vec3(0, WORLD_LIMIT_Y * -.5f, randDepth(generator));
		::gCoordinator->AddComponent(
			entity,
			Transform{
				.position = Vec3(position),
				.rotation = Vec3(),
				.scale = Vec3(100.f, 5.f, 1.f)
			});
		::gCoordinator->AddComponent(
			entity,
			AABBCollider{
				.dimension = Vec2(100.f, 5.f),
				.position = Vec2(position)
			});
		::gCoordinator->AddComponent(
			entity,
			Renderable{
				.color = Vec3(randColor(generator), randColor(generator), randColor(generator)),
				.drawMode = GL_FILL
			});
		//}
	}
}
