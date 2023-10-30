//#include "../include/pch.hpp"
//#include "Graphics/SpriteManager.hpp"
//struct AnimationProperties {
//	std::string path;
//	int frameCount;
//};
//
//
//namespace Image {
//	class AnimationManager {
//	public:
//		static ResourceID LoadAnimation(std::string const& path, int frameCount, float idxCoordy, glm::vec2 const& dim);
//
//		static std::vector<AnimationFrame> GetAnimationFrameList(ResourceID textureID);
//
//	private:
//		static std::unordered_map<ResourceID, std::vector<AnimationFrame>> mAnimationFrameLists;
//	};
//
//}

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