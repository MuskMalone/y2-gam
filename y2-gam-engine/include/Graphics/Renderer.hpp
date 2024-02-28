/******************************************************************************/
/*!
\par        Image Engine
\file       Renderer.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Header file for graphics rendering operations

			Contains the definitions for initializing graphics
			settings, managing texture units, and rendering various graphical
			primitives such as quads, lines, and sprites. It also provides
			utility functions for setting buffer data and flushing batches.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/VertexBuffer.hpp"
#include "Graphics/ElementBuffer.hpp"
#include "Components/Transform.hpp"

struct Statistics {
	unsigned int drawCalls{};
	unsigned int quadCount{};
	unsigned int lineCount{};

	unsigned int GetTotalVtxCount() { return quadCount * 4; }
	unsigned int GetTotalIdxCount() { return quadCount * 6; }
};


struct ScreenVtx {
	glm::vec2 pos;
	glm::vec2 texCoord;
};

struct FullscreenQuad {
	std::shared_ptr<VertexArray> screenVertexArray;
	std::shared_ptr<VertexBuffer> screenVertexBuffer;
	std::shared_ptr<Shader> screenShader;
	std::array<ScreenVtx, 6> screenVertices;
};

struct QuadVtx {
	glm::vec3 pos;
	glm::vec4 clr;
	glm::vec2 texCoord;
	float texIdx; //float as it is passed to shader
	//TODO test if unsigned int works
	int entity {};

};

struct LineVtx {
	glm::vec3 pos;
	glm::vec4 clr;
};

struct CircleVtx {
	glm::vec3 worldPos;
	glm::vec3 localPos;
	glm::vec4 clr;
	float thickness;
	float fade;
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

	std::shared_ptr<VertexArray> circleVertexArray;
	std::shared_ptr<VertexBuffer> circleVertexBuffer;
	std::shared_ptr<Shader> circleShader;

	std::shared_ptr<Shader> postProcShader;

	unsigned int quadIdxCount{};
	std::vector<QuadVtx> quadBuffer; // Dynamic buffer to hold vertex data for batching
	QuadVtx* quadBufferPtr{ nullptr }; // Pointer to the current position in the buffer

	unsigned int lineVtxCount{};
	std::vector<LineVtx> lineBuffer;
	LineVtx* lineBufferPtr{ nullptr };

	unsigned int circleIdxCount{};
	std::vector<CircleVtx> circleBuffer;
	CircleVtx* circleBufferPtr{ nullptr };

	std::array<glm::vec4, 4> quadVtxPos{};

	std::vector<std::shared_ptr<Texture>> texUnits; //pointer to an array of Texture pointers (may change to vector)
	unsigned int texUnitIdx{ 1 }; // 0 = white tex

	FullscreenQuad screen;
	Statistics stats;
};

class Renderer {
public:

	static void Init();
	static void Shutdown();

	static void RenderFullscreenTexture(unsigned int tex);
	static void ApplyPostProcessing(unsigned int texture);
	static void RenderSceneBegin(glm::mat4 const& viewProjMtx);
	static void RenderSceneEnd();

	static unsigned int GetMaxTextureUnits();

	//Quads
	//TODO overload transform parameter
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot = 0.f, int entity = -1);

	//TODO add tint 
	static void DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
		std::shared_ptr<Texture>const& tex, float rot = 0.f, int entity = -1);

	static void DrawSprite(glm::vec3 const& pos, glm::vec2 const& scale, std::shared_ptr<SubTexture>const& subtex, glm::vec4 const& tint = {1.f,1.f,1.f,1.f}, float rot = 0.f, int entity = -1);
	static void DrawSprite(Transform const& transform, std::shared_ptr<SubTexture> const& subtex, glm::vec4 const& tint = { 1.f,1.f,1.f,1.f }, int entity = -1);
	
	//Lines
	static void DrawLine(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec4 const& clr);
	static void DrawLineRect(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float = 0.f);

	//Circles
	static void DrawCircle(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float thickness = 1.f, float fade = 0.005f);

	static void FlushBatch();
private:
	static void SetQuadBufferData(const glm::vec3& pos, const glm::vec2& scale,
		const glm::vec4& clr, const glm::vec2& texCoord,
		float texIdx, int entity);
	static void SetLineBufferData(glm::vec3 const& pos, glm::vec4 const& clr);
	static void BeginBatch();
	static void NextBatch();
public:
	//OpenGL Render Commands
	static void SetViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
	static void SetClearColor(glm::vec4 const& color);
	static void ClearColor();
	static void ClearDepth();
	static void DrawIndexed(std::shared_ptr<VertexArray> const& vertexArray, unsigned int idxCount = 0);
	static void DrawLineArray(std::shared_ptr<VertexArray> const& vao, unsigned int vtxCount);

	//Stats
	static Statistics GetStats();
	static void ResetStats();

private:
	static RendererData mData;
};