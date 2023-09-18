#pragma once
#include <rapidjson/document.h>
struct Editor
{
	Editor() = default;
	Editor(rapidjson::Value const& obj) {}
	void Serialize(rapidjson::Value& obj) {

	}
};
