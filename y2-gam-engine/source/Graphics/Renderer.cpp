/******************************************************************************/
/*!
\par        Image Engine
\file       Renderer.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Implementation file for graphics rendering operations

			Contains the definitions for initializing graphics
			settings, managing texture units, and rendering various graphical
			primitives such as quads, lines, and sprites. It also provides
			utility functions for setting buffer data and flushing batches.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Graphics/Renderer.hpp"
#include "Components/Transform.hpp"
#include <Core/Globals.hpp>

RendererData Renderer::mData;

/*  _________________________________________________________________________ */
/*! Init

@return none.

Initializes the renderer.
This method sets up OpenGL states, initializes data structures, and prepares shaders.
*/
void Renderer::Init() {

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	mData.maxTexUnits = GetMaxTextureUnits();

	mData.texUnits = std::vector<std::shared_ptr<Texture>>(mData.maxTexUnits);

	//Quads
	mData.quadVertexArray = VertexArray::Create();
	mData.quadVertexBuffer = VertexBuffer::Create(mData.cMaxVertices * sizeof(QuadVtx));

	BufferLayout quadLayout = {
		{AttributeType::VEC3, "a_Position"},
		{AttributeType::VEC4, "a_Color"},
		{AttributeType::VEC2, "a_TexCoord"},
		{AttributeType::FLOAT, "a_TexIdx"},
		{AttributeType::INT, "a_E ntity"}
	};

	mData.quadVertexBuffer->SetLayout(quadLayout); //must set layout before adding vbo
	mData.quadVertexArray->AddVertexBuffer(mData.quadVertexBuffer);
	mData.quadBuffer = std::vector<QuadVtx>(mData.cMaxVertices);

	std::vector<unsigned int> quadIndices(mData.cMaxIndices);


	unsigned int offset{};
	for (size_t i{}; i < mData.cMaxIndices; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	std::shared_ptr<ElementBuffer> quadEbo = ElementBuffer::Create(quadIndices.data(), mData.cMaxIndices);
	mData.quadVertexArray->SetElementBuffer(quadEbo);

	//Lines
	mData.lineVertexArray = VertexArray::Create();
	mData.lineVertexBuffer = VertexBuffer::Create(mData.cMaxVertices * sizeof(LineVtx));

	BufferLayout lineLayout = {
		{AttributeType::VEC3, "a_Position"},
		{AttributeType::VEC4, "a_Color"},
	};

	mData.lineVertexBuffer->SetLayout(lineLayout); //must set layout before adding vbo
	mData.lineVertexArray->AddVertexBuffer(mData.lineVertexBuffer);
	mData.lineBuffer = std::vector<LineVtx>(mData.cMaxVertices);

	//Circles
	mData.circleVertexArray = VertexArray::Create();

	mData.circleVertexBuffer = VertexBuffer::Create(mData.cMaxVertices * sizeof(CircleVtx));

	BufferLayout circleLayout = {
		{AttributeType::VEC3, "a_WorldPosition"},
		{AttributeType::VEC3, "a_LocalPosition"},
		{AttributeType::VEC4, "a_Color"},
		{AttributeType::FLOAT, "a_Thickness"},
		{AttributeType::FLOAT, "a_Fade" }
	};

	mData.circleVertexBuffer->SetLayout(circleLayout); //must set layout before adding vbo
	mData.circleVertexArray->AddVertexBuffer(mData.circleVertexBuffer);
	mData.circleVertexArray->SetElementBuffer(quadEbo); //reusing quad EBO due to same geometry for circle
	mData.circleBuffer = std::vector<CircleVtx>(mData.cMaxVertices);
	
	mData.whiteTex = std::make_shared<Texture>(1, 1);
	unsigned int whiteTexData{ 0xffffffff };
	mData.whiteTex->SetData(&whiteTexData);
	mData.texUnits[0] = mData.whiteTex;

	std::vector<int> samplers(mData.maxTexUnits);
	for (unsigned int i{}; i < mData.maxTexUnits; ++i)
		samplers[i] = i;

	mData.lineShader = std::make_shared<Shader>("../Shaders/Line.vert", "../Shaders/Line.frag");
	mData.texShader = std::make_shared<Shader>("../Shaders/Tex.vert", "../Shaders/Tex.frag");
	mData.circleShader = std::make_shared<Shader>("../Shaders/Circle.vert", "../Shaders/Circle.frag");

	mData.texShader->Use();
	mData.texShader->SetUniform("u_Tex", samplers.data(), mData.maxTexUnits);

	//Universal Mesh
	mData.quadVtxPos[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	mData.quadVtxPos[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
	mData.quadVtxPos[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
	mData.quadVtxPos[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}

/*  _________________________________________________________________________ */
/*! Shutdown

@return none.

Shuts down the renderer.
This method is currently empty and can be used for cleanup if necessary.
*/
void Renderer::Shutdown() {
	//TODO SHIFT RENDERER DESTROY HERE
}

/*  _________________________________________________________________________ */
/*! RenderSceneBegin

@param camera
The orthographic camera used for rendering.

@return none.

Begins the rendering scene.
Sets up the shaders and view projection matrices based on the provided camera.
*/
void Renderer::RenderSceneBegin(glm::mat4 const& viewProjMtx) {

	mData.texShader->Use();
	mData.texShader->SetUniform("u_ViewProjMtx", viewProjMtx);

	mData.lineShader->Use();
	mData.lineShader->SetUniform("u_ViewProjMtx", viewProjMtx);

	mData.circleShader->Use();
	mData.circleShader->SetUniform("u_ViewProjMtx", viewProjMtx);

	BeginBatch();
}

/*  _________________________________________________________________________ */
/*! RenderSceneEnd

@return none.

Ends the rendering scene.
Flushes the batch of primitives to be rendered.
*/
void Renderer::RenderSceneEnd() {

	FlushBatch();
}

/*  _________________________________________________________________________ */
/*! GetMaxTextureUnits

@return
The maximum number of texture units supported by the GPU.
*/
unsigned int Renderer::GetMaxTextureUnits() {
	int maxTexUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTexUnits);
	return static_cast<unsigned int>(maxTexUnits);
}

/*  _________________________________________________________________________ */
  /*! SetQuadBufferData

  @param pos
  Position of the quad.

  @param scale
  Scale of the quad.

  @param clr
  Color of the quad.

  @param texCoord
  Texture coordinates for the quad.

  @param texIdx
  Texture index for multi-texturing.

  @return none.

  Sets the buffer data for a quad.
  This method populates the quad buffer with vertex data.
  */
void Renderer::SetQuadBufferData(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, glm::vec2 const& texCoord, float texIdx, int entity) {
	UNREFERENCED_PARAMETER(scale);
	mData.quadBufferPtr->pos = pos;
	mData.quadBufferPtr->clr = clr; // Ensure color is set for each vertex
	mData.quadBufferPtr->texCoord = texCoord;
	mData.quadBufferPtr->texIdx = texIdx;
	mData.quadBufferPtr->entity = entity;
	++mData.quadBufferPtr;
}

/*  _________________________________________________________________________ */
/*! SetLineBufferData

@param pos
Position of the line vertex.

@param clr
Color of the line.

@return none.

Sets the buffer data for a line.
This method populates the line buffer with vertex data.
*/
void Renderer::SetLineBufferData(glm::vec3 const& pos, glm::vec4 const& clr) {
	mData.lineBufferPtr->pos = pos;
	mData.lineBufferPtr->clr = clr;
	++mData.lineBufferPtr;
}

/*  _________________________________________________________________________ */
/*! DrawQuad

@param pos
Position of the quad.

@param scale
Scale of the quad.

@param clr
Color of the quad.

@param rot (optional)
Rotation of the quad. Default is 0.0f.

@return none.

Draws a flat-colored quad.
*/
void Renderer::DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot, int entity) {

	if (mData.quadIdxCount >= RendererData::cMaxIndices)
		NextBatch();

	constexpr glm::vec2 texCoords[4]{ { 0.f, 0.f }, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f} };

	const float texIdx{}; //white tex index = 0

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };
	glm::mat4 transformMtx{ translateMtx * rotateMtx * scaleMtx };

	for (size_t i{}; i < 4; ++i)
		SetQuadBufferData(transformMtx * mData.quadVtxPos[i], scale, clr, texCoords[i], texIdx, entity);

	mData.quadIdxCount += 6;

	//++mData.stats.quadCount;
}

/*  _________________________________________________________________________ */
/*! DrawQuad (Overloaded)

@param pos
Position of the quad.

@param scale
Scale of the quad.

@param clr
Color of the quad.

@param texture
Texture to be applied to the quad.

@param rot (optional)
Rotation of the quad. Default is 0.0f.

@return none.

This overloaded version draws a textured quad.
*/
void Renderer::DrawQuad(glm::vec3 const& pos, glm::vec2 const& scale,
	std::shared_ptr<Texture>const& tex, float rot, int entity) {
	if (mData.quadIdxCount >= RendererData::cMaxIndices)
		NextBatch();

	constexpr glm::vec4 clr{ 1.f, 1.f, 1.f, 1.f }; //TODO add parameter tint
	constexpr glm::vec2 texCoords[4]{ { 0.f, 0.f }, {1.f, 0.f}, {1.f, 1.f}, {0.f, 1.f} };

	float texIdx = 0.f;

	//don't need to iterate all 32 slots every time
	for (unsigned int i{ 1 }; i < mData.texUnitIdx; ++i) {
		if (*mData.texUnits[i].get() == *tex.get()) { //check if the particular texture has already been set
			texIdx = static_cast<float>(i);
			break;
		}
	}

	if (texIdx == 0.f) {
		texIdx = static_cast<float>(mData.texUnitIdx);
		mData.texUnits[mData.texUnitIdx] = tex;
		++mData.texUnitIdx;
	}

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };
	glm::mat4 transformMtx{ translateMtx * rotateMtx * scaleMtx };

	for (size_t i{}; i < 4; ++i)
		SetQuadBufferData(transformMtx * mData.quadVtxPos[i], scale, clr, texCoords[i], texIdx, entity);

	mData.quadIdxCount += 6;

	//++mData.stats.quadCount;
}

