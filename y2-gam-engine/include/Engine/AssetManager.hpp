#pragma once
//#include <Core/Serialization/SerializationManager.hpp>
//#include <memory>
#include <rttr/type>

class AssetManager{
public:

	static std::shared_ptr<AssetManager> GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<AssetManager>();
		return _mSelf;
	}
	void Init();
	void Exit();
private:
	inline static ResourceID PathToKey(std::string const& path) {
		return _hash(path);
	}
	
	//makes the assumption that the id is not called in rapid succession
	//ie within one loop or in consecutive lines (actions that can fire within less than a nanosecond) 
	AssetID GetAssetID() {
		return GetTimestampNano();
	}
public:
	//give a system and a assetid. 
	//system will load asset to its own map of assets and return the id of the system
	//SLOW, INVOLVES JSON
	template <typename _system>
	AssetID LoadAsset(AssetID aid) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
		using namespace Serializer;

		auto type{ rttr::type::get<_system>() };
		auto const& sysKey{ type.get_name().to_string() }; //gets name of _system type
		auto key{ std::to_string(aid) };
		if (!sm->At(cmFileName, sysKey).IsObject()) return static_cast<AssetID>(-1);
		if (!sm->At(cmFileName, sysKey)[key.c_str()].IsObject()) return static_cast<AssetID>(-1);

		_system::LoadAsset(sm->At(cmFileName, sysKey)[key.c_str()]);
		return aid;
	}
	
	//gives a system a uint32 id to save back into the json
	//SLOW INVOLVES JSON
	template <typename _system, typename _props>
	void SaveAsset(AssetID aid, _props const& p) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
		using namespace Serializer;

		auto type{ rttr::type::get<_system>() };
		auto const& sysKey{ type.get_name().to_string() }; //gets name of _system type
		auto key{ std::to_string(aid) };
		if (!sm->At(cmFileName, sysKey).IsObject()) return static_cast<AssetID>(-1);
		if (!sm->At(cmFileName, sysKey)[key].IsObject()) return static_cast<AssetID>(-1);

		_system::SaveAsset(aid, p, sm->At(cmFileName, sysKey)[key]);
	}

	//safe to use in game loop
	template <typename _system>
	auto& GetAsset(AssetID aid) {
		return _system::GetAsset(mAssets[aid].resourceId);
	}

	//safe to use in game loop
	template <typename _system>
	auto& GetAssetProperties(AssetID aid) {
		return _system::GetAssetProperties(mAssets[aid].resourceId);
	}

	//returns the AssetID of the asset that was added.
	//SLOW JSON INVOLVED
	template <typename _system>
	AssetID AddAsset(std::string const& path) {
		
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
		using namespace Serializer;

		auto type{ rttr::type::get<_system>() };
		auto const& sysKey{ type.get_name().to_string() }; //gets name of _system type
		AssetID key{ GetAssetID() };
		auto keyStr{ std::to_string(key) };

		//if the asset system has not been added
		if (!sm->GetDoc(cmFileName).HasMember(keyStr.c_str())) {
			JSONObj obj{ JSON_OBJ_TYPE };
			obj.SetObject();
			sm->InsertValue(sysKey, obj);
		}
		JSONObj ast{ JSON_OBJ_TYPE };
		ast.SetObject();
		sm->InsertValue(sm->At(cmFileName, sysKey), keyStr, ast);
		ResourceID rid{ GetTimestampNano() };
		sm->InsertValue(sm->At(cmFileName, sysKey)[keyStr.c_str()], "id", rid);
		sm->InsertValue(sm->At(cmFileName, sysKey)[keyStr.c_str()], "path", path);
		mAssets[key] = { path, sysKey,
		 _system::AddAsset(sm->At(cmFileName, sysKey)[keyStr.c_str()], path, rid ) };

		return key;
	}

	
private:
	struct Asset {
		std::string path;
		std::string systemType;
		ResourceID resourceId;
	};
	std::map<AssetID, Asset> mAssets;
	const char const* cmFileName {"Assets"};
	static std::shared_ptr<AssetManager> _mSelf;
	
	
};