#pragma once
#include "Graphics/Texture.hpp"
#include <glm/glm.hpp>
#include <memory>

class SubTexture {
public:
	SubTexture(std::shared_ptr<Texture> const& tex, glm::vec2 const& min, glm::vec2 const& max);

	std::shared_ptr<Texture> const GetTexture() const;
	glm::vec2 const* GetTexCoords() const;

	static std::shared_ptr<SubTexture> Create(std::shared_ptr<Texture> const& tex, glm::vec2 const& idxCoord, glm::vec2 const& size);
private:
	std::shared_ptr<Texture> mTex;
	glm::vec2 mTexCoords[4];
};