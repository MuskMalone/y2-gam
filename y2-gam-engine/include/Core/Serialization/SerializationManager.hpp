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

namespace Serializer {
	using namespace rapidjson;
	using JSONObj = rapidjson::Value;
	class SerializationManager {
	public:
		static std::shared_ptr<SerializationManager> GetInstance();
		bool OpenJSON(std::string const& pathToFile);
		bool FlushJSON(std::string const& pathToFile);

		//doesnt verify type of value you write!!!!
		//pls verify it urself
		template<typename _type>
		void ModifyValue(JSONObj& obj, const char* key, _type const& val) {
			if (!obj.HasMember(key)) return;
			obj[key] = val;
		}

		//doesnt verify type of value you write!!!!
		//pls verify it urself
		template <typename _type>
		void InsertValue(JSONObj& obj, const char* key, _type const& val) {
			obj.AddMember(key, val);
		}

	private:
		static std::shared_ptr<SerializationManager> _mSelf;
		std::map<std::string, rapidjson::Document> mDocumentMap;
	};
}