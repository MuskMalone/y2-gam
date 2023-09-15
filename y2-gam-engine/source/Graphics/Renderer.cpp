#include "Graphics/Renderer.hpp"
#include "glad/glad.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

struct QuadVtx {
	glm::vec3 pos;
	glm::vec4 clr;
	glm::vec2 texCoord;
	float texIdx; //float as it is passed to shader
	//test if unsigned int works
};

struct LineVtx {
	glm::vec3 pos;
	glm::vec4 clr;
};

struct RendererData {

	static const unsigned int cMaxQuads{ 10000 };
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
	QuadVtx* quadBuffer{ nullptr }; // Dynamic buffer to hold vertex data for batching
	QuadVtx* quadBufferPtr{ nullptr }; // Pointer to the current position in the buffer

	unsigned int lineVtxCount{};
	LineVtx* lineBuffer{ nullptr };
	LineVtx* lineBufferPtr{ nullptr };

	glm::vec4 quadVtxPos[4];

	std::unique_ptr<std::shared_ptr<Texture>[]> texUnits; //pointer to an array of Texture pointers (may change to vector)
	unsigned int texUnitIdx{ 1 }; // 0 = white tex

	Renderer::Statistics stats;
};

static RendererData sData;

void Renderer::Init() {

	
	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_DEPTH_TEST);

	sData.maxTexUnits = GetMaxTextureUnits();
	sData.texUnits.reset(new std::shared_ptr<Texture>[sData.maxTexUnits]);

	sData.quadVertexArray.reset(new VertexArray);

	sData.quadVertexBuffer.reset(new VertexBuffer{ sData.cMaxVertices * sizeof(QuadVtx) });

	BufferLayout quadLayout = {
		{AttributeType::VEC3, "a_Position"},
		{AttributeType::VEC4, "a_Color"},
		{AttributeType::VEC2, "a_TexCoord"},
		{AttributeType::FLOAT, "a_TexIdx"}
	};

	sData.quadVertexBuffer->SetLayout(quadLayout); //must set layout before adding vbo
	sData.quadVertexArray->AddVertexBuffer(sData.quadVertexBuffer);
	sData.quadBuffer = new QuadVtx[sData.cMaxVertices]; //deleted in shutdown

	unsigned int* quadIndices = new unsigned int[sData.cMaxIndices]; //might need to refactor, do a ref count?
	//std::unique_ptr<unsigned int[]> quadIndices{new unsigned int[sData.cMaxIndices]};

	unsigned int offset{};
	for (unsigned int i{}; i < sData.cMaxIndices; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	std::shared_ptr<ElementBuffer> quadIB;
	quadIB.reset(new ElementBuffer(quadIndices, sData.cMaxIndices));
	sData.quadVertexArray->SetElementBuffer(quadIB);
	delete[] quadIndices; //assumes indices gets uploaded by gpu immediately this line might cause trouble

	//Lines
	sData.lineVertexArray.reset(new VertexArray);
	sData.lineVertexBuffer.reset(new VertexBuffer{ sData.cMaxVertices * sizeof(LineVtx) });

	BufferLayout lineLayout = {
		{AttributeType::VEC3, "a_Position"},
		{AttributeType::VEC4, "a_Color"},
	};

	sData.lineVertexBuffer->SetLayout(lineLayout); //must set layout before adding vbo
	sData.lineVertexArray->AddVertexBuffer(sData.lineVertexBuffer);
	sData.lineBuffer = new LineVtx[sData.cMaxVertices]; //change this in future.. deleted in shutdown

	sData.whiteTex.reset(new Texture{ 1, 1 });
	unsigned int whiteTexData{ 0xffffffff };
	sData.whiteTex->SetData(&whiteTexData);
	sData.texUnits[0] = sData.whiteTex;

	std::vector<int> samplers(sData.maxTexUnits);
	for (unsigned int i{}; i < sData.maxTexUnits; ++i)
		samplers[i] = i;

	sData.lineShader.reset(new Shader{ "../Shaders/Line.vert", "../Shaders/Line.frag" });
	sData.texShader.reset(new Shader{ "../Shaders/Tex.vert", "../Shaders/Tex.frag" });
	sData.texShader->Use();
	sData.texShader->SetUniform("u_Tex", samplers.data(), sData.maxTexUnits);

	sData.quadVtxPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	sData.quadVtxPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	sData.quadVtxPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	sData.quadVtxPos[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}
void Renderer::Shutdown() {
	delete[] sData.quadBuffer;
	delete[] sData.lineBuffer;
}

void Renderer::RenderSceneBegin(OrthoCamera const& camera) {

	sData.texShader->Use();
	sData.texShader->SetUniform("u_ViewProjMtx", camera.GetViewProjMtx());

	sData.lineShader->Use();
	sData.lineShader->SetUniform("u_ViewProjMtx", camera.GetViewProjMtx());

	//start Batch
	BeginBatch();
}

void Renderer::RenderSceneEnd() {

	FlushBatch();
}

unsigned int Renderer::GetMaxTextureUnits() {
	int maxTexUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTexUnits);
	return static_cast<unsigned int>(maxTexUnits);
}

