#pragma once

#include <string>
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>
#include "Math/MathUtils.h"

struct Text {
	std::string fontName;
	float scale;
	std::string text;
	Vec3 color;

	Text() = default;

	Text(std::string fn, float s, std::string tex, Vec3 col) :
		fontName{ fn }, scale{ s }, text{ tex }, color{ col } {}

	Text([[maybe_unused]] rapidjson::Value const& obj) {}

	bool Serialize([[maybe_unused]] rapidjson::Value& obj) {
		return false;
	}
};