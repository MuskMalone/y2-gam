#pragma once
#include <memory>

class ElementBuffer {
public:
	ElementBuffer(unsigned int* indices, unsigned int count);
	~ElementBuffer();

	static std::shared_ptr<ElementBuffer> Create(unsigned int* indices, unsigned int count);

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const;
private:
	unsigned int mEboHdl;
	unsigned int mIdxCount;
};