void Renderer::SetQuadBufferData(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, glm::vec2 const& texCoord, float texIdx) {
	sData.quadBufferPtr->pos = pos;
	sData.quadBufferPtr->clr = clr; // Ensure color is set for each vertex
	sData.quadBufferPtr->texCoord = texCoord;
	sData.quadBufferPtr->texIdx = texIdx;
	++sData.quadBufferPtr;
}

void Renderer::SetLineBufferData(glm::vec3 const& pos, glm::vec4 const& clr) {
	sData.lineBufferPtr->pos = pos;
	sData.lineBufferPtr->clr = clr;
	++sData.lineBufferPtr;
}

//FOR DRAWING FLAT COLORED QUAD
//rotation is an optinal paramter
void Renderer::DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot) {

	if (sData.quadIdxCount >= RendererData::cMaxIndices)
		NextBatch();

	const float texIdx{}; //white tex

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };
	glm::mat4 transformMtx{ translateMtx * rotateMtx * scaleMtx };

	SetQuadBufferData(transformMtx * sData.quadVtxPos[0], scale, clr, { 0.f, 0.f }, texIdx);
	SetQuadBufferData(transformMtx * sData.quadVtxPos[1], scale, clr, { 1.f, 0.f }, texIdx);
	SetQuadBufferData(transformMtx * sData.quadVtxPos[2], scale, clr, { 1.f, 1.f }, texIdx);
	SetQuadBufferData(transformMtx * sData.quadVtxPos[3], scale, clr, { 0.f, 1.f }, texIdx);

	sData.quadIdxCount += 6;

	++sData.stats.quadCount;
}

void Renderer::DrawQuad(glm::vec2 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot) {
	DrawQuad({ pos.x, pos.y, 0.f }, scale, clr, rot);
}

//FOR DRAWING TEXTURED QUAD
//rotation is an optional parameter
//this version takes in vec2 for pos
void Renderer::DrawQuad(glm::vec2 const& pos, glm::vec2 const& scale, std::shared_ptr<Texture>const& tex, float rot) {
	DrawQuad({ pos.x, pos.y, 0.f }, scale, tex, rot);
}

//this version takes in vec3 for pos
void Renderer::DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
	std::shared_ptr<Texture>const& tex, float rot) {
	if (sData.quadIdxCount >= RendererData::cMaxIndices)
		NextBatch();

	constexpr glm::vec4 clr{ 1.f, 1.f, 1.f, 1.f }; //TODO add parameter tint

	float texIdx = 0.f;

	//don't need to iterate all 32 slots every time
	for (unsigned int i{ 1 }; i < sData.texUnitIdx; ++i) {
		if (*sData.texUnits[i].get() == *tex.get()) { //check if the particular texture has already been set
			texIdx = static_cast<float>(i);
			break;
		}
	}

	if (texIdx == 0.f) {
		texIdx = static_cast<float>(sData.texUnitIdx);
		sData.texUnits[sData.texUnitIdx] = tex;
		++sData.texUnitIdx;
	}

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };

	glm::mat4 transformMtx{ translateMtx * rotateMtx * scaleMtx };

	SetQuadBufferData(transformMtx * sData.quadVtxPos[0], scale, clr, { 0.f, 0.f }, texIdx);
	SetQuadBufferData(transformMtx * sData.quadVtxPos[1], scale, clr, { 1.f, 0.f }, texIdx);
	SetQuadBufferData(transformMtx * sData.quadVtxPos[2], scale, clr, { 1.f, 1.f }, texIdx);
	SetQuadBufferData(transformMtx * sData.quadVtxPos[3], scale, clr, { 0.f, 1.f }, texIdx);

	sData.quadIdxCount += 6;

	++sData.stats.quadCount;
}

