#include "Systems/RenderSystem.hpp"

#include "Components/Camera.hpp"
#include "Components/Renderable.hpp"
#include "Components/Transform.hpp"
#include "Core/Coordinator.hpp"
#include "Graphics/Shader.hpp"
#include <Graphics/GLSLShader.hpp>
#include <cmath>
#include "Core/Globals.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Graphics/Renderer.hpp"
#include "Graphics/Camera.hpp"

 
namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

Entity RenderSystem::GetCamera() { return mCamera; }

void RenderSystem::Init()
{
	gCoordinator = Coordinator::GetCoordinator();
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
		tCamera{});

	Renderer::Init();

	////36 vertices form a cube
	//std::vector<Vec3> vertices =
	//	{
	//		Vec3(-0.5f, -0.5f, -0.5f),
	//		Vec3(0.5f, -0.5f, -0.5f),
	//		Vec3(0.5f, 0.5f, -0.5f),
	//		Vec3(0.5f, 0.5f, -0.5f),
	//		Vec3(-0.5f, 0.5f, -0.5f),
	//		Vec3(-0.5f, -0.5f, -0.5f),
	//		Vec3(-0.5f, -0.5f, 0.5),
	//		Vec3(0.5f, -0.5f, 0.5),
	//		Vec3(0.5f, 0.5f, 0.5),
	//		Vec3(0.5f, 0.5f, 0.5),
	//		Vec3(-0.5f, 0.5f, 0.5),
	//		Vec3(-0.5f, -0.5f, 0.5),
	//		Vec3(-0.5f, 0.5f, 0.5f),
	//		Vec3(-0.5f, 0.5f, -0.5f),
	//		Vec3(-0.5f, -0.5f, -0.5f),
	//		Vec3(-0.5f, -0.5f, -0.5f),
	//		Vec3(-0.5f, -0.5f, 0.5f),
	//		Vec3(-0.5f, 0.5f, 0.5f),
	//		Vec3(0.5f, 0.5f, 0.5),
	//		Vec3(0.5f, 0.5f, -0.5),
	//		Vec3(0.5f, -0.5f, -0.5),
	//		Vec3(0.5f, -0.5f, -0.5),
	//		Vec3(0.5f, -0.5f, 0.5),
	//		Vec3(0.5f, 0.5f, 0.5),
	//		Vec3(-0.5f, -0.5f, -0.5f),
	//		Vec3(0.5f, -0.5f, -0.5f),
	//		Vec3(0.5f, -0.5f, 0.5f),
	//		Vec3(0.5f, -0.5f, 0.5f),
	//		Vec3(-0.5f, -0.5f, 0.5f),
	//		Vec3(-0.5f, -0.5f, -0.5f),
	//		Vec3(-0.5f, 0.5f, -0.5),
	//		Vec3(0.5f, 0.5f, -0.5),
	//		Vec3(0.5f, 0.5f, 0.5),
	//		Vec3(0.5f, 0.5f, 0.5),
	//		Vec3(-0.5f, 0.5f, 0.5),
	//		Vec3(-0.5f, 0.5f, -0.5)
	//	};

	//std::vector<Vec3> normals =
	//	{
	//		Vec3(0.0f, 0.0f, -1.0f),
	//		Vec3(0.0f, 0.0f, -1.0f),
	//		Vec3(0.0f, 0.0f, -1.0f),
	//		Vec3(0.0f, 0.0f, -1.0f),
	//		Vec3(0.0f, 0.0f, -1.0f),
	//		Vec3(0.0f, 0.0f, -1.0f),
	//		Vec3(0.0f, 0.0f, 1.0f),
	//		Vec3(0.0f, 0.0f, 1.0f),
	//		Vec3(0.0f, 0.0f, 1.0f),
	//		Vec3(0.0f, 0.0f, 1.0f),
	//		Vec3(0.0f, 0.0f, 1.0f),
	//		Vec3(0.0f, 0.0f, 1.0f),
	//		Vec3(-1.0f, 0.0f, 0.0f),
	//		Vec3(-1.0f, 0.0f, 0.0f),
	//		Vec3(-1.0f, 0.0f, 0.0f),
	//		Vec3(-1.0f, 0.0f, 0.0f),
	//		Vec3(-1.0f, 0.0f, 0.0f),
	//		Vec3(-1.0f, 0.0f, 0.0f),
	//		Vec3(1.0f, 0.0f, 0.0f),
	//		Vec3(1.0f, 0.0f, 0.0f),
	//		Vec3(1.0f, 0.0f, 0.0f),
	//		Vec3(1.0f, 0.0f, 0.0f),
	//		Vec3(1.0f, 0.0f, 0.0f),
	//		Vec3(1.0f, 0.0f, 0.0f),
	//		Vec3(0.0f, -1.0f, 0.0f),
	//		Vec3(0.0f, -1.0f, 0.0f),
	//		Vec3(0.0f, -1.0f, 0.0f),
	//		Vec3(0.0f, -1.0f, 0.0f),
	//		Vec3(0.0f, -1.0f, 0.0f),
	//		Vec3(0.0f, -1.0f, 0.0f),
	//		Vec3(0.0f, 1.0f, 0.0f),
	//		Vec3(0.0f, 1.0f, 0.0f),
	//		Vec3(0.0f, 1.0f, 0.0f),
	//		Vec3(0.0f, 1.0f, 0.0f),
	//		Vec3(0.0f, 1.0f, 0.0f),
	//		Vec3(0.0f, 1.0f, 0.0)
	//	};


	//glGenVertexArrays(1, &mVao);
	//glBindVertexArray(mVao);

	//// Vertices
	//glGenBuffers(1, &mVboVertices);
	//glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)nullptr);
	//glEnableVertexAttribArray(0);

	//// Surface normal
	//glGenBuffers(1, &mVboNormals);
	//glBindBuffer(GL_ARRAY_BUFFER, mVboNormals);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (void*)nullptr);
	//glEnableVertexAttribArray(1);

	//glBindVertexArray(0);
}


