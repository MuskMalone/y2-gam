/******************************************************************************/
/*!
\par        Image Engine
\file       Texture.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Header file for the Texture class.

			Declares the Texture class which manages textures in OpenGL.
			It provides functionalities for creating, binding, unbinding,
			and setting data for textures. It also supports texture operations
			for font glyphs.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

class Texture {
public:

	static std::shared_ptr<Texture> Create(std::string const& path);
	//Temp default constructor
	Texture();
	Texture(std::string const& path);
	Texture(unsigned int width, unsigned int height);

	// For Font Glyphs
	Texture(unsigned int width, unsigned int height, const void* data);

	~Texture();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	unsigned int GetTexHdl() const;

	void SetData(void* data);

	void Bind(unsigned int texUnit = 0) const;
	void Unbind(unsigned int texUnit = 0) const;

	bool operator==(Texture const& rhs) const;
private:
	std::string mPath;
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mTexHdl;
};