#include "../include/pch.hpp"
#include <Engine/AssetManager.hpp>
#include <Core/Coordinator.hpp>
std::shared_ptr<AssetManager> AssetManager::_mSelf = 0;
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
			mAssets[aid] = { path, sysName, PathToKey(path) };
			std::cout << "aid, path, sysname: " << aid << " " << path << " " << sysName << std::endl;

		}
	}
}
void AssetManager::Exit() {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	sm->FlushJSON(cmFileName);
}

