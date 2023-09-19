#pragma once

#include "Core/System.hpp"
#include "Graphics/Shader.hpp"
#include <memory>
#include <Math/Mat44.hpp>


class Event;


class RenderSystem : public System
{
public:
	void Init();

	void Update(float dt);

	Entity GetCamera();
	//for debug
	//Mat44 mLastModelXform;

private:
	void WindowSizeListener(Event& event);

	std::unique_ptr<Shader> shader;


	Entity mCamera{};

	GLuint mVao{};
	GLuint mVboVertices{};
	GLuint mVboNormals{};
};
