#pragma once
#include <string>

class Texture {
public:
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