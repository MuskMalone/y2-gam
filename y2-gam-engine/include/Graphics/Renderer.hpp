#pragma once

#include <memory>
#include "glm/glm.hpp"

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"
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

	//Quads
	//TODO overload transform parameter
	static void DrawQuad(glm::vec2 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot = 0.f);
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot = 0.f);

	//TODO add tint 
	static void DrawQuad(glm::vec2 const& pos, glm::vec2 const& scale,
		std::shared_ptr<Texture>const& tex, float rot = 0.f);
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
		std::shared_ptr<Texture>const& tex, float rot = 0.f);

	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
		std::shared_ptr<SubTexture>const& subtex, float rot = 0.f);
	
	//Lines
	static void DrawLine(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec4 const& clr);
	static void DrawLineRect(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr);

	static void FlushBatch();
private:
	static void SetQuadBufferData(const glm::vec3& pos, const glm::vec2& scale,
		const glm::vec4& clr, const glm::vec2& texCoord,
		float texIdx);
	static void SetLineBufferData(glm::vec3 const& pos, glm::vec4 const& clr);
	static void BeginBatch();
	static void NextBatch();
public:
	//OpenGL Render Commands
	static void SetClearColor(glm::vec4 const& color);
	static void ClearColor();
	static void ClearDepth();
	static void DrawIndexed(std::shared_ptr<VertexArray> const& vertexArray, unsigned int idxCount = 0);
	static void DrawLineArray(std::shared_ptr<VertexArray> const& vao, unsigned int vtxCount);

	//Stats
	struct Statistics {
		unsigned int drawCalls{};
		unsigned int quadCount{};
		unsigned int lineCount{};

		unsigned int GetTotalVtxCount() { return quadCount * 4; }
		unsigned int GetTotalIdxCount() { return quadCount * 6; }
	};
	static Statistics GetStats();
	static void ResetStats();
};