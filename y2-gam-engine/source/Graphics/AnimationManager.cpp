#include "../include/pch.hpp"
#include "Graphics/AnimationManager.hpp"
#include "Graphics/SpriteManager.hpp"
std::unordered_map<ResourceID, AnimationProperties> AnimationManager::mAnimationFrameLists;
ResourceID AnimationManager::LoadAnimation(std::string const& path, ResourceID rid, int frameCount, float idxCoordy, glm::vec2 const& dim) {
	ResourceID texID = SpriteManager::LoadTexture(path);
	AnimationProperties ap{};
	ap.path = path;
	ap.id = rid;
	ap.frameCount = frameCount;
	ap.idxCoordy = idxCoordy;
	ap.dim = dim;
	static uint32_t animframeID = 1;

	for (float i{}; i < frameCount; ++i) {
		ResourceID frameID = SpriteManager::CreateSubTexture(texID, SpriteProperties{ animframeID++, {i, idxCoordy}, {dim.x, dim.y} });
		ap.frames.push_back( AnimationFrame{ 0.f, frameID }); // supposed to be read somewhere... instead of 0.f....
	}

	mAnimationFrameLists[rid] = std::move(ap);
	return rid; 
}

AnimationFrames& AnimationManager::GetAnimationFrameList(ResourceID rid) {
	return mAnimationFrameLists[rid].frames;
}



// asset manager
ResourceID AnimationManager::LoadAsset(rapidjson::Value const& obj) {
	ResourceID key{ obj["id"].GetUint64() };
	LoadAnimation(obj["path"].GetString(), key, obj["frameCount"].GetInt(), obj["idxCoordY"].GetFloat(),
		glm::vec2{obj["dimX"].GetFloat(), obj["dimY"].GetFloat()});
	return key;
}
void AnimationManager::SaveAsset(ResourceID aid, AnimationProperties const& props, rapidjson::Value& obj) {
	auto sm{ Serializer::SerializationManager::GetInstance() };

	sm->ModifyValue(obj, "frameCount", props.frameCount);
	sm->ModifyValue(obj, "idxCoordY", props.idxCoordy);

	sm->ModifyValue(obj, "dimX", props.dim.x);
	sm->ModifyValue(obj, "dimY", props.dim.y);
}
AnimationFrames const& AnimationManager::GetAsset(ResourceID rid) {
	return mAnimationFrameLists[rid].frames;
}
AnimationProperties& AnimationManager::AnimationManager::GetAssetProperties(ResourceID rid) {
	return mAnimationFrameLists[rid];
}
ResourceID AnimationManager::AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID rid) {
	auto sm{ Serializer::SerializationManager::GetInstance() };

	sm->InsertValue(obj, "frameCount", 0);
	sm->InsertValue(obj, "idxCoordY", 0.0);

	sm->InsertValue(obj, "dimX", 0.0);
	sm->InsertValue(obj, "dimY", 0.0);
	
	return rid;

}

