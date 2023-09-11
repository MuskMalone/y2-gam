#pragma once

#include <memory>
#include "VertexBuffer.h"
#include "ElementBuffer.h"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo);
	void SetElementBuffer(const std::shared_ptr<ElementBuffer>& ebo);

	std::vector<std::shared_ptr<VertexBuffer>> const& GetVertexBuffers() const { return mVbos; }
	std::shared_ptr<ElementBuffer> const& GetElementBuffer() const { return mEbo; }
private:
	unsigned int vaoHdl;
	//change to refs in future maybe
	std::vector<std::shared_ptr<VertexBuffer>> mVbos;
	std::shared_ptr<ElementBuffer> mEbo;
};