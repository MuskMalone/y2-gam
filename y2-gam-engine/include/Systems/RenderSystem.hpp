#pragma once

#include "Core/System.hpp"
#include "Graphics/Shader.hpp"
#include <memory>
#include <Math/Mat44.hpp>
#include "Graphics/Framebuffer.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"

class Event;


class RenderSystem : public System
{
public:
	void Init();

	void Update(float dt);

	Entity GetCamera();

	std::shared_ptr<Framebuffer> const& GetFramebuffer() const;
	//for debug
	//Mat44 mLastModelXform;
	void ToggleDebugMode();

private:
	bool mDebugMode{false};

	void WindowSizeListener(Event& event);
	std::shared_ptr<Framebuffer> mFramebuffer; //TEMP
	std::shared_ptr<SubTexture> mBgSubtex; // TEMP


	Entity mCamera{};

	//GLuint mVao{};
	//GLuint mVboVertices{};
	//GLuint mVboNormals{};
};
