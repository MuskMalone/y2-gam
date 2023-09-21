#include <Core/Serialization/SerializationManager.hpp>

namespace Serializer {
	std::shared_ptr<SerializationManager> SerializationManager::_mSelf = 0;
	std::shared_ptr<SerializationManager> SerializationManager::GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<SerializationManager>();
		return _mSelf;
	}
	bool SerializationManager::OpenJSON(std::string const& pathToFile) {
		if (mDocumentMap.find(pathToFile) != mDocumentMap.end()) return false;
		std::ifstream jsonFile{pathToFile};
		if (!jsonFile.is_open()) return false;
		std::stringstream buffer;
		buffer << jsonFile.rdbuf();
		mDocumentMap[pathToFile].Parse(buffer.str().c_str());
		if (mDocumentMap[pathToFile].HasParseError()) return false;
		return (mDocumentMap[pathToFile].IsObject());
	}
	bool SerializationManager::FlushJSON(std::string const& pathToFile) {
		if (mDocumentMap.find(pathToFile) == mDocumentMap.end()) return false;
		std::ofstream jsonFile{pathToFile};
		if (!jsonFile.is_open()) return false;
		rapidjson::StringBuffer strbuf;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
		mDocumentMap[pathToFile].Accept(writer);
		jsonFile << strbuf.GetString();
		return true;
	}
	
}