#include "Graphics/Renderer.hpp"
#include "glad/glad.h"
#include "Components/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

RendererData Renderer::sData;

void Renderer::Init() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	sData.maxTexUnits = GetMaxTextureUnits();

	sData.texUnits = std::vector<std::shared_ptr<Texture>>(sData.maxTexUnits);

	sData.quadVertexArray = VertexArray::Create();

	sData.quadVertexBuffer = VertexBuffer::Create(sData.cMaxVertices * sizeof(QuadVtx));

	BufferLayout quadLayout = {
		{AttributeType::VEC3, "a_Position"},
		{AttributeType::VEC4, "a_Color"},
		{AttributeType::VEC2, "a_TexCoord"},
		{AttributeType::FLOAT, "a_TexIdx"}
	};

	sData.quadVertexBuffer->SetLayout(quadLayout); //must set layout before adding vbo
	sData.quadVertexArray->AddVertexBuffer(sData.quadVertexBuffer);
	sData.quadBuffer = std::vector<QuadVtx>(sData.cMaxVertices);

	std::vector<unsigned int> quadIndices(sData.cMaxIndices);


	unsigned int offset{};
	for (size_t i{}; i < sData.cMaxIndices; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	std::shared_ptr<ElementBuffer> quadEbo = ElementBuffer::Create(quadIndices.data(), sData.cMaxIndices);
	sData.quadVertexArray->SetElementBuffer(quadEbo);

	//Lines
	sData.lineVertexArray = VertexArray::Create();
	sData.lineVertexBuffer = VertexBuffer::Create(sData.cMaxVertices * sizeof(LineVtx));

	BufferLayout lineLayout = {
		{AttributeType::VEC3, "a_Position"},
		{AttributeType::VEC4, "a_Color"},
	};

	sData.lineVertexBuffer->SetLayout(lineLayout); //must set layout before adding vbo
	sData.lineVertexArray->AddVertexBuffer(sData.lineVertexBuffer);
	sData.lineBuffer = std::vector<LineVtx>(sData.cMaxVertices);

	sData.whiteTex = std::make_shared<Texture>(1, 1);
	unsigned int whiteTexData{ 0xffffffff };
	sData.whiteTex->SetData(&whiteTexData);
	sData.texUnits[0] = sData.whiteTex;

	std::vector<int> samplers(sData.maxTexUnits);
	for (unsigned int i{}; i < sData.maxTexUnits; ++i)
		samplers[i] = i;

	sData.lineShader = std::make_shared<Shader>("../Shaders/Line.vert", "../Shaders/Line.frag");
	sData.texShader = std::make_shared<Shader>("../Shaders/Tex.vert", "../Shaders/Tex.frag");
	sData.texShader->Use();
	sData.texShader->SetUniform("u_Tex", samplers.data(), sData.maxTexUnits);

	sData.quadVtxPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	sData.quadVtxPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	sData.quadVtxPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	sData.quadVtxPos[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}
void Renderer::Shutdown() {

}

void Renderer::RenderSceneBegin(OrthoCamera const& camera) {

	sData.texShader->Use();
	sData.texShader->SetUniform("u_ViewProjMtx", camera.GetViewProjMtx());

	sData.lineShader->Use();
	sData.lineShader->SetUniform("u_ViewProjMtx", camera.GetViewProjMtx());

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

	constexpr glm::vec2 texCoords[4]{ { 0.f, 0.f }, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f} };

	const float texIdx{}; //white tex index = 0

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };
	glm::mat4 transformMtx{ translateMtx * rotateMtx * scaleMtx };

	for (size_t i{}; i < 4; ++i)
		SetQuadBufferData(transformMtx * sData.quadVtxPos[i], scale, clr, texCoords[i], texIdx);

	sData.quadIdxCount += 6;

	//++sData.stats.quadCount;
}

//FOR DRAWING TEXTURED QUAD
//rotation is an optional parameter

void Renderer::DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
	std::shared_ptr<Texture>const& tex, float rot) {
	if (sData.quadIdxCount >= RendererData::cMaxIndices)
		NextBatch();

	constexpr glm::vec4 clr{ 1.f, 1.f, 1.f, 1.f }; //TODO add parameter tint
	constexpr glm::vec2 texCoords[4]{ { 0.f, 0.f }, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f} };

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

	for (size_t i{}; i < 4; ++i)
		SetQuadBufferData(transformMtx * sData.quadVtxPos[i], scale, clr, texCoords[i], texIdx);

	sData.quadIdxCount += 6;

	//++sData.stats.quadCount;
}

