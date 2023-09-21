#pragma once
#include "memory"

struct FramebufferProps {
	unsigned int width, height;
	unsigned int samples = 1;
};

class Framebuffer {
public:
	FramebufferProps const& GetProps() const;

	static std::shared_ptr<Framebuffer> Create(FramebufferProps const& props);
};