/*  _________________________________________________________________________ */
/*! DrawSprite

@param pos
Position of the sprite.

@param scale
Scale of the sprite.

@param subtex
A shared pointer to the SubTexture object which contains the texture information
for the sprite.

@param tint
Color tint to be applied to the sprite.

@param rot (optional)
Rotation of the sprite. Default is 0.0f.

@return none.

This function is responsible for drawing a sprite on the screen. It checks if 
the maximum indices have been reached and if so, it moves to the next batch.
The function also checks if a particular texture has already been set to avoid
unnecessary texture bindings. The sprite's position, scale, and rotation are combined
to form a transformation matrix which is then used to set the quad buffer data. The
function increments the quad index count by 6 for each sprite drawn.

*/
//TODO Add duplicated code in function
void Renderer::DrawSprite(glm::vec3 const& pos, glm::vec2 const& scale, std::shared_ptr<SubTexture>const& subtex, glm::vec4 const& tint, float rot, int entity) {

	if (mData.quadIdxCount >= RendererData::cMaxIndices)
		NextBatch();

	glm::vec2 const* texCoords{ subtex->GetTexCoords() };
	std::shared_ptr<Texture> tex = subtex->GetTexture();

	float texIdx = 0.f;

	//don't need to iterate all 32 slots every time
	for (unsigned int i{ 1 }; i < mData.texUnitIdx; ++i) {
		if (*mData.texUnits[i].get() == *tex.get()) { //check if the particular texture has already been set
			texIdx = static_cast<float>(i);
			break;
		}
	}

	if (texIdx == 0.f) {
		texIdx = static_cast<float>(mData.texUnitIdx);
		mData.texUnits[mData.texUnitIdx] = tex;
		++mData.texUnitIdx;
	}

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };

	glm::mat4 transformMtx{ translateMtx * rotateMtx * scaleMtx };

	for (size_t i{}; i < 4; ++i) 
		SetQuadBufferData(transformMtx * mData.quadVtxPos[i], scale, tint, texCoords[i], texIdx, entity);

	mData.quadIdxCount += 6;

	//++mData.stats.quadCount;
}

