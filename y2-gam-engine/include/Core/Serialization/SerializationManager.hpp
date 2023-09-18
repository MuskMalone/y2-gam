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
namespace Serialization {
	using namespace rapidjson;
	class SerializationManager {
	public:
		static std::shared_ptr<SerializationManager> GetInstance();
		bool OpenJSON(std::string const& pathToFile);
		bool FlushJSON(std::string const& pathToFile);
		
	private:
		static std::shared_ptr<SerializationManager> _mSelf;
		std::map<std::string, rapidjson::Document> mDocumentMap;
	};
}