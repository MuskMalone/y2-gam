#include "../include/pch.hpp"
//#include "Graphics/AnimationManager.hpp"
//
//ResourceID AnimationManager::LoadAnimation(std::string const& path, int frameCount, float idxCoordy, glm::vec2 const& dim) {
//	ResourceID texID = SpriteManager::LoadTexture(path);
//
//	std::vector<AnimationFrame> frames;
//	for (float i{}; i < frameCount; ++i) {
//		ResourceID frameID = SpriteManager::CreateSubTexture(texID, SpriteProperties{ GetTimestampNano(), {i, idxCoordy}, {dim.x, dim.y} });
//		frames.push_back( AnimationFrame{ 0.f, frameID }); // supposed to be read somewhere... instead of 0.f....
//	}
//	ResourceID rid = 0; //GENERATE RID
//	return rid; 
//}
//
//std::vector<AnimationFrame> AnimationManager::GetAnimationFrameList(ResourceID rid) {
//	return mAnimationFrameLists[rid];
//}
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