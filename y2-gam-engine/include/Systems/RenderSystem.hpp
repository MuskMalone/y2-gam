#pragma once

#include "Core/System.hpp"
#include "Graphics/Shader.hpp"
#include <memory>
#include <Math/Mat44.hpp>
#include "Graphics/Framebuffer.hpp"
#include "Graphics/Texture.hpp"

class Event;


class RenderSystem : public System
{
public:
	void Init();

	void Update(float dt);

	Entity GetCamera();

	std::shared_ptr<Framebuffer> const& GetFramebuffer() const { return mFramebuffer; }
	//for debug
	//Mat44 mLastModelXform;

private:
	void WindowSizeListener(Event& event);
	std::shared_ptr<Framebuffer> mFramebuffer; //TEMP
	//std::unique_ptr<Shader> shader;


	Entity mCamera{};

	//GLuint mVao{};
	//GLuint mVboVertices{};
	//GLuint mVboNormals{};
};
