/******************************************************************************/
/*!
\par        Image Engine
\file       SubTexture.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 10, 2023

\brief      Implementation file for the SubTexture class.

			Defines the interface for the SubTexture class which represents
			a sub-region of a larger texture. It provides functionalities for
			creating and accessing sub-textures and their associated texture
			coordinates.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Graphics/SubTexture.hpp"

/*  _________________________________________________________________________ */
/*! SubTexture

@param tex
Shared pointer to the parent texture.

@param min
Minimum texture coordinates of the sub-texture.

@param max
Maximum texture coordinates of the sub-texture.

This constructor initializes the SubTexture using the provided parent texture
and the specified minimum and maximum texture coordinates.
*/
SubTexture::SubTexture(std::shared_ptr<Texture> const& tex, SpriteProperties const& props) :mTex{ tex }, mProps{props} {
	//mTexCoords[0] = { min.x, min.y };
	//mTexCoords[1] = { max.x, min.y };
	//mTexCoords[2] = { max.x, max.y };
	//mTexCoords[3] = { min.x, max.y };
}

/*  _________________________________________________________________________ */
/*! GetTexture

@return
Shared pointer to the parent texture.

This function returns a shared pointer to the parent texture of the sub-texture.
*/
std::shared_ptr<Texture> const SubTexture::GetTexture() const {
	return mTex;
}

/*  _________________________________________________________________________ */
/*! GetTexCoords

@return
Pointer to the array of texture coordinates.

This function returns a pointer to the array of texture coordinates of the sub-texture.
*/
std::array<glm::vec2, 4> SubTexture::GetTexCoords() const {
	//mTexCoords[0] = { min.x, min.y };
	//mTexCoords[1] = { max.x, min.y };
	//mTexCoords[2] = { max.x, max.y };
	//mTexCoords[3] = { min.x, max.y };
	return std::array<glm::vec2, 4>{
		glm::vec2{ mProps.min.x, mProps.min.y }, 
		glm::vec2{ mProps.max.x, mProps.min.y }, 
		glm::vec2{ mProps.max.x, mProps.max.y }, 
		glm::vec2{ mProps.min.x, mProps.max.y }
	};
}

/*  _________________________________________________________________________ */
/*! Create

@param tex
Shared pointer to the parent texture.

@param idxCoord
Index coordinates of the sub-texture on the parent texture.

@param size
Size of the sub-texture.

@return
Shared pointer to the created SubTexture.

This static function creates and returns a shared pointer to a SubTexture
using the provided parent texture, index coordinates, and size.
*/
std::shared_ptr<SubTexture> SubTexture::Create(std::shared_ptr<Texture> const& tex, glm::vec2 const& idxCoord, glm::vec2 const& size, ResourceID id) {
	glm::vec2 min{ (idxCoord.x * size.x) / tex->GetWidth(), (idxCoord.y * size.y) / tex->GetHeight() };
	glm::vec2 max{ ((idxCoord.x + 1) * size.x) / tex->GetWidth(), ((idxCoord.y + 1) * size.y) / tex->GetHeight() };
	std::shared_ptr<SubTexture> subtex { std::make_shared<SubTexture>(tex, SpriteProperties{id, min, max}) };
	return subtex;
}

SpriteProperties& SubTexture::GetProperties() {
	return this->mProps;
}