/*  _________________________________________________________________________ */
/*! DrawSprite (Overloaded)

@param transform
Transform object containing position, scale, and rotation information.

@param subtex
A shared pointer to the SubTexture object which contains the texture information
for the sprite.

@param tint
Color tint to be applied to the sprite.

@return none.

This function is an overloaded version of the DrawSprite function. It uses the
transform object to extract position, scale, and rotation information and then
calls the primary DrawSprite function.
*/
void Renderer::DrawSprite(Transform const& transform, std::shared_ptr<SubTexture> const& subtex, glm::vec4 const& tint, int entity) {
	DrawSprite(transform.position, transform.scale, subtex, tint, transform.rotation.z, entity);
}

/*  _________________________________________________________________________ */
/*! DrawLine

@param p0
Starting point of the line.

@param p1
Ending point of the line.

@param clr
Color of the line.

@return none.

This function is responsible for drawing a line between two points. It checks if
the maximum vertices have been reached and if so, it moves to the next batch. The
function sets the buffer data for the line and increments the line vertex count.
*/
void Renderer::DrawLine(glm::vec3 const& p0, glm::vec3 const& p1, glm::vec4 const& clr) {
	if (mData.lineVtxCount >= RendererData::cMaxVertices)
		NextBatch();

	SetLineBufferData(p0, clr);
	SetLineBufferData(p1, clr);

	mData.lineVtxCount += 2;

	//++mData.stats.lineCount;
}

