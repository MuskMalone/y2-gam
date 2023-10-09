/******************************************************************************/
/*!
\par        Image Engine
\file       ElementBuffer.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Header file for the ElementBuffer class.

			Declares the ElementBuffer class which manages element buffer objects
            (EBOs) in OpenGL. It provides functionalities for creating, binding,
            unbinding, and setting data for EBOs.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
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