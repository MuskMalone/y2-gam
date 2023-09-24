#include "Graphics/Texture.hpp"

#include <stdexcept>
#include <glad/glad.h>
#include <stb_image.h>

Texture::Texture() : mWidth{ 0 }, mHeight{ 0 }, mTexHdl{ 0 } {}

Texture::Texture(std::string const& path)
	: mPath{ path } {

	// Load image using stb_image
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	//unsigned char* data{ stbi_load(path.c_str(), &width, &height, &channels, STBI_default) };
	if (!data) {
		throw std::runtime_error("Failed to load texture: " + path);
	}

	mWidth = static_cast<unsigned int>(width);
	mHeight = static_cast<unsigned int>(height);

	glCreateTextures(GL_TEXTURE_2D, 1, &mTexHdl);
	// allocate GPU storage for texture image data loaded from file
	glTextureStorage2D(mTexHdl, 1, GL_RGBA8, mWidth, mHeight);


	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(mTexHdl, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mTexHdl, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// copy image data from client memory to GPU texture buffer memory
	glTextureSubImage2D(mTexHdl, 0, 0, 0, mWidth, mHeight,
		GL_RGBA, GL_UNSIGNED_BYTE, data);
	// client memory not required since image is buffered in GPU memory
	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);
}

Texture::Texture(unsigned int width, unsigned int height) :mWidth{ width }, mHeight{ height } {

	//TODO might add more parameters

	glCreateTextures(GL_TEXTURE_2D, 1, &mTexHdl);
	// allocate GPU storage for texture image data loaded from file
	glTextureStorage2D(mTexHdl, 1, GL_RGBA8, mWidth, mHeight);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(mTexHdl, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mTexHdl, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

// For Font Glyphs
Texture::Texture(unsigned int width, unsigned int height, const void* data) : mWidth{ width }, mHeight{ height }, mTexHdl{} {
	/*
	if (data == nullptr) {
		std::cout << "Invalid Data!" << "\n";
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	// Create a texture object
	glCreateTextures(GL_TEXTURE_2D, 1, &mTexHdl);

	// Allocate storage for the texture
	glTextureStorage2D(mTexHdl, 1, GL_RED, static_cast<GLsizei>(mWidth), static_cast<GLsizei>(mHeight));

	// Upload texture data
	glTextureSubImage2D(mTexHdl, 0, 0, 0, static_cast<GLsizei>(mWidth), static_cast<GLsizei>(mHeight), GL_RED, GL_UNSIGNED_BYTE, data);

	// Set texture parameters
	glTextureParameteri(mTexHdl, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(mTexHdl, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(mTexHdl, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(mTexHdl, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // reset byte-alignment
	*/

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &mTexHdl);
	glBindTexture(GL_TEXTURE_2D, mTexHdl);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		mWidth,
		mHeight,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data
	);
	// set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // reset byte-alignment
}

Texture::~Texture() {
	glDeleteTextures(1, &mTexHdl);
}

unsigned int Texture::GetWidth() const {
	return mWidth;
}
unsigned int Texture::GetHeight() const {
	return mHeight;
}

unsigned int Texture::GetTexHdl() const {
	return mTexHdl;
}

//unsigned int Texture::GetTexHandle() const {
//	return mTexHdl;
//}

void Texture::SetData(void* data) {
	//TODO may add format to texture member variables
	glTextureSubImage2D(mTexHdl, 0, 0, 0, mWidth, mHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::Bind(unsigned int texUnit) const {
	glBindTextureUnit(texUnit, mTexHdl);
}

void Texture::Unbind(unsigned int texUnit) const {
	glBindTextureUnit(texUnit, 0);
}

bool Texture::operator==(Texture const& rhs) const {
	return mTexHdl == rhs.mTexHdl;
}