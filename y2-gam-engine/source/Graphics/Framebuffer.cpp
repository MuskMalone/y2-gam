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

namespace FbUtils{

	static GLenum TextureTarget(bool multisample) {
		return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static bool IsDepthFormat(FramebufferTexFormat fmt) {
		switch (fmt) {
		case FramebufferTexFormat::DEPTH24STENCIL8: return true;

		}
		return false;
	}

	static void CreateTextures(bool multisample, unsigned int* id, unsigned int count) {
		glCreateTextures(TextureTarget(multisample), count, id);
	}

	static void BindTexture(bool multisample, unsigned int id) {
		glBindTexture(TextureTarget(multisample), id);
	}

	static void AttachColorTexture(unsigned int id, int samples, GLenum intFmt, GLenum fmt, unsigned int width, unsigned int height, int index) {
		bool multisample{ samples > 1 };
		if (multisample) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, intFmt, width, height, GL_FALSE); //TODO may remove
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, intFmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); //TODO may change
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisample), id, 0); //attach framebuffer at correct index
	}

	static void AttachDepthTexture(unsigned int id, int samples, GLenum intFmt, GLenum attachType, unsigned int width, unsigned int height) {
		bool multisample{ samples > 1 };
		if (multisample) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, intFmt, width, height, GL_FALSE); //TODO may remove
		}
		else {
			glTexStorage2D(GL_TEXTURE_2D, 1, intFmt, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); //TODO may change
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, TextureTarget(multisample), id, 0);
	}
}


/*  _________________________________________________________________________ */
/*! Framebuffer

@param props
Properties of the framebuffer.

This constructor initializes the Framebuffer using the provided properties and
calls the Recreate function.
*/
Framebuffer::Framebuffer(FramebufferProps const& props) : mProps{ props } {

	for (auto const& prop : mProps.attachments.attachments) {
		if (!FbUtils::IsDepthFormat(prop.TexFormat))
			mColorAttachmentProps.emplace_back(prop);
		else
			mDepthAttachmentProp = prop;
	}

	Recreate();
}
/*  _________________________________________________________________________ */
/*! ~Framebuffer

Destructor that cleans up the framebuffer resources by deleting the framebuffer
object.
*/
Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &mFboHdl);
	glDeleteTextures(static_cast<GLsizei>(mColorAttachments.size()), mColorAttachments.data());
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
		glDeleteTextures(static_cast<GLsizei>(mColorAttachments.size()), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);

		mColorAttachments.clear();
		mDepthAttachment = 0;
	}

	glCreateFramebuffers(1, &mFboHdl);
	glBindFramebuffer(GL_FRAMEBUFFER, mFboHdl);

	bool multisample = mProps.samples > 1;

	if (mColorAttachmentProps.size()) {
		mColorAttachments.resize(mColorAttachmentProps.size());
		FbUtils::CreateTextures(multisample, mColorAttachments.data(), static_cast<unsigned int>(mColorAttachments.size()) );

		for (size_t i{}; i < mColorAttachments.size(); ++i) {
			FbUtils::BindTexture(multisample, mColorAttachments[i]);
			switch (mColorAttachmentProps[i].TexFormat) {
			case FramebufferTexFormat::RGBA8:
				FbUtils::AttachColorTexture(mColorAttachments[i], mProps.samples, GL_RGBA8, GL_RGBA, mProps.width, mProps.height, static_cast<int>(i));
				break;
			case FramebufferTexFormat::RED_INTEGER:
				FbUtils::AttachColorTexture(mColorAttachments[i], mProps.samples, GL_R32I, GL_RED_INTEGER, mProps.width, mProps.height, static_cast<int>(i));
				break;
			}
		}
	}

	if (mDepthAttachmentProp.TexFormat != FramebufferTexFormat::NONE) {
		FbUtils::CreateTextures(multisample, &mDepthAttachment, 1);
		FbUtils::BindTexture(multisample, mDepthAttachment);
		switch (mDepthAttachmentProp.TexFormat) {
		case FramebufferTexFormat::DEPTH24STENCIL8:
			FbUtils::AttachDepthTexture(mDepthAttachment, mProps.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mProps.width, mProps.height);
			break;
		}
	}

	if (mColorAttachments.size() > 1) {
		if (mColorAttachments.size() > 4) {
			LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "TOO MANY ATTACHMENTS!", __FUNCTION__);
			throw std::runtime_error("TOO MANY ATTACHMENTS");
		}

		GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(static_cast<GLsizei>(mColorAttachments.size()), buffers);
	}
	else if (mColorAttachments.empty()) {
		glDrawBuffer(GL_NONE); // only depth
	}
	
	//glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
	//glBindTexture(GL_TEXTURE_2D, mColorAttachment);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mProps.width, mProps.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

	//glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
	//glBindTexture(GL_TEXTURE_2D, mDepthAttachment);

	//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mProps.width, mProps.height);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

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

int Framebuffer::ReadPixel(unsigned int attachIdx, int x, int y) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachIdx);
	int pixels;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixels);
	return pixels;
}

void Framebuffer::ClearAttachmentInt(unsigned int attachIdx, int val) {
	FramebufferTexProps& prop { mColorAttachmentProps[attachIdx]};

	glClearTexImage(mColorAttachments[attachIdx], 0, GL_RED_INTEGER, GL_INT, &val);
}
/*  _________________________________________________________________________ */
/*! GetColorAttachmentID

@return
ID of the color attachment of the framebuffer.

This function returns the ID of the color attachment associated with the framebuffer.
*/
unsigned int Framebuffer::GetColorAttachmentID(unsigned int index) const { if (index >= mColorAttachments.size()) { throw std::runtime_error("out of bounds"); } return mColorAttachments[index]; }

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