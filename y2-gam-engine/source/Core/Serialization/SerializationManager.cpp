/******************************************************************************/
/*!
\par        Serialization Manager
\file       SerializationManager.cpp

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
#include "../include/pch.hpp"

#include <Core/Serialization/SerializationManager.hpp>

namespace Serializer {

	std::shared_ptr<SerializationManager> SerializationManager::_mSelf = 0;
	/*  _________________________________________________________________________ */
	/*! GetInstance

	@param indices
	Pointer to the indices data.

	@return
	Shared pointer to the SerializationManager instance.

	This function returns a shared pointer to the SerializationManager instance.
	If the instance doesn't exist, it creates one.
	*/
	std::shared_ptr<SerializationManager> SerializationManager::GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<SerializationManager>();
		return _mSelf;
	}

	//creates an empty json in the path given
	//returns ifstream of created json
	//if file already exists returns ifstream to original file
	/*  _________________________________________________________________________ */
	/*! CreateJSON

	@param ifs
    Reference to the ifstream for file input.

	@param path
	JSON file path.

	@return
    Reference to the opened ifstream for the JSON file.

	Creates empty JSON file at path or opens an existing one.
	*/
	std::ifstream& SerializationManager::CreateJSON(std::ifstream& ifs, std::string const& p) {
		try {
			std::cout << p << std::endl;
			ifs.imbue(std::locale("C")); // or another appropriate locale
			ifs.open(p);
		}
		catch (std::exception const& e) {
			std::cout << e.what();
		}

		if (ifs.is_open()) return ifs;
		if (!ifs.is_open()) throw std::runtime_error("cant open json\n");

		//creates an empty file
		std::ofstream ofs{p};
		ofs.imbue(std::locale("C"));
		ofs.close();
		ofs.imbue(std::locale("C"));
		ifs.open(p);
		return ifs;
	}

	//{} or [] is not considered to be null
	/*  _________________________________________________________________________ */
	/*! IsJSONNull

	@param name
    The name of the JSON document.

	@return
    Return true if the JSON document is null, else return false.

	Checks if the JSON document is null.
	*/
	bool SerializationManager::IsJSONNull(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return false;
		return mDocumentMap[path].IsNull();
	}

	/*  _________________________________________________________________________ */
	/*! IsJSONObject

	@param name
    The name of the JSON document.

	@return
    Return true if the JSON document is an object, else return false.

	Checks if the JSON document is an object.
	*/
	bool SerializationManager::IsJSONObject(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return false;
		return mDocumentMap[path].IsObject();
	}

	/*  _________________________________________________________________________ */
	/*! OpenJSON

	@param name
    The name of the JSON document.

	@return
    Return true if the JSON document is successfully opened and parsed as an object, 
	else return false.

	Opens and parses the JSON document.
	*/
	bool SerializationManager::OpenJSON(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) != mDocumentMap.end()) 
			return (mDocumentMap[path].IsObject());
		std::ifstream jsonFile{};

		CreateJSON(jsonFile, path);

		std::stringstream buffer;
		buffer << jsonFile.rdbuf();
		mDocumentMap[path].Parse(buffer.str().c_str());
		if (mDocumentMap[path].HasParseError()) return false;
		return (mDocumentMap [path].IsObject());
	}

	/*  _________________________________________________________________________ */
	/*! FlushJSON

	@param name
    The name of the JSON document.

	@return
    Return true if the JSON document is successfully written to its file, 
	else return false.

	Writes the JSON document back to its file.
	*/
	bool SerializationManager::FlushJSON(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return false;
		std::ofstream jsonFile{};
		jsonFile.imbue(std::locale(""));
		jsonFile.open(path);
		if (!jsonFile.is_open()) return false;
		rapidjson::StringBuffer strbuf;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
		mDocumentMap[path].Accept(writer);
		jsonFile << strbuf.GetString();
		return true;
	}

	/*  _________________________________________________________________________ */
	/*! ClearJSON

	@param name
    The name of the JSON document.

	Clears the JSON document\.
	*/
	void SerializationManager::ClearJSON(std::string const& name) {
		path = mPath + name + mExt;
		if (mDocumentMap.find(path) == mDocumentMap.end()) return;
		mDocumentMap[path].SetNull();
	}
}