//TODO Add duplicated code in function
void Renderer::DrawSprite(glm::vec3 const& pos, glm::vec2 const& scale, std::shared_ptr<SubTexture>const& subtex, glm::vec4 const& tint, float rot) {

	if (sData.quadIdxCount >= RendererData::cMaxIndices)
		NextBatch();

	glm::vec2 const* texCoords{ subtex->GetTexCoords() };
	std::shared_ptr<Texture> tex = subtex->GetTexture();

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

	for (size_t i{}; i < 4; ++i) 
		SetQuadBufferData(transformMtx * sData.quadVtxPos[i], scale, tint, texCoords[i], texIdx);

	sData.quadIdxCount += 6;

	//++sData.stats.quadCount;
}

void Renderer::DrawSprite(Transform const& transform, std::shared_ptr<SubTexture> const& subtex, glm::vec4 const& tint) {
	DrawSprite(transform.position, transform.scale, subtex, tint, transform.rotation.z);
}

void Renderer::DrawLine(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec4 const& clr) {
	if (sData.lineVtxCount >= RendererData::cMaxVertices)
		NextBatch();

	SetLineBufferData(p0, clr);
	SetLineBufferData(p1, clr);

	sData.lineVtxCount += 2;

	//++sData.stats.lineCount;
}

void Renderer::DrawLineRect(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr) {
	glm::vec3 p0{ glm::vec3(pos.x - scale.x * 0.5f, pos.y - scale.y * 0.5f, pos.z) };
	glm::vec3 p1{ glm::vec3(pos.x + scale.x * 0.5f, pos.y - scale.y * 0.5f, pos.z) };
	glm::vec3 p2{ glm::vec3(pos.x + scale.x * 0.5f, pos.y + scale.y * 0.5f, pos.z) };
	glm::vec3 p3{ glm::vec3(pos.x - scale.x * 0.5f, pos.y + scale.y * 0.5f, pos.z) };

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	//glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };
	glm::mat4 transformMtx{ translateMtx * scaleMtx };

	DrawLine(p0, p1, clr);
	DrawLine(p1, p2, clr);
	DrawLine(p2, p3, clr);
	DrawLine(p3, p0, clr);
}

void Renderer::FlushBatch() {
	if (sData.quadIdxCount) {
		ptrdiff_t difference{ reinterpret_cast<unsigned char*>(sData.quadBufferPtr)
							- reinterpret_cast<unsigned char*>(sData.quadBuffer.data()) };

		//how many elements it takes up in terms of bytes
		unsigned int dataSize = static_cast<unsigned int>(difference);
		//uint32_t dataSize = (uint32_t)((uint8_t*)sData.quadBufferPtr - (uint8_t*)sData.quadBuffer);
		sData.quadVertexBuffer->SetData(sData.quadBuffer.data(), dataSize);

		//Bind all the textures that has been set
		for (unsigned int i{}; i < sData.texUnitIdx; ++i) {
			sData.texUnits[i]->Bind(i);
		}
		sData.texShader->Use();
		DrawIndexed(sData.quadVertexArray, sData.quadIdxCount);

		//++sData.stats.drawCalls;
	}

	if (sData.lineVtxCount) {
		ptrdiff_t difference{ reinterpret_cast<unsigned char*>(sData.lineBufferPtr)
							- reinterpret_cast<unsigned char*>(sData.lineBuffer.data()) };

		//how many elements it takes up in terms of bytes
		unsigned int dataSize = static_cast<unsigned int>(difference);
		sData.lineVertexBuffer->SetData(sData.lineBuffer.data(), dataSize);

		sData.lineShader->Use();
		DrawLineArray(sData.lineVertexArray, sData.lineVtxCount);

		//++sData.stats.drawCalls;
	}
}

void Renderer::BeginBatch() {
	sData.quadIdxCount = 0;
	sData.quadBufferPtr = sData.quadBuffer.data();
	//sData.quadBuffer.clear();
	sData.lineVtxCount = 0;
	sData.lineBufferPtr = sData.lineBuffer.data();
	//sData.lineBuffer.clear();
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
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawLineArray(std::shared_ptr<VertexArray> const& vao, unsigned int vtxCount) {
	vao->Bind();
	glDrawArrays(GL_LINES, 0, vtxCount);
}

void Renderer::ResetStats() {
	memset(&sData.stats, 0, sizeof(Statistics));
}

Statistics Renderer::GetStats() {
	return sData.stats;
}