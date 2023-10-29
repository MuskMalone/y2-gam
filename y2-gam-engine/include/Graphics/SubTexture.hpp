/******************************************************************************/
/*!
\par        Image Engine
\file       SubTexture.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Sep 10, 2023

\brief      Header file for the SubTexture class.

			Defines the interface for the SubTexture class which represents
			a sub-region of a larger texture. It provides functionalities for
			creating and accessing sub-textures and their associated texture
			coordinates.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "Graphics/Texture.hpp"
struct SpriteProperties : public ResProp{
	glm::vec2 min;
	glm::vec2 max;
};
class SubTexture {
public:
	SubTexture(std::shared_ptr<Texture> const& tex, SpriteProperties const&);

	std::shared_ptr<Texture> const GetTexture() const;
	std::array<glm::vec2, 4> GetTexCoords() const;
	SpriteProperties& GetProperties();

	static std::shared_ptr<SubTexture> Create(std::shared_ptr<Texture> const& tex, glm::vec2 const& idxCoord, glm::vec2 const& size, ResourceID id);
private:
	std::shared_ptr<Texture> mTex;
	//glm::vec2 mTexCoords[4];
	SpriteProperties mProps;
};