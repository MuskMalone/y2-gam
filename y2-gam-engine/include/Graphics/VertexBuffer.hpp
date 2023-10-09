/******************************************************************************/
/*!
\par        Image Engine
\file       VertexBuffer.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 5, 2023

\brief      Header file for the BufferLayout and VertexBuffer classes.

			Contains the declarations for the BufferLayout and VertexBuffer classes.
			The BufferLayout class is responsible for managing the layout of attributes
			in a buffer, while the VertexBuffer class manages vertex buffer objects (VBOs)
			in OpenGL.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#pragma once
#include <string>
#include <vector>
#include <iostream>

enum class AttributeType {
	NONE = 0,
	FLOAT,
	VEC2,
	VEC3,
	VEC4,
	MAT3,
	MAT4,
	INT,
	IVEC2,
	IVEC3,
	IVEC4,
	BOOL
};

static unsigned int AttributeTypeSize(AttributeType type)
{
	switch (type) {
	case AttributeType::FLOAT:	return 4;
	case AttributeType::VEC2:	return 4 * 2;
	case AttributeType::VEC3:	return 4 * 3;
	case AttributeType::VEC4:	return 4 * 4;
	case AttributeType::MAT3:	return 4 * 3 * 3;
	case AttributeType::MAT4:	return 4 * 4 * 4;
	case AttributeType::INT:	return 4;
	case AttributeType::IVEC2:	return 4 * 2;
	case AttributeType::IVEC3:	return 4 * 3;
	case AttributeType::IVEC4:	return 4 * 4;
	case AttributeType::BOOL:	return 1;
	}

	std::cout << "ERROR: Unknown AttributeType!\n";
	//@TODO exit program or assert
	return 0;
}

struct BufferAttribute {
	std::string name;
	AttributeType type;
	unsigned int size;
	unsigned int offset;
	bool isNormalized;

	BufferAttribute() = default;

/*  _________________________________________________________________________ */
/*! BufferAttribute

@param b_type
The type of the attribute.

@param b_name
The name of the attribute.

@param b_isNormalized
Whether the attribute is normalized.

This constructor initializes the BufferAttribute with the provided type, name, and normalization flag.
*/
	BufferAttribute(AttributeType b_type, std::string const& b_name, bool b_isNormalized = false)
		: name{ b_name }, type{ b_type }, size{ AttributeTypeSize(b_type) }, offset{ 0 }, isNormalized{ b_isNormalized } {

	}

/*  _________________________________________________________________________ */
/*! GetComponentCount

@return
The number of components in the attribute.

This function returns the number of components based on the attribute type.
*/
	unsigned int GetComponentCount() const {
		switch (type) {
		case AttributeType::FLOAT:	return 1;
		case AttributeType::VEC2:	return 2;
		case AttributeType::VEC3:	return 3;
		case AttributeType::VEC4:	return 4;
		case AttributeType::INT:	return 1;
		case AttributeType::IVEC2:	return 2;
		case AttributeType::IVEC3:	return 3;
		case AttributeType::IVEC4:	return 4;
		case AttributeType::BOOL:	return 1;
		}

		std::cout << "ERROR: Unknown AttributeType!\n";
		//@TODO EXIT
		return 0;
	}
};

class BufferLayout {
public:
	BufferLayout() {}; //temp
	BufferLayout(std::initializer_list<BufferAttribute> const& b_elements)
		: mElements(b_elements) {
		ComputeOffsetAndStride();
	}
	inline unsigned int GetStride() const { return mStride; };
	inline const std::vector<BufferAttribute>& GetElements() const { return mElements; } //change to cpp
	std::vector<BufferAttribute>::iterator begin() { return mElements.begin(); };
	std::vector<BufferAttribute>::iterator end() { return mElements.end(); };
	std::vector<BufferAttribute>::const_iterator begin() const { return mElements.begin(); };
	std::vector<BufferAttribute>::const_iterator end() const { return mElements.end(); };
private:
	void ComputeOffsetAndStride() {
		unsigned int newOffset{ 0 };
		mStride = 0;

		for (auto& elem : mElements) {
			elem.offset = newOffset;
			newOffset += elem.size;
			mStride += elem.size;
		}
	}
private:
	std::vector<BufferAttribute> mElements;
	unsigned int mStride{};
};

#pragma once
class VertexBuffer {
public:
	VertexBuffer(unsigned int size);
	VertexBuffer(float* vertices, unsigned int size);
	~VertexBuffer();

	static std::shared_ptr<VertexBuffer> Create(unsigned int size);
	static std::shared_ptr<VertexBuffer> Create(float* vertices, unsigned int size);

	void Bind() const;
	void Unbind() const;

	BufferLayout const& GetLayout() const;
	void SetLayout(BufferLayout& layout);

	void SetData(void* const data, unsigned int size);

private:
	unsigned int mVboHdl;
	BufferLayout mLayout;
};