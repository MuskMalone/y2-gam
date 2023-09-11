#include "ElementBuffer.h"
#include "glad/glad.h"

ElementBuffer::ElementBuffer(unsigned int* indices, unsigned int count) {
	//change this in future maybe
	glCreateBuffers(1, &mIboHdl);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboHdl);
	//ElementBuffer::Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	mIdxCount = count;

}

ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &mIboHdl);
}

void ElementBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIboHdl);
}

void ElementBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

unsigned int ElementBuffer::GetCount() const {
	return mIdxCount;
}