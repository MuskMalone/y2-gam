#pragma once
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <cassert>
#include "Vec3.h"
#include "Vec4.h"
namespace Image {
	union Vec3;
	union Vec4;
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

/**************************************************************************/
/*!

 */
 /**************************************************************************/
	typedef union Vec2
	{
		struct
		{
			float x, y;
		};

		float mData[2];

		// Constructors
		Vec2();
		Vec2(float val);
		Vec2(float x, float y);
		Vec2(Vec2 const& rhs);
		Vec2(Vec3 const& rhs);
		Vec2(Vec4 const& rhs);


		// Assignment operators
		Vec2& operator += (const Vec2& rhs);
		Vec2& operator -= (const Vec2& rhs);
		Vec2& operator *= (float rhs);
		Vec2& operator /= (float rhs);

		// Unary operators
		Vec2 operator -() const;
		float operator[](int idx)const;
		float& operator[](int idx);
	}  Vec2;

#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
	// Overloads
	inline float Vector2Cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
	inline Vec2 Vector2Cross(Vec2 a, float s) { return Vec2 { s* a.y, -s * a.x }; }
	inline Vec2 Vector2Cross(float s, Vec2 a) { return Vec2 { -s * a.y, s* a.x }; }
	float length(Vec2 const& v);
	Vec2 normalized(Vec2 const& v);
	float cross(Vec2 const& lhs, Vec2 const& rhs);
	Vec2 cross(Vec2 const& lhs, float const& rhs);
	Vec2 cross(float const& lhs, Vec2 const& rhs);
	float dot(Vec2 const& lhs, Vec2 const& rhs);
	Vec2 operator+(Vec2 const& lhs, Vec2 const& rhs);
	Vec2 operator-(Vec2 const& lhs, Vec2 const& rhs);
	Vec2 operator*(Vec2 const& lhs, Vec2 const& rhs);
	Vec2 operator+(Vec2 const& lhs, float val);
	Vec2 operator-(Vec2 const& lhs, float val);
	Vec2 operator*(Vec2 const& lhs, float val);
	Vec2 operator/(Vec2 const& lhs, float val);
	bool operator==(Vec2 const& lhs, Vec2 const& rhs);
	std::ostream& operator<<(std::ostream& os, Vec2 const& v);




//#ifdef _MSC_VER
//    // Supress warning: nonstandard extension used : nameless struct/union
//#pragma warning( disable : 4201 )
//#endif
//    typedef union Vec2{
//        struct{
//            float x, y;
//        };
//
//        float mData[2];
//        //Constructors
//        Vec2();
//        Vec2(float val);
//        Vec2(float x, float y);
//        Vec2(Vec2 const& rhs);
//        Vec2(Vec3 const& rhs);
//        Vec2(Vec4 const& rhs);
//
//        //Overloads
//        Vec2 operator+(Vec2 const& rhs) const;
//        Vec2 operator-(Vec2 const& rhs) const;
//        Vec2 operator*(Vec2 const& rhs)const;
//        Vec2 operator+(float val) const;
//        Vec2 operator-(float val) const;
//        Vec2 operator*(float val) const;
//        Vec2 operator/(float val) const;
//        Vec2& operator+=(Vec2 const& rhs);
//        Vec2& operator-=(Vec2 const& rhs);
//        Vec2& operator*=(float const& val);
//        Vec2& operator/=(float const& val);
//        bool operator==(Vec2 rhs);
//        float operator[](int idx)const;
//        float& operator[](int idx);
//        //Members
//        //float mData[2];
//    };
//    std::ostream& operator<<(std::ostream& os, Vec2 const& v);
//    float length(Vec2 const& rhs);
//    float normalized(Vec2 const& lhs, Vec2 const& rhs);
//    float cross(Vec2 const& lhs, Vec2 const& rhs);
//    float dot(Vec2 const& lhs, Vec2 const& rhs);

}
