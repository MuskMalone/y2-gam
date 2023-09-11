#pragma once
#include <string>

class Texture {
public:
	Texture(std::string const& path);
	Texture(unsigned int width, unsigned int height);
	~Texture();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

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