#include <memory>
#include "VertexArray.h"
#include "glad/glad.h"

static GLenum AttributeToGLType(AttributeType type) {
	switch (type) {
		//dunno if fall through behaviour is better
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

VertexArray::VertexArray() {
	glCreateVertexArrays(1, &vaoHdl);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoHdl);
}

void VertexArray::Bind() const {
	glBindVertexArray(vaoHdl);
}
void VertexArray::Unbind() const {
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo) {
	glBindVertexArray(vaoHdl);
	vbo->Bind();
	unsigned int attrib_idx{};
	const auto& layout{ vbo->GetLayout() };
	for (auto const& elem : layout) {
		glEnableVertexAttribArray(attrib_idx);
		glVertexAttribPointer(attrib_idx,
			elem.GetComponentCount(),
			AttributeToGLType(elem.type),
			elem.isNormalized ? GL_TRUE : GL_FALSE,
			vbo->GetLayout().GetStride(),
			reinterpret_cast<const void*>(
				static_cast<uintptr_t>(elem.offset)));
		++attrib_idx;
	}
	mVbos.push_back(vbo);
}

void VertexArray::SetElementBuffer(const std::shared_ptr<ElementBuffer>& ebo) {
	glBindVertexArray(vaoHdl);
	ebo->Bind();
	mEbo = ebo;
}