#pragma once

#include "Math/Vec3.hpp"
#include "Math/Mat44.hpp"

//world transform values
struct Transform
{
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;

	Transform() = default;
	Transform(Vec3 const& pos, Vec3 const& rot, Vec3 const& sca) : position{ pos }, rotation{ rot }, scale{ sca } {}
	Transform(rapidjson::Value const& obj) {}
	void Serialize(rapidjson::Value& obj) {

	}
};