void RenderSystem::Update(float dt)
{

	//shader->Use();
	//
	
	//BindVertexArray(mVao);
	//sData.

	auto& cameraTransform = gCoordinator->GetComponent<Transform>(mCamera);
	auto& camera = gCoordinator->GetComponent<tCamera>(mCamera);

	Camera cam{1.f,1.f,1.f,1.f};
	Renderer::RenderSceneBegin(cam);

	for (auto const& entity : mEntities)
	{
		auto const& transform = gCoordinator->GetComponent<Transform>(entity);
		auto const& renderable = gCoordinator->GetComponent<Renderable>(entity);

		//glPolygonMode(GL_FRONT_AND_BACK, renderable.drawMode);

		Mat44 view = tCamera::MakeViewTransform(camera.eye, camera.tgt, camera.up);

		Mat44 translate{ 1 };
		translate[3][0] = transform.position.x;
		translate[3][1] = transform.position.y;
		translate[3][2] = transform.position.z;

		Mat44 scaleMat{ 1 };
		scaleMat[0][0] = transform.scale.x;
		scaleMat[1][1] = transform.scale.y;
		scaleMat[2][2] = transform.scale.z;

		Mat44 model = translate * scaleMat;

		mLastModelXform = model;

		Mat44 projection = camera.projectionTransform;

		//shader->SetUniform("uModel", model);
		//shader->SetUniform("uView", view);
		//shader->SetUniform("uProjection", projection);
		//shader->SetUniform("uColor", renderable.color);

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		Renderer::DrawQuad(transform.position, transform.scale, { renderable.color,1.f });
	}

	Renderer::RenderSceneEnd();

	//glBindVertexArray(0);
}

void RenderSystem::WindowSizeListener(Event& event)
{
	auto windowWidth = event.GetParam<unsigned int>(Events::Window::Resized::WIDTH);
	auto windowHeight = event.GetParam<unsigned int>(Events::Window::Resized::HEIGHT);

	auto& camera = gCoordinator->GetComponent<tCamera>(mCamera);
	//camera.projectionTransform = Camera::MakeProjectionTransform(45.0f, 0.1f, 1000.0f, windowWidth, windowHeight);
	camera.projectionTransform = tCamera::MakeProjectionTransform(-50, 50, -50, 50, 0, -100);
}

