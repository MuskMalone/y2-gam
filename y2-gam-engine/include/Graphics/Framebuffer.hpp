#pragma once
#include "memory"

struct FramebufferProps {
	unsigned int width, height;
	unsigned int samples{ 1 };

	bool swapChainTarget{ false }; //rendering to screen or not
};

class Framebuffer {
public:
	Framebuffer(FramebufferProps const& props);
	~Framebuffer();

	static std::shared_ptr<Framebuffer> Create(FramebufferProps const& props);

	unsigned int GetColorAttachmentID() const;
	FramebufferProps const& GetProps() const;

	void Invalidate();

	void Bind();
	void Unbind();

private:
	unsigned int mFboHdl{};
	unsigned int mColorAttachment{};
	unsigned int mDepthAttachment{};
	FramebufferProps mProps;
};