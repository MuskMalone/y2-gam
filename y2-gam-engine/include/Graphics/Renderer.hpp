#pragma once

#include <memory>
#include "glm/glm.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/ElementBuffer.hpp"
#include "OrthoCamera.hpp"

class Renderer {
public:

	static void Init();
	static void Shutdown();

	static void RenderSceneBegin(OrthoCamera const& camera);
	static void RenderSceneEnd();

	static unsigned int GetMaxTextureUnits();
	static void SetQuadBufferData(const glm::vec3& pos, const glm::vec2& scale,
		const glm::vec4& clr, const glm::vec2& texCoord,
		float texIdx);
	static void SetLineBufferData(glm::vec3 const& pos, glm::vec4 const& clr);

	static void DrawQuad(glm::vec2 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot = 0.f);
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot = 0.f);

	static void DrawQuad(glm::vec2 const& pos, glm::vec2 const& scale,
		std::shared_ptr<Texture>const& tex, float rot = 0.f);
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
		std::shared_ptr<Texture>const& tex, float rot = 0.f);

	static void DrawLine(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec4 const& clr);
	static void DrawLineRect(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr);

	static void FlushBatch();

	//OpenGL Render Commands
	static void SetClearColor(glm::vec4 const& color);
	static void ClearColor();
	static void ClearDepth();
	static void DrawIndexed(std::shared_ptr<VertexArray> const& vertexArray, unsigned int idxCount = 0);
	static void DrawLineArray(std::shared_ptr<VertexArray> const& vao, unsigned int vtxCount);
};