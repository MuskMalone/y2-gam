#include <Core/Serialization/SerializationManager.hpp>

namespace Serializer {
	std::shared_ptr<SerializationManager> SerializationManager::_mSelf = 0;
	std::shared_ptr<SerializationManager> SerializationManager::GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<SerializationManager>();
		return _mSelf;
	}

	//creates an empty json in the path given
	//returns ifstream of created json
	//if file already exists returns ifstream to original file
	std::ifstream& SerializationManager::CreateJSON(std::ifstream& ifs, std::string const& path) {
		ifs.open(path);
		if (ifs.is_open()) return ifs;

		//creates an empty file
		std::ofstream ofs{path};
		ofs.close();
		ifs.open(path);
		return ifs;
	}

	//{} or [] is not considered to be null
	bool SerializationManager::IsJSONNull(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return false;
		return mDocumentMap[path].IsNull();
	}

	bool SerializationManager::IsJSONObject(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return false;
		return mDocumentMap[path].IsObject();
	}

	bool SerializationManager::OpenJSON(std::string const& name) {
		path = mPath + name + mExt;
		std::ifstream jsonFile{};
		CreateJSON(jsonFile, path);
		std::stringstream buffer;
		buffer << jsonFile.rdbuf();
		mDocumentMap[path].Parse(buffer.str().c_str());
		if (mDocumentMap[path].HasParseError()) return false;
		return (mDocumentMap [path].IsObject());
	}
	bool SerializationManager::FlushJSON(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return false;
		std::ofstream jsonFile{path};
		if (!jsonFile.is_open()) return false;
		rapidjson::StringBuffer strbuf;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
		mDocumentMap[path].Accept(writer);
		jsonFile << strbuf.GetString();
		return true;
	}
	void SerializationManager::ClearJSON(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return;
		mDocumentMap[path].SetNull();
	}
}