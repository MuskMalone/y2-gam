#pragma once

#include <memory>
#include <array>
#include "glm/glm.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/ElementBuffer.hpp"
#include "Components/OrthoCamera.hpp"
#include "Components/Transform.hpp"

struct Statistics {
	unsigned int drawCalls{};
	unsigned int quadCount{};
	unsigned int lineCount{};

	unsigned int GetTotalVtxCount() { return quadCount * 4; }
	unsigned int GetTotalIdxCount() { return quadCount * 6; }
};

struct QuadVtx {
	glm::vec3 pos;
	glm::vec4 clr;
	glm::vec2 texCoord;
	float texIdx; //float as it is passed to shader
	//TODO test if unsigned int works
};

struct LineVtx {
	glm::vec3 pos;
	glm::vec4 clr;
};

struct RendererData {

	static const unsigned int cMaxQuads{ 20000 };
	static const unsigned int cMaxVertices{ cMaxQuads * 4 };
	static const unsigned int cMaxIndices{ cMaxQuads * 6 };
	unsigned int maxTexUnits{}; //set actual number in init 

	std::shared_ptr<VertexArray> quadVertexArray;
	std::shared_ptr<VertexBuffer> quadVertexBuffer;
	std::shared_ptr<Shader> texShader;
	std::shared_ptr<Texture> whiteTex;

	std::shared_ptr<VertexArray> lineVertexArray;
	std::shared_ptr<VertexBuffer> lineVertexBuffer;
	std::shared_ptr<Shader> lineShader;

	unsigned int quadIdxCount{};
	std::vector<QuadVtx> quadBuffer; // Dynamic buffer to hold vertex data for batching
	QuadVtx* quadBufferPtr{ nullptr }; // Pointer to the current position in the buffer

	unsigned int lineVtxCount{};
	std::vector<LineVtx> lineBuffer;
	LineVtx* lineBufferPtr{ nullptr };

	std::array<glm::vec4, 4> quadVtxPos{};

	std::vector<std::shared_ptr<Texture>> texUnits; //pointer to an array of Texture pointers (may change to vector)
	unsigned int texUnitIdx{ 1 }; // 0 = white tex

	Statistics stats;
};

class Renderer {
public:

	static void Init();
	static void Shutdown();

	static void RenderSceneBegin(OrthoCamera const& camera);
	static void RenderSceneEnd();

	static unsigned int GetMaxTextureUnits();

	//Quads
	//TODO overload transform parameter
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot = 0.f);

	//TODO add tint 
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
		std::shared_ptr<Texture>const& tex, float rot = 0.f);

	static void DrawSprite(glm::vec3 const& pos, glm::vec2 const& scale, std::shared_ptr<SubTexture>const& subtex, glm::vec4 const& tint = {1.f,1.f,1.f,1.f}, float rot = 0.f);

	static void DrawSprite(Transform const& transform, std::shared_ptr<SubTexture> const& subtex, glm::vec4 const& tint = { 1.f,1.f,1.f,1.f });
	
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

	static Statistics GetStats();
	static void ResetStats();

private:
	static RendererData sData;
};