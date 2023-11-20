/**
 * @file AssetManager.cpp
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief 
 * @version 0.1
 * @date 2023-11-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../include/pch.hpp"
#include <Engine/AssetManager.hpp>
#include <Core/Coordinator.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include <Audio/Sound.hpp>
std::shared_ptr<AssetManager> AssetManager::_mSelf = 0;
namespace AssetHelper{
	bool FindStr(std::string const& string, std::string const& substr) {
		return !(string.find(substr) == std::string::npos);
	}
}

void AssetManager::Init() {
	
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	if (!sm->OpenJSON(cmFileName)) sm->SetObject(cmFileName);
	std::cout << "init\n";
	for (auto sys = (sm->GetDoc(cmFileName)).MemberBegin(); sys != (sm->GetDoc(cmFileName)).MemberEnd(); ++sys) {
		auto sysName{ sys->name.GetString() };
		std::cout << sysName << std::endl;
		for (auto ass = (sm->At(cmFileName, sysName)).MemberBegin(); ass != (sm->At(cmFileName, sysName)).MemberEnd(); ++ass) {
			AssetID aid{ std::stoull(ass->name.GetString()) };
			std::string path{ ass->value["path"].GetString() };
			ResourceID rid{ ass->value["id"].GetUint64() };
			mAssets[aid] = { path, sysName, rid };
			//std::cout << "aid, path, sysname: " << aid << " " << path << " " << sysName << std::endl;
			LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Aid, Path, Sysname: " + std::to_string(aid) + 
				", " + path + ", " + sysName, __FUNCTION__);
			LoadAID(aid);
		}
	}
}


void AssetManager::LoadAssetChunk(std::set<AssetID> const& assetChunk) {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	if (!sm->OpenJSON(cmFileName)) sm->SetObject(cmFileName);
	std::cout << "init\n";
	for (auto sys = (sm->GetDoc(cmFileName)).MemberBegin(); sys != (sm->GetDoc(cmFileName)).MemberEnd(); ++sys) {
		auto sysName{ sys->name.GetString() };
		std::cout << sysName << std::endl;
		for (auto ass = (sm->At(cmFileName, sysName)).MemberBegin(); ass != (sm->At(cmFileName, sysName)).MemberEnd(); ++ass) {
			AssetID aid{ std::stoull(ass->name.GetString()) };

			//if asset is not part of the chunk
			if (assetChunk.find(aid) == assetChunk.end()) continue;
			std::string path{ ass->value["path"].GetString() };
			ResourceID rid{ ass->value["id"].GetUint64() };
			mAssets[aid] = { path, sysName, rid };
			//std::cout << "aid, path, sysname: " << aid << " " << path << " " << sysName << std::endl;
			LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Aid, Path, Sysname: " + std::to_string(aid) +
				", " + path + ", " + sysName, __FUNCTION__);

			//could use type reflection.
			LoadAID(aid);
		}
	}
}
void AssetManager::LoadAID(AssetID aid) {
	{
		using namespace AssetHelper;

		//could use type reflection.
		if (FindStr(mAssets[aid].systemType, "Sound")) {
			LoadAsset<SoundManager>(aid);
		}
		else if (FindStr(mAssets[aid].systemType, "Sprite")) {
			LoadAsset<SpriteManager>(aid);
		}
		else if (FindStr(mAssets[aid].systemType, "Animation")) {
			LoadAsset<AnimationManager>(aid);
		}

	}
}
void AssetManager::ClearAssetChunk(std::set<AssetID> const&assetChunk) {
	mAssets.clear();
}
void AssetManager::Exit() {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	sm->FlushJSON(cmFileName);
}

