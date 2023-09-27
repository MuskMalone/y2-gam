#pragma once
#include <glm/vec2.hpp>
using Vec2 = glm::vec2;
inline float Vector2Cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
inline Vec2 Vector2Cross(Vec2 a, float s) { return Vec2 { s* a.y, -s * a.x }; }
inline Vec2 Vector2Cross(float s, Vec2 a) { return Vec2 { -s * a.y, s* a.x }; }

//class Vec2
//{
//public:
//	Vec2()
//		: x(0.0f), y(0.0f)
//	{}
//
//	Vec2(float x, float y)
//		: x(x), y(y)
//	{}
//
//	Vec2 operator+(Vec2 const& v)
//	{
//		return Vec2(
//			x + v.mData[0],
//			y + v.mData[1]);
//	}
//
//	Vec2 operator+=(Vec2 const& v)
//	{
//		x += v.mData[0];
//		y += v.mData[1];
//
//		return *this;
//	}
//
//	Vec2 operator-(Vec2 const& v)
//	{
//		return Vec2(
//			x - v.mData[0],
//			y - v.mData[1]);
//	}
//
//	Vec2 operator-=(Vec2 const& v)
//	{
//		x -= v.mData[0];
//		y -= v.mData[1];
//
//		return *this;
//	}
//
//
//	float x, y;
//};
