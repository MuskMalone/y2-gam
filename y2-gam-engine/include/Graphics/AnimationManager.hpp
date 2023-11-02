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

	//for assetmanager
	static ResourceID LoadAsset(rapidjson::Value const& obj);
	static void SaveAsset(ResourceID aid, AnimationProperties const& props, rapidjson::Value& obj);
	static AnimationFrames const& GetAsset(ResourceID);
	static AnimationProperties& GetAssetProperties(ResourceID);
	static ResourceID AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID);


private:
	static std::unordered_map<ResourceID, AnimationProperties> mAnimationFrameLists;
};

