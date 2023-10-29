/******************************************************************************/
/*!
\par        Image Engine
\file       VertexArray.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Implementation file for the VertexArray class.

			Contains the declarations for the VertexArray class which is responsible
			for managing vertex array objects (VAOs) in OpenGL. The class provides
			functions for creating, binding, and setting up VAOs with vertex buffers
			and element buffers.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Graphics/VertexArray.hpp"

static GLenum AttributeToGLType(AttributeType type) {
	switch (type) {
	case AttributeType::MAT3:
	case AttributeType::FLOAT:
	case AttributeType::VEC2:
	case AttributeType::VEC3:
	case AttributeType::VEC4:
	case AttributeType::MAT4:
		return GL_FLOAT;
	case AttributeType::INT:
	case AttributeType::IVEC2:
	case AttributeType::IVEC3:
	case AttributeType::IVEC4:
		return GL_INT;
	case AttributeType::BOOL:
		return GL_BOOL;
	}

	std::cout << "Unknown AttributeType!\n";
	//@TODO EXIT PROGRAM/ ASSERT
	return 0;
}

/*  _________________________________________________________________________ */
/*! VertexArray

Default constructor for the VertexArray class. Initializes the VAO handle.
*/
VertexArray::VertexArray() {
	glCreateVertexArrays(1, &vaoHdl);
}

/*  _________________________________________________________________________ */
/*! ~VertexArray

Destructor for the VertexArray class. Deletes the VAO handle.
*/
VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoHdl);
}

/*  _________________________________________________________________________ */
/*! Create

@return
A shared pointer to a new VertexArray object.

This function creates a new VertexArray object and returns a shared pointer to it.
*/
std::shared_ptr<VertexArray> VertexArray::Create() {
	return std::make_shared<VertexArray>();
}

/*  _________________________________________________________________________ */
/*! Bind

Binds the VAO for rendering.
*/
void VertexArray::Bind() const {
	glBindVertexArray(vaoHdl);
}

/*  _________________________________________________________________________ */
/*! Unbind

Unbinds the VAO.
*/
void VertexArray::Unbind() const {
	glBindVertexArray(0);
}

/*  _________________________________________________________________________ */
/*! AddVertexBuffer

@param vbo
A shared pointer to the VertexBuffer object to be added to the VAO.

This function binds the VAO and the provided VertexBuffer, sets up the vertex attributes
according to the layout of the VertexBuffer, and adds the VertexBuffer to the list of VBOs.
*/
void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) {
	glBindVertexArray(vaoHdl);
	vbo->Bind();
	unsigned int attribIdx{};
	auto const& layout{ vbo->GetLayout() };
	for (auto const& elem : layout) {
		switch (elem.type) {
		case AttributeType::FLOAT:
		case AttributeType::VEC2:
		case AttributeType::VEC3:
		case AttributeType::VEC4:
			glEnableVertexAttribArray(attribIdx);
			glVertexAttribPointer(attribIdx,
				elem.GetComponentCount(),
				AttributeToGLType(elem.type),
				elem.isNormalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				reinterpret_cast<const void*>(
					static_cast<uintptr_t>(elem.offset)));
			++attribIdx;
			break;
		case AttributeType::INT:
		case AttributeType::IVEC2:
		case AttributeType::IVEC3:
		case AttributeType::IVEC4:
		case AttributeType::BOOL:
			glEnableVertexAttribArray(attribIdx);
			glVertexAttribIPointer(attribIdx,
				elem.GetComponentCount(),
				AttributeToGLType(elem.type),
				layout.GetStride(),
				reinterpret_cast<const void*>(
					static_cast<uintptr_t>(elem.offset)));
			++attribIdx;
			break;
		}

	}
	mVbos.push_back(vbo);
}

/*  _________________________________________________________________________ */
/*! SetElementBuffer

@param ebo
A shared pointer to the ElementBuffer object to be set for the VAO.

This function binds the VAO and the provided ElementBuffer and sets the ElementBuffer for the VAO.
*/
void VertexArray::SetElementBuffer(const std::shared_ptr<ElementBuffer>& ebo) {
	glBindVertexArray(vaoHdl);
	ebo->Bind();
	mEbo = ebo;
}