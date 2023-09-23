#pragma once
#include <rapidjson/document.h>
struct Editor
{
	Editor() = default;
	Editor(rapidjson::Value const& obj) {}
	bool Serialize(rapidjson::Value& obj) {
		return false;
	}
};
