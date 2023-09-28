/******************************************************************************/
/*!
\par        Image Engine
\file       VertexArray.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Header file for the VertexArray class.

			Contains the declarations for the VertexArray class which is responsible
			for managing vertex array objects (VAOs) in OpenGL. The class provides
			functions for creating, binding, and setting up VAOs with vertex buffers
			and element buffers.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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