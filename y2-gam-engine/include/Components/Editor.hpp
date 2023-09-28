#pragma once
#include <rapidjson/document.h>
struct Editor
{
	Editor() = default;
	Editor([[maybe_unused]] rapidjson::Value const& obj) {}
	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};
