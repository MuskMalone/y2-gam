/******************************************************************************/
/*!
\par        Image Engine
\file       Framebuffer.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 15, 2023

\brief      Header file for the Framebuffer class.

			Defines the interface for the Framebuffer class which represents
			an OpenGL framebuffer object. It provides functionalities for
			creating, binding, unbinding, and accessing framebuffer properties
			and attachments.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <memory>

struct FramebufferProps {
	unsigned int width, height;

};

class Framebuffer {
public:
	Framebuffer(FramebufferProps const&);
	~Framebuffer();

	void Invalidate();

	void Bind() const;
	void Unbind() const;

	unsigned int GetColorAttachmentID() const;
	FramebufferProps const& GetFramebufferProps() const;

	static std::shared_ptr<Framebuffer> Create(FramebufferProps const& props);
private:
	unsigned int mFboHdl;
	unsigned int mColorAttachment;
	unsigned int mDepthAttachment;
	FramebufferProps mProps;
};