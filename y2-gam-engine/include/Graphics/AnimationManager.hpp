#include "../include/pch.hpp"
using AnimationFrames = std::vector<AnimationFrame>;
struct AnimationProperties : public ResProp{
	std::string path;
	AnimationFrames frames;
	int frameCount;
	float idxCoordy;
	glm::vec2 dim;
	
};
class AnimationManager {
public:
	static ResourceID LoadAnimation(std::string const& path, ResourceID rid, int frameCount, float idxCoordy, glm::vec2 const& dim);
	static AnimationFrames& GetAnimationFrameList(ResourceID textureID);

private:
	static std::unordered_map<ResourceID, AnimationProperties> mAnimationFrameLists;
};

//ResourceID texrid{ SpriteManager::LoadTexture("../assets/textures/ROBIN_ANIM_Spritesheet.png") };
//
////TEMP
//for (float i{}; i < 7; ++i)
//	SpriteManager::CreateSubTexture(texrid, SpriteProperties{ GetTimestampNano(), {i, 1}, {256, 256} });
//
//
//for (float i{ 7 }; i < 16; ++i)
//	SpriteManager::CreateSubTexture(texrid, SpriteProperties{ GetTimestampNano(), { i, 2 }, { 256, 256 } });
//
//for (float i{ 16 }; i < 23; ++i)
//	SpriteManager::CreateSubTexture(texrid, SpriteProperties{ GetTimestampNano(), { i, 0 }, { 256, 256 } });