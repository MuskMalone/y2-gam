/******************************************************************************/
/*!
\par        Image Engine
\file       Vec2.h

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Header file for Vec2 math

			This file contains the declaration for a vector 2 class.
			It provides various constructors, operators, and utility functions 
			for vector operations such as dot product, cross product, 
			and normalization.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
		Vec2& operator += (Vec2 const& rhs);
		Vec2& operator -= (Vec2 const& rhs);
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
	//inline float Vector2Cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
	//inline Vec2 Vector2Cross(Vec2 a, float s) { return Vec2 { s* a.y, -s * a.x }; }
	//inline Vec2 Vector2Cross(float s, Vec2 a) { return Vec2 { -s * a.y, s* a.x }; }
	float length(Vec2 const& rhs);
	Vec2 normalized(Vec2 const& rhs);
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

}
