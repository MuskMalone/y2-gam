#pragma once
#include <pch.hpp>
#include <Core/Coordinator.hpp>

class SceneManager {
public:
	static std::shared_ptr<SceneManager> GetInstance();
	void Init();
	void LoadScene(std::string const&);
	void SaveScene(std::string const&);
	void ExitScene(std::string const&);
	void ResetScene(std::string const&);
	void AddAsset(std::string const& scnpath, AssetID aid);
	void RemoveAsset(std::string const& scnpath, AssetID aid);

private:
	static std::shared_ptr<SceneManager> _mSelf;
	std::set<Entity> mEntities;
	bool mSceneListenerStart;
	//listens after all the other scene entities are done
	void OnEntityEvent(Event&);
};