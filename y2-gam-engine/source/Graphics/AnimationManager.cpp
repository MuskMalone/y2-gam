#include "../include/pch.hpp"
#include "Graphics/AnimationManager.hpp"
#include "Graphics/SpriteManager.hpp"
std::unordered_map<ResourceID, AnimationProperties> AnimationManager::mAnimationFrameLists;
ResourceID AnimationManager::LoadAnimation(std::string const& path, ResourceID rid, int frameCount, float idxCoordy, glm::vec2 const& dim) {
	ResourceID texID = SpriteManager::LoadTexture(path);
	AnimationProperties ap{};
	ap.id = rid;
	ap.frameCount = frameCount;
	ap.idxCoordy = idxCoordy;
	ap.dim = dim;
	
	for (float i{}; i < frameCount; ++i) {
		ResourceID frameID = SpriteManager::CreateSubTexture(texID, SpriteProperties{ GetTimestampNano(), {i, idxCoordy}, {dim.x, dim.y} });
		ap.frames.push_back( AnimationFrame{ 0.f, frameID }); // supposed to be read somewhere... instead of 0.f....
	}

	mAnimationFrameLists[rid] = std::move(ap);
	return rid; 
}

AnimationFrames& AnimationManager::GetAnimationFrameList(ResourceID rid) {
	return mAnimationFrameLists[rid].frames;
}


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