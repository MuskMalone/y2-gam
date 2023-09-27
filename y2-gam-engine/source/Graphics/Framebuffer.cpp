#include "Graphics/Framebuffer.hpp"
#include <glad/glad.h>
#include <iostream>
#include "Logging/LoggingSystem.hpp"
#include "Logging/backward.hpp"
Framebuffer::Framebuffer(FramebufferProps const& props) : mProps{ props } {
	Invalidate();
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &mFboHdl);
}

void Framebuffer::Invalidate() {
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
		//std::cout << "ERROR: Framebuffer is INCOMPLETE!" << std::endl;
		LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "ERROR: Framebuffer is INCOMPLETE!", __FUNCTION__);
	else LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Framebuffer is complete!", __FUNCTION__);
	//else std::cout << "Framebuffer is complete!" << std::endl;


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, mFboHdl);
}

void Framebuffer::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Framebuffer::GetColorAttachmentID() const { return mColorAttachment; }

FramebufferProps const& Framebuffer::GetFramebufferProps() const { return mProps; }

std::shared_ptr<Framebuffer> Framebuffer::Create(FramebufferProps const& props) {
	return std::make_shared<Framebuffer>(props);
}