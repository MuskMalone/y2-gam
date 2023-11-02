#include "../include/pch.hpp"
#include <Engine/AssetManager.hpp>
#include <Core/Coordinator.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include <Audio/Sound.hpp>
std::shared_ptr<AssetManager> AssetManager::_mSelf = 0;
namespace {
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
			std::cout << "aid, path, sysname: " << aid << " " << path << " " << sysName << std::endl;

			//could use type reflection.
			if (FindStr(sysName, "Sound")) {
				LoadAsset<SoundManager>(aid);
			}
			else if (FindStr(sysName, "Sprite")) {
				LoadAsset<SpriteManager>(aid);
			}
			else if (FindStr(sysName, "Animation")) {
				LoadAsset<AnimationManager>(aid);
			}

		}
	}
}
void AssetManager::Exit() {
	std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
	sm->FlushJSON(cmFileName);
}

