#include "Systems/EditorControlSystem.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Components/Camera.hpp"
#include "Components/Editor.hpp"
#include "Components/Transform.hpp"
#include "Components/Rigidbody.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Gravity.hpp"
#include "Components/Animation.hpp"
#include "Core/Coordinator.hpp"
#include <Systems/InputSystem.hpp>
#include "Systems/RenderSystem.hpp"
#include <Core/Globals.hpp>
#include <random>
#include <chrono>
#include "Components/OrthoCamera.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
namespace Testing {
	std::default_random_engine generator;
}

void EditorControlSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();

	// TO DELETE
	using namespace std::chrono;
	Testing::generator.seed(static_cast<unsigned int>(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	).count()));

	Entity entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	Vec3 position = Vec3(0, -WORLD_LIMIT_Y,1);
	::gCoordinator->AddComponent(
		entity,
		BoxCollider{
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(2 * WORLD_LIMIT_X, 5.f)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			.position = Vec3(position),
			.rotation = Vec3(),
			.scale = Vec3(2 * WORLD_LIMIT_X, 5.f, 1.f)
		});

	::gCoordinator->AddComponent(
		entity,
		Sprite{
			.color = Vec4(0,0,0, 1),
			.texture = nullptr
		});

	entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(-WORLD_LIMIT_X, 0, 1);
	::gCoordinator->AddComponent(
		entity,
		BoxCollider{
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(5.f, 1.9 * WORLD_LIMIT_Y)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			.position = Vec3(position),
			.rotation = Vec3(),
			.scale = Vec3(5.f, 2 * WORLD_LIMIT_Y, 1.f)
		});

	::gCoordinator->AddComponent(
		entity,
		Sprite{
			.color = Vec4(1,1,1,1),
			.texture = nullptr
		});

	entity = ::gCoordinator->CreateEntity();
	::gCoordinator->AddComponent<Gravity>(
		entity,
		{ Vec2(0.0f, -10.f) });
	position = Vec3(WORLD_LIMIT_X, 0, 1);
	::gCoordinator->AddComponent(
		entity,
		BoxCollider{
		});
	::gCoordinator->AddComponent(
		entity,
		RigidBody{
			Vec2(position), .0f, FLOAT_MAX, Vec2(5.f, 1.9 * WORLD_LIMIT_Y)
		});
	::gCoordinator->AddComponent(
		entity,
		Transform{
			.position = Vec3(position),
			.rotation = Vec3(),
			.scale = Vec3(5.f, 2 * WORLD_LIMIT_Y, 1.f)
		});

	::gCoordinator->AddComponent(
		entity,
		Sprite{
			.color = Vec4(1,1,1,1),
			.texture = nullptr
		});
}

void EditorControlSystem::Update(float dt)
{
	//TODO REMOVE TEMP
	float moveSpeed = 100.f;
	float rotSpeed = 80.f;

	auto& camera = ::gCoordinator->GetComponent<OrthoCamera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera());
	auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_W)){
		camera.mPos.y += moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_S)){
		camera.mPos.y -= moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_A)){
		camera.mPos.x -= moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_D)){
		camera.mPos.x += moveSpeed * dt;
		camera.SetPosition(camera.mPos);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_Q)) {
		camera.mRot -= rotSpeed * dt;
		camera.SetRotation(camera.mRot);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_E)) {
		camera.mRot += rotSpeed * dt;
		camera.SetRotation(camera.mRot);
	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_X)) {
		::gCoordinator->GetSystem<RenderSystem>()->ToggleDebugMode();
	}

	if (inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB)) &&
		inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, static_cast<size_t>(GLFW_KEY_LEFT_CONTROL))) {
		//std::vector<Entity> entities(1);
		using namespace Testing;

		std::uniform_real_distribution<float> randPositionY(0.f, 100.f);
		std::uniform_real_distribution<float> randPosition(-WORLD_LIMIT_X, WORLD_LIMIT_X);

		std::uniform_real_distribution<float> randDepth(-1.0f, 0.0f);
		//std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
		std::uniform_real_distribution<float> randScale(2.f, 5.f);
		std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
		std::uniform_real_distribution<float> randGravity(-100.f, -50.f);
		std::uniform_real_distribution<float> randVelocity(-10.f, 10.f);

		for (int i{}; i < 500; ++i) {
			float scale = randScale(generator);
			Entity entity = ::gCoordinator->CreateEntity();

			//for (auto& entity : entities)
			//{
			::gCoordinator->AddComponent<Gravity>(
				entity,
				//{Vec3(0.0f, randGravity(generator), 0.0f)});
				{ Vec2(0.0f, -100.f) });

			Vec3 position = Vec3(randPosition(generator), randPositionY(generator), randDepth(generator));
			::gCoordinator->AddComponent(
				entity,
				BoxCollider{
				});
			::gCoordinator->AddComponent(
				entity,
				RigidBody{
					Vec2(position),.0f, 10.f, Vec2(scale, scale), false
				});
			::gCoordinator->AddComponent(
				entity,
				Transform{
					.position = Vec3(position),
					.rotation = Vec3(),
					.scale = Vec3(scale, scale, scale)
				});
			::gCoordinator->AddComponent(
				entity,
				Sprite{
					.color = Vec4(randColor(generator), randColor(generator), randColor(generator), 1),
					.texture = nullptr
				});

		}


	}
	if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_P)) {



		using namespace Testing;
		std::uniform_real_distribution<float> randPositionY(0.f, 100.f);
		std::uniform_real_distribution<float> randPosition(-WORLD_LIMIT_X, WORLD_LIMIT_X);

		std::uniform_real_distribution<float> randDepth(-1.0f, 0.0f);
		//std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
		std::uniform_real_distribution<float> randColor(0.0f, 1.0f);
		std::uniform_real_distribution<float> randGravity(-100.f, -50.f);
		std::uniform_real_distribution<float> randVelocity(-10.f, 10.f);
		float scale = 50.f;
		for (int i{}; i < 500; ++i) {
			Entity entity = ::gCoordinator->CreateEntity();
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
				Sprite{
					.color = Vec4(randColor(generator), randColor(generator), randColor(generator), 1),
					.texture = nullptr
				});

			//------------TEMPORARY TO BE READ FROM JSON FILES------------------------------------------------------------------/
			std::vector<AnimationFrame> idleFrames{ {0.f, 0}, {0.f, 1}, { 0.f, 2 }, { 0.f, 3 }, { 0.f, 4 }, { 0.f, 5 }, { 0.f, 6 }, { 0.f, 7} };
			std::vector<AnimationFrame> runFrames{ {0.f, 8}, {0.f, 9}, { 0.f, 10 }, { 0.f, 11 }, { 0.f, 12 }, { 0.f, 13 }, { 0.f, 14 }, { 0.f, 15 } };
			std::vector<AnimationFrame> attackFrames{ {0.f, 16}, {0.f, 17}, { 0.f, 18 }, { 0.f, 19 }, { 0.f, 20 }, { 0.f, 21 }, { 0.f, 22 } };
			std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> map{ {ANIM_STATE::IDLE, idleFrames},
																			 {ANIM_STATE::RUN, runFrames},
																			 {ANIM_STATE::ATTACK, attackFrames} };
			::gCoordinator->AddComponent(
				entity,
				Animation{
					.speed = 0.08f,
					.currFrame = 0,
					.currState = ANIM_STATE::IDLE,
					.stateMap = map
				});
		}
	}

}
