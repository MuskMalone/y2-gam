#include "Graphics/SubTexture.hpp"

SubTexture::SubTexture(std::shared_ptr<Texture> const& tex, glm::vec2 const& min, glm::vec2 const& max) :mTex{ tex } {
	mTexCoords[0] = { min.x, min.y };
	mTexCoords[1] = { max.x, min.y };
	mTexCoords[2] = { max.x, max.y };
	mTexCoords[3] = { min.x, max.y };
}

std::shared_ptr<Texture> const SubTexture::GetTexture() const {
	return mTex;
}

glm::vec2 const* SubTexture::GetTexCoords() const {
	return mTexCoords;
}

std::shared_ptr<SubTexture> SubTexture::Create(std::shared_ptr<Texture> const& tex, glm::vec2 const& idxCoord, glm::vec2 const& size) {
	glm::vec2 min{ (idxCoord.x * size.x) / tex->GetWidth(), (idxCoord.y * size.y) / tex->GetHeight() };
	glm::vec2 max{ ((idxCoord.x + 1) * size.x) / tex->GetWidth(), ((idxCoord.y + 1) * size.y) / tex->GetHeight() };
	std::shared_ptr<SubTexture> subtex { std::make_shared<SubTexture>(tex, min, max) };
	/*std::shared_ptr<SubTexture> subtex;
	subtex.reset(new SubTexture{ tex, min, max });*/
	return subtex;
}