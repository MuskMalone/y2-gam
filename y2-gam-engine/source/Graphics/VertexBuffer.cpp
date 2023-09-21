#include "Graphics/VertexBuffer.hpp"
#include "glad/glad.h"

VertexBuffer::VertexBuffer(unsigned int size) {
	glCreateBuffers(1, &mVboHdl);
	VertexBuffer::Bind();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

}

VertexBuffer::VertexBuffer(float* vertices, unsigned int size) {
	glCreateBuffers(1, &mVboHdl);
	VertexBuffer::Bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

}

VertexBuffer::~VertexBuffer() {
	glDeleteBuffers(1, &mVboHdl);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(unsigned int size) {
	return std::make_shared<VertexBuffer>(size);
}

std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, unsigned int size) {
	return std::make_shared<VertexBuffer>(vertices, size);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, mVboHdl);
}

void VertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetLayout(BufferLayout& layout) {
	mLayout = layout;
}

BufferLayout const& VertexBuffer::GetLayout() const { return mLayout; }

void VertexBuffer::SetData(void* const data, unsigned int size) {
	glBindBuffer(GL_ARRAY_BUFFER, mVboHdl);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}