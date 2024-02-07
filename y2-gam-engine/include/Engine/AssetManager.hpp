/**
 * @file AssetManager.hpp
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief 
 * @version 0.1
 * @date 2023-11-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
//#include <Core/Serialization/SerializationManager.hpp>
//#include <memory>
#include <rttr/type>
#include <rttr/registration>
class AssetManager{
public:
/**
 * @brief Fetches the singleton instance of AssetManager.
 *
 * @return shared_ptr<AssetManager> The singleton instance.
 */

	static std::shared_ptr<AssetManager> GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<AssetManager>();
		return _mSelf;
	}
	void Init();
	//loads a set of asset ids
	void LoadAssetChunk(std::set<AssetID> const&);

	//just clears the asset map. the asset data will be deleted via its own managers
	void ClearAssetChunk(std::set<AssetID> const&);
	void Exit();

private:
	void LoadAID(AssetID aid);
/**
 * @brief Converts a file path into a unique ResourceID using hash function.
 *
 * @param path The file path to be converted.
 * @return Unique ResourceID.
 */

	inline static ResourceID PathToKey(std::string const& path) {
		return _hash(path);
	}
	
	//makes the assumption that the id is not called in rapid succession
	//ie within one loop or in consecutive lines (actions that can fire within less than a nanosecond) 
	AssetID GetAssetID() {
		return GetTimestampNano();
	}
public:
	struct Asset {
		std::string path;
		std::string systemType;
		ResourceID resourceId;
	};
	inline Asset const& GetAsset(AssetID aid) { return mAssets[aid]; }
	inline std::map<AssetID, Asset> const& GetAllAssets() { return mAssets; }

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
		//the field "id" inside this json obj refers to the rid
		_system::LoadAsset(sm->At(cmFileName, sysKey)[key.c_str()]);
		return aid;
	}
	template <typename _system>
	void UnloadAsset(Asset asset) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
		using namespace Serializer;

		_system::UnloadAsset(asset);
	}

	template <typename _system>
	AssetID ChangeAssetResource(AssetID aid, std::string const& path) {

		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
		using namespace Serializer;

		auto type{ rttr::type::get<_system>() };
		auto const& sysKey{ type.get_name().to_string() }; //gets name of _system type
		AssetID key{ aid };
		auto keyStr{ std::to_string(key) };

		//if the asset system has not been added
		if (!sm->GetDoc(cmFileName).HasMember(sysKey.c_str())) { return 0; }
		if (!sm->At(cmFileName, sysKey)[keyStr.c_str()].IsObject()) return 0;
		ResourceID rid{ GetTimestampNano() };
		sm->ModifyValue(sm->At(cmFileName, sysKey)[keyStr.c_str()], "id", rid);
		
		sm->ModifyValueString(sm->At(cmFileName, sysKey)[keyStr.c_str()], "path", path);
		mAssets[key].path = path;
		mAssets[key].resourceId = rid;
		LoadAsset<_system>(key);
		return key;
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
		if (!sm->At(cmFileName, sysKey).IsObject()) return;
		if (!sm->At(cmFileName, sysKey)[key.c_str()].IsObject()) return;

		_system::SaveAsset(aid, p, sm->At(cmFileName, sysKey)[key.c_str()]);
	}

	//safe to use in game loop
	template <typename _system>
	auto& GetAsset(AssetID aid) {
		if (mAssets.find(aid) == mAssets.end()) throw std::runtime_error("couldnt find asset");
		return _system::GetAsset(mAssets[aid].resourceId);
	}

	//safe to use in game loop
	template <typename _system>
	auto& GetAssetProperties(AssetID aid) {
		if (mAssets.find(aid) == mAssets.end()) throw std::runtime_error("couldnt find assetproperties");
		return _system::GetAssetProperties(mAssets[aid].resourceId);
	}
	//safe to use in game loop
	inline ResourceID GetResourceID(AssetID aid) {
		if (mAssets.find(aid) == mAssets.end()) return 0;
		return mAssets[aid].resourceId;
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
		if (!sm->GetDoc(cmFileName).HasMember(sysKey.c_str())) {
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
		LoadAsset<_system>(key);
		return key;
	}
	template <typename _system>
	void DeleteAsset(AssetID aid) {
		std::shared_ptr< Serializer::SerializationManager> sm {Serializer::SerializationManager::GetInstance()};
		using namespace Serializer;

		auto type{ rttr::type::get<_system>() };
		auto const& sysKey{ type.get_name().to_string() }; //gets name of _system type
		AssetID key{ aid };
		auto keyStr{ std::to_string(key) };

		if (sm->GetDoc(cmFileName).HasMember(sysKey.c_str()) &&
			sm->GetDoc(cmFileName)[sysKey.c_str()].HasMember(keyStr.c_str())) {
			
			std::cout << "deleting asset " << aid << std::endl;
			sm->GetDoc(cmFileName)[sysKey.c_str()].RemoveMember(keyStr.c_str());

			if (mAssets.find(key) != mAssets.end()) {
				UnloadAsset<_system>(mAssets[key]);
				mAssets.erase(key);
			}

		}
		

	}

	bool IsAssetExist(AssetID aid) { return (mAssets.find(aid) != mAssets.end()); }
	
private:
	std::map<AssetID, Asset> mAssets;
	char const* const cmFileName {"assets"};
	static std::shared_ptr<AssetManager> _mSelf;
};