/*  _________________________________________________________________________ */
/*! DrawLineRect

@param pos
Position of the rectangle.

@param scale
Scale of the rectangle.

@param clr
Color of the rectangle's border.

@return none.

This function draws a rectangle using lines. It calculates the four corners of
the rectangle and then draws lines between them to form the rectangle.
*/
void Renderer::DrawLineRect(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float rot) {
	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 rotateMtx{ glm::rotate(glm::mat4{ 1.f }, glm::radians(rot), {0.f, 0.f, 1.f}) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };
	glm::mat4 transformMtx{ translateMtx * rotateMtx * scaleMtx };

	//DrawLine(p0, p1, clr);
	//DrawLine(p1, p2, clr);
	//DrawLine(p2, p3, clr);
	//DrawLine(p3, p0, clr);

	glm::vec3 lineVertices[4]{};
	for (size_t i{}; i < 4; ++i)
		lineVertices[i] = transformMtx * mData.quadVtxPos[i];

	DrawLine(lineVertices[0], lineVertices[1], clr);
	DrawLine(lineVertices[1], lineVertices[2], clr);
	DrawLine(lineVertices[2], lineVertices[3], clr);
	DrawLine(lineVertices[3], lineVertices[0], clr);
   
}

/*  _________________________________________________________________________ */
/*! DrawCircle

@param pos
Position of the center of the circle.

@param scale
Scale of the circle in x and y dimensions.

@param clr
Color of the circle's border.

@param thickness
Thickness of the circle's border.

@param fade
Fade effect for the circle's border.

@return none.

This function draws a circle with specified parameters. It calculates the
transformation for the circle using its position and scale. The circle's appearance
is further defined by its color, border thickness, and fade effect.
*/
void Renderer::DrawCircle(glm::vec3 const& pos, glm::vec2 const& scale, glm::vec4 const& clr, float thickness, float fade) {

	//TODO Implement circles
	//if (mData.quadIdxCount >= RendererData::cMaxIndices)
	//	NextBatch();

	glm::mat4 translateMtx{ glm::translate(glm::mat4{ 1.f }, pos) };
	glm::mat4 scaleMtx{ glm::scale(glm::mat4{ 1.f }, { scale.x, scale.y, 1.f }) };
	glm::mat4 transformMtx{ translateMtx * scaleMtx };

	for (size_t i{}; i < 4; ++i) {
		mData.circleBufferPtr->worldPos = transformMtx * mData.quadVtxPos[i];
		mData.circleBufferPtr->localPos = mData.quadVtxPos[i] * 2.f; //Using the Quad Mesh to get -1.f, to 1.f
		mData.circleBufferPtr->clr = clr;
		mData.circleBufferPtr->thickness = thickness;
		mData.circleBufferPtr->fade = fade;
		++mData.circleBufferPtr;
	}

	mData.circleIdxCount += 6;
}

