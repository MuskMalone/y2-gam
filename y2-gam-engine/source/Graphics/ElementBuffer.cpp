#include "Graphics/ElementBuffer.hpp"
#include "glad/glad.h"

ElementBuffer::ElementBuffer(unsigned int* indices, unsigned int count) {
	glCreateBuffers(1, &mEboHdl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboHdl);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	mIdxCount = count;

}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &mEboHdl);
}

std::shared_ptr<ElementBuffer> ElementBuffer::Create(unsigned int* indices, unsigned int count) {
	return std::make_shared<ElementBuffer>(indices, count);
}

void ElementBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboHdl);
}

void ElementBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int ElementBuffer::GetCount() const {
	return mIdxCount;
}