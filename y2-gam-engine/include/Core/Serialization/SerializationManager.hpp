#pragma once
#include <exception>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>
#include <string>
#include <map>
#include <memory>
#include <exception>
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


	private:
		std::string path{}; //saves the last key used
		const std::string mPath{ "Data/" };
		const std::string mExt{".json"};
		static std::shared_ptr<SerializationManager> _mSelf;
		std::map<std::string, rapidjson::Document> mDocumentMap;
	};
}