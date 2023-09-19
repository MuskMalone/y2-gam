#pragma once

#include <memory>
#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"

class VertexArray {
public:
	VertexArray();
	~VertexArray();

	static std::shared_ptr<VertexArray> Create();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vbo);
	void SetElementBuffer(const std::shared_ptr<ElementBuffer>& ebo);

	std::vector<std::shared_ptr<VertexBuffer>> const& GetVertexBuffers() const { return mVbos; }
	std::shared_ptr<ElementBuffer> const& GetElementBuffer() const { return mEbo; }

private:
	unsigned int vaoHdl;
	std::vector<std::shared_ptr<VertexBuffer>> mVbos;
	std::shared_ptr<ElementBuffer> mEbo;
};