/*  _________________________________________________________________________ */
/*! FlushBatch

@return none.

This function flushes the current batch of data to the GPU. It checks if
there are any quads or lines to be drawn and then sends the data to the GPU.
The function also binds the necessary textures and shaders before drawing.
*/
void Renderer::FlushBatch() {
	if (mData.quadIdxCount) {
		ptrdiff_t difference{ reinterpret_cast<unsigned char*>(mData.quadBufferPtr)
							- reinterpret_cast<unsigned char*>(mData.quadBuffer.data()) };

		//how many elements it takes up in terms of bytes
		unsigned int dataSize = static_cast<unsigned int>(difference);

		mData.quadVertexBuffer->SetData(mData.quadBuffer.data(), dataSize);

		//Bind all the textures that has been set
		for (unsigned int i{}; i < mData.texUnitIdx; ++i) {
			mData.texUnits[i]->Bind(i);
		}
		mData.texShader->Use();
		DrawIndexed(mData.quadVertexArray, mData.quadIdxCount);

		//++mData.stats.drawCalls;
	}

	if (mData.lineVtxCount) {
		ptrdiff_t difference{ reinterpret_cast<unsigned char*>(mData.lineBufferPtr)
							- reinterpret_cast<unsigned char*>(mData.lineBuffer.data()) };

		//how many elements it takes up in terms of bytes
		unsigned int dataSize = static_cast<unsigned int>(difference);
		mData.lineVertexBuffer->SetData(mData.lineBuffer.data(), dataSize);

		mData.lineShader->Use();
		DrawLineArray(mData.lineVertexArray, mData.lineVtxCount);

		//++mData.stats.drawCalls;
	}

	if (mData.circleIdxCount) {
		ptrdiff_t difference{ reinterpret_cast<unsigned char*>(mData.circleBufferPtr)
							- reinterpret_cast<unsigned char*>(mData.circleBuffer.data()) };

		unsigned int dataSize = static_cast<unsigned int>(difference);
		mData.circleVertexBuffer->SetData(mData.circleBuffer.data(), dataSize);

		mData.circleShader->Use();
		DrawIndexed(mData.circleVertexArray, mData.circleIdxCount);
	}
}

/*  _________________________________________________________________________ */
/*! BeginBatch

@return none.

This function initializes a new batch by resetting the quad index count, line
vertex count, and texture unit index. It also sets the buffer pointers to the
start of the buffer data.
*/
void Renderer::BeginBatch() {
	mData.quadIdxCount = 0;
	mData.quadBufferPtr = mData.quadBuffer.data();

	mData.lineVtxCount = 0;
	mData.lineBufferPtr = mData.lineBuffer.data();

	mData.circleIdxCount = 0;
	mData.circleBufferPtr = mData.circleBuffer.data();

	mData.texUnitIdx = 1;
}

/*  _________________________________________________________________________ */
/*! NextBatch

@return none.

This function moves to the next batch by first flushing the current batch and then
beginning a new one. It also provides debug information about the renderer's statistics.
*/
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

/*  _________________________________________________________________________ */
/*! SetClearColor

@param color
The color to set as the clear color.

@return none.

This function sets the clear color for the OpenGL context.
*/
void Renderer::SetClearColor(glm::vec4 const& color) {
	//TODO might change to clearColor member variable
	glClearColor(color.r, color.g, color.b, color.a);
}

/*  _________________________________________________________________________ */
/*! ClearColor

@return none.

This function clears the color buffer using the previously set clear color.
*/
void Renderer::ClearColor() {
	glClear(GL_COLOR_BUFFER_BIT);
}

/*  _________________________________________________________________________ */
/*! ClearDepth

@return none.

This function clears the depth buffer.
*/
void Renderer::ClearDepth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

/*  _________________________________________________________________________ */
/*! DrawIndexed

@param vao
A shared pointer to the VertexArray object.

@param idxCount (optional)
The number of indices to draw. If not provided, the function will use the count 
from the element buffer of the VertexArray.

@return none.

This function binds the provided VertexArray and then issues a draw call to OpenGL
to draw the indexed geometry.
*/
void Renderer::DrawIndexed(std::shared_ptr<VertexArray> const& vao, unsigned int idxCount) {
	vao->Bind();
	unsigned int count = idxCount ? idxCount : vao->GetElementBuffer()->GetCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

/*  _________________________________________________________________________ */
/*! DrawLineArray

@param vao
A shared pointer to the VertexArray object.

@param vtxCount
The number of vertices to draw.

@return none.

This function binds the provided VertexArray and then issues a draw call to OpenGL
to draw the line geometry.
*/
void Renderer::DrawLineArray(std::shared_ptr<VertexArray> const& vao, unsigned int vtxCount) {
	vao->Bind();
	glDrawArrays(GL_LINES, 0, vtxCount);
}

/*  _________________________________________________________________________ */
/*! ResetStats

@return none.

This function resets the renderer's statistics to zero.
*/
void Renderer::ResetStats() {
	memset(&mData.stats, 0, sizeof(Statistics));
}

/*  _________________________________________________________________________ */
/*! GetStats

@return
Returns the current statistics of the renderer.

This function retrieves the current statistics of the renderer.
*/
Statistics Renderer::GetStats() {
	return mData.stats;
}