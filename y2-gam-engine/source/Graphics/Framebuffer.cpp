#include "Graphics/Framebuffer.hpp"
#include "glad/glad.h"
#include <iostream>

Framebuffer::Framebuffer(FramebufferProps const& props) : mProps{props} {
	Invalidate();
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &mFboHdl);
}

void Framebuffer::Invalidate() {
	glCreateFramebuffers(1, &mFboHdl);
	glBindFramebuffer(GL_TEXTURE_2D, mFboHdl);

	glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
	glBindTexture(GL_TEXTURE_2D, mColorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mProps.width, mProps.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//TODO MAY ADD CLAMP

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

	glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
	glBindTexture(GL_TEXTURE_2D, mDepthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, mProps.width, mProps.height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is complete." << std::endl;
	}
	else {
		std::cout << "ERROR: Framebuffer is incomplete." << std::endl;
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::shared_ptr<Framebuffer> Framebuffer::Create(FramebufferProps const& props) {
	return std::make_shared<Framebuffer>(props);
}

unsigned int Framebuffer::GetColorAttachmentID() const {
	return mColorAttachment;
}

FramebufferProps const& Framebuffer::GetProps() const {
	return mProps;
}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, mFboHdl);
}

void Framebuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
