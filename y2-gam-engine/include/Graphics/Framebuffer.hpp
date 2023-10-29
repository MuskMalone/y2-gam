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

enum class FramebufferTexFormat {
	NONE = 0,

	RGBA8,
	RED_INTEGER,
	DEPTH24STENCIL8,

	//default
	DEPTH = DEPTH24STENCIL8
};

struct FramebufferTexProps {
	FramebufferTexProps() = default;
	FramebufferTexProps(FramebufferTexFormat fmt) : TexFormat{fmt} {}

	FramebufferTexFormat TexFormat = FramebufferTexFormat::NONE;
};

struct FramebufferAttachmentProps {
	FramebufferAttachmentProps() = default;
	FramebufferAttachmentProps(std::initializer_list<FramebufferTexProps> ats) : attachments{ ats } {}

	std::vector<FramebufferTexProps> attachments;
};

struct FramebufferProps {
	unsigned int width{}, height{};
	unsigned int samples = 1;
	FramebufferAttachmentProps attachments;
};

class Framebuffer {
public:
	Framebuffer(FramebufferProps const&);
	~Framebuffer();

	void Recreate();

	void Bind() const;
	void Unbind() const;

	void Resize(unsigned int width, unsigned int height);
	int ReadPixel(unsigned int attachIdx, int x, int y);

	void ClearAttachmentInt(unsigned int attachIdx, int val);

	unsigned int GetColorAttachmentID(unsigned int index = 0) const;
	FramebufferProps const& GetFramebufferProps() const;

	static std::shared_ptr<Framebuffer> Create(FramebufferProps const& props);
private:
	unsigned int mFboHdl{};
	//unsigned int mColorAttachment{};
	//unsigned int mDepthAttachment{};
	FramebufferProps mProps;

	std::vector<FramebufferTexProps> mColorAttachmentProps;
	FramebufferTexProps mDepthAttachmentProp = FramebufferTexFormat::NONE;

	std::vector<unsigned int> mColorAttachments;
	unsigned int mDepthAttachment{};
};