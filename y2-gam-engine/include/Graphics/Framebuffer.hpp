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