#pragma once
class ElementBuffer {
public:
	ElementBuffer(unsigned int* indices, unsigned int count);
	~ElementBuffer();

	void Bind() const;
	void Unbind() const;
	unsigned int GetCount() const;
private:
	unsigned int mIboHdl;
	unsigned int mIdxCount;
};