void Renderer::DrawLine(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec4 const& clr) {
	if (sData.lineVtxCount >= RendererData::cMaxVertices)
		NextBatch();

	SetLineBufferData(p0, clr);
	SetLineBufferData(p1, clr);

	sData.lineVtxCount += 2;

	++sData.stats.lineCount;
}

void Renderer::DrawLineRect(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr) {
	glm::vec3 p0{ glm::vec3(pos.x - scale.x * 0.5f, pos.y - scale.y * 0.5f, pos.z) };
	glm::vec3 p1{ glm::vec3(pos.x + scale.x * 0.5f, pos.y - scale.y * 0.5f, pos.z) };
	glm::vec3 p2{ glm::vec3(pos.x + scale.x * 0.5f, pos.y + scale.y * 0.5f, pos.z) };
	glm::vec3 p3{ glm::vec3(pos.x - scale.x * 0.5f, pos.y + scale.y * 0.5f, pos.z) };

	DrawLine(p0, p1, clr);
	DrawLine(p1, p2, clr);
	DrawLine(p2, p3, clr);
	DrawLine(p3, p0, clr);
}

void Renderer::FlushBatch() {
	if (sData.quadIdxCount) {
		ptrdiff_t difference{ reinterpret_cast<unsigned char*>(sData.quadBufferPtr)
							- reinterpret_cast<unsigned char*>(sData.quadBuffer) };

		//how many elements it takes up in terms of bytes
		unsigned int dataSize = static_cast<unsigned int>(difference);
		//uint32_t dataSize = (uint32_t)((uint8_t*)sData.quadBufferPtr - (uint8_t*)sData.quadBuffer);
		sData.quadVertexBuffer->SetData(sData.quadBuffer, dataSize);

		//Bind all the textures that has been set
		for (unsigned int i{}; i < sData.texUnitIdx; ++i) {
			sData.texUnits[i]->Bind(i);
		}
		sData.texShader->Use();
		DrawIndexed(sData.quadVertexArray, sData.quadIdxCount);

		++sData.stats.drawCalls;
	}

	if (sData.lineVtxCount) {
		ptrdiff_t difference{ reinterpret_cast<unsigned char*>(sData.lineBufferPtr)
							- reinterpret_cast<unsigned char*>(sData.lineBuffer) };

		//how many elements it takes up in terms of bytes
		unsigned int dataSize = static_cast<unsigned int>(difference);
		sData.lineVertexBuffer->SetData(sData.lineBuffer, dataSize);

		sData.lineShader->Use();
		DrawLineArray(sData.lineVertexArray, sData.lineVtxCount);

		++sData.stats.drawCalls;
	}
}

void Renderer::BeginBatch() {
	sData.quadIdxCount = 0;
	sData.quadBufferPtr = sData.quadBuffer;

	sData.lineVtxCount = 0;
	sData.lineBufferPtr = sData.lineBuffer;

	sData.texUnitIdx = 1;
}
void Renderer::NextBatch() {
	FlushBatch();
	BeginBatch();

	//auto stats = GetStats();
	//std::cout << "Renderer Stats:\n"
	//	<< "Draw Calls: " << stats.drawCalls
	//	<< "\nQuads: " << stats.quadCount
	//	<< "\nVertices: " << stats.GetTotalVtxCount()
	//	<< "\nIndices: " << stats.GetTotalIdxCount();
	//ResetStats();
}

//OpenGL Commands
void Renderer::SetClearColor(glm::vec4 const& color) {
	//might change to clearColor member variable
	glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::ClearColor() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawIndexed(std::shared_ptr<VertexArray> const& vao, unsigned int idxCount) {
	vao->Bind();
	unsigned int count = idxCount ? idxCount : vao->GetElementBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, vao->GetElementBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLineArray(std::shared_ptr<VertexArray> const& vao, unsigned int vtxCount) {
	vao->Bind();
	glDrawArrays(GL_LINES, 0, vtxCount);
}

void Renderer::ResetStats() {
	memset(&sData.stats, 0, sizeof(Statistics));
}

Renderer::Statistics Renderer::GetStats() {
	return sData.stats;
}