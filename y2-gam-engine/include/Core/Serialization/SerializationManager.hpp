/******************************************************************************/
/*!
\par        Serialization Manager
\file       SerializationManager.hpp

\author     Ng Wen Wen (wenwen.ng@digipen.edu)
\date       Sep 7, 2023

\brief      Implementation file for Serialization

			This source file contains the definitions of creation, manipulation
			and file storage of JSON data

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#define JSON_OBJ_TYPE rapidjson::kObjectType
#define JSON_ARR_TYPE rapidjson::kArrayType
namespace Serializer {
	using namespace rapidjson;
	using JSONDoc = rapidjson::Document;
	using JSONObj = rapidjson::Value;

	class SerializationManager {
	public:
		static std::shared_ptr<SerializationManager> GetInstance();
		std::ifstream& CreateJSON(std::ifstream& ifs, std::string const& path);

		bool IsJSONNull(std::string const& name);
		bool IsJSONObject(std::string const& name);
		bool OpenJSON(std::string const& name);
		bool FlushJSON(std::string const& name);
		void ClearJSON(std::string const& name);

		void SetArray(std::string const& name) {
			path = mPath + name + mExt;
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{"couldnt find json document" };
			mDocumentMap[path].SetArray();
		}

		void SetObject(std::string const& name) {
			path = mPath + name + mExt;
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{"couldnt find json document" };
			mDocumentMap[path].SetObject();
		}

		//only can be used if u used SetArray before when creating the document
		//otherwise use AddMember!!!
		void PushToArray(std::string const& name, JSONObj& to, JSONObj& obj) {
			path = mPath + name + mExt;
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{"couldnt find json document" };
			to.PushBack(obj, mDocumentMap[path].GetAllocator());
		}

		//doesnt verify type of value you write!!!!
		//pls verify it urself
		template<typename _type>
		void ModifyValue(JSONObj& obj, const char* key, _type & val) {
			if (!obj.HasMember(key)) return;
			obj[key] = val;
		}

		//doesnt verify type of value you write!!!!
		//pls verify it urself/
		//key and val are not references because AddMember uses move semantics
		template <typename _json, typename _type>
		void InsertValue(_json& obj, std::string key, _type val) {
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{"couldnt find json document" };
			JSONObj keyName( key.c_str(), static_cast<rapidjson::SizeType>(key.size()), mDocumentMap[key].GetAllocator() );
			//JSONObj valName( val, mDocumentMap[key].GetAllocator() );
			obj.AddMember(keyName, std::forward<_type>(val), mDocumentMap[key].GetAllocator());
		}

		void InsertValue(JSONObj& obj, std::string key, JSONObj& val) {
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{"couldnt find json document" };
			JSONObj keyName(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), mDocumentMap[key].GetAllocator());
			//JSONObj valName( val, mDocumentMap[key].GetAllocator() );
			obj.AddMember(keyName, val, mDocumentMap[key].GetAllocator());
		}

		void InsertValue(std::string key, JSONObj& val) {
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{"couldnt find json document" };
			JSONObj keyName(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), mDocumentMap[key].GetAllocator());
			//JSONObj valName( val, mDocumentMap[key].GetAllocator() );
			mDocumentMap[path].AddMember(keyName, val, mDocumentMap[key].GetAllocator());
		}

		JSONObj& At(std::string const& key) {
			return mDocumentMap[path][key.c_str()];
		}
		JSONObj& At(std::string const& name, std::string const& key) {
			path = mPath + name + mExt;
			return mDocumentMap[path][key.c_str()];
		}

		void InsertValue(JSONObj& obj, std::string key, std::string val) {
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{ "couldnt find json document" };
			JSONObj keyName(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), mDocumentMap[key].GetAllocator());
			JSONObj valName(val.c_str(), static_cast<rapidjson::SizeType>(val.size()), mDocumentMap[key].GetAllocator());
			obj.AddMember(keyName, valName, mDocumentMap[key].GetAllocator());
		}

		void InsertValue(JSONObj& obj, std::string key, const char* v) {
			std::string val{ v };
			if (mDocumentMap.find(path) == mDocumentMap.end()) throw std::runtime_error{ "couldnt find json document" };
			JSONObj keyName(key.c_str(), static_cast<rapidjson::SizeType>(key.size()), mDocumentMap[key].GetAllocator());
			JSONObj valName(val.c_str(), static_cast<rapidjson::SizeType>(val.size()), mDocumentMap[key].GetAllocator());
			obj.AddMember(keyName, valName, mDocumentMap[key].GetAllocator());
		}


	private:
		std::string path{}; //saves the last key used
		const std::string mPath{ "Data/" };
		const std::string mExt{".json"};
		static std::shared_ptr<SerializationManager> _mSelf;
		std::map<std::string, rapidjson::Document> mDocumentMap;
	};
}