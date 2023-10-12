/******************************************************************************/
/*!
\par        Image Engine
\file       Framebuffer.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 15, 2023

\brief      Implementation file for the Framebuffer class.

			Provides the definitions for the member functions of the Framebuffer
			class, which represents an OpenGL framebuffer object. It includes
			functionalities for creating, binding, unbinding, and accessing
			framebuffer properties and attachments.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "../include/pch.hpp"

#include "Graphics/Framebuffer.hpp"

/*  _________________________________________________________________________ */
/*! Framebuffer

@param props
Properties of the framebuffer.

This constructor initializes the Framebuffer using the provided properties and
calls the Recreate function.
*/
Framebuffer::Framebuffer(FramebufferProps const& props) : mProps{ props } {
	Recreate();
}

/*  _________________________________________________________________________ */
/*! ~Framebuffer

Destructor that cleans up the framebuffer resources by deleting the framebuffer
object.
*/
Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &mFboHdl);
	glDeleteTextures(1, &mColorAttachment);
	glDeleteTextures(1, &mDepthAttachment);
}

/*  _________________________________________________________________________ */
/*! Recreate

This function creates a new framebuffer object and sets up its color and depth
attachments. It also checks if the framebuffer is complete and logs the result.
*/
void Framebuffer::Recreate() {

	if (mFboHdl) {
		glDeleteFramebuffers(1, &mFboHdl);
		glDeleteTextures(1, &mColorAttachment);
		glDeleteTextures(1, &mDepthAttachment);
	}

	glCreateFramebuffers(1, &mFboHdl);
	glBindFramebuffer(GL_FRAMEBUFFER, mFboHdl);

	glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
	glBindTexture(GL_TEXTURE_2D, mColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mProps.width, mProps.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, mDepthAttachment);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mProps.width, mProps.height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "ERROR: Framebuffer is INCOMPLETE!", __FUNCTION__);
	else LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Framebuffer is complete!", __FUNCTION__);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*  _________________________________________________________________________ */
/*! Bind

Binds the framebuffer, making it the current target for rendering operations.
*/
void Framebuffer::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, mFboHdl);
}

/*  _________________________________________________________________________ */
/*! Unbind

Unbinds the framebuffer, reverting the rendering target to the default framebuffer.
*/
void Framebuffer::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*  _________________________________________________________________________ */
/*! Resize

@param width The new width for the framebuffer
@param height The new height for the framebuffer

Resizes the framebuffer to the specified width and height.

*/
void Framebuffer::Resize(unsigned int width, unsigned int height){
	mProps.width = width;
	mProps.height = height;

	Recreate();
}

/*  _________________________________________________________________________ */
/*! GetColorAttachmentID

@return
ID of the color attachment of the framebuffer.

This function returns the ID of the color attachment associated with the framebuffer.
*/
unsigned int Framebuffer::GetColorAttachmentID() const { return mColorAttachment; }

/*  _________________________________________________________________________ */
/*! GetFramebufferProps

@return
Properties of the framebuffer.

This function returns the properties associated with the framebuffer.
*/
FramebufferProps const& Framebuffer::GetFramebufferProps() const { return mProps; }

/*  _________________________________________________________________________ */
/*! Create

@param props
Properties for the new framebuffer.

@return
Shared pointer to the created Framebuffer.

This static function creates and returns a shared pointer to a Framebuffer
using the provided properties.
*/
std::shared_ptr<Framebuffer> Framebuffer::Create(FramebufferProps const& props) {
	return std::make_shared<Framebuffer>(props);
}