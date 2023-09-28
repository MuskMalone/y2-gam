/******************************************************************************/
/*!
\par        Image Engine
\file       Vec4.hpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Implementation file for Vec4 math

			This file contains the Implementation for a vector 4 class.
			It provides various constructors, operators, and utility functions
			for vector operations such as dot product, cross product,
			and normalization.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "Math/Vec4.h"

namespace Image {

	/*  _________________________________________________________________________ */
	/*! Vec4

	@param none.

	@return none.

	Default constructor. Set the values to 0
	*/
	Vec4::Vec4() :mData{ 0,0,0,0 } {}

	/*  _________________________________________________________________________ */
	/*! Vec4

	@param val
	Value for the vector

	@return none.

	Constructor that sets both values to val
	*/
	Vec4::Vec4(float val) :mData{ val,val,val,val } {}

	/*  _________________________________________________________________________ */
	/*! Vec4

	@param x
	X component of the vector

	@param y
	Y component of the vector

	@param z
	Z component of the vector

	@param w
	W component of the vector

	@return none.

	Constructor that sets the x, y, z and w
	*/
	Vec4::Vec4(float x, float y, float z, float w) : mData{ x,y,z,w } {}
	
	/*  _________________________________________________________________________ */
	/*! Vec4

	@param rhs
	Vector to copy from

	@return none.

	Copy constructor.
	*/
	Vec4::Vec4(Vec4 const& rhs) :mData{rhs.mData[0],rhs.mData[1],rhs.mData[2],rhs.mData[3]} {}

	/*  _________________________________________________________________________ */
	/*! Vec4

	@param rhs
	Vector to copy from

	@return none.

	Copy constructor that only takes the first 2 value of the vec2 and last 2 value
	as 0
	*/
	Vec4::Vec4(Vec2 const& rhs) : mData{ rhs.mData[0],rhs.mData[1],0.f,0.f } {}
	
	/*  _________________________________________________________________________ */
	/*! Vec4

	@param rhs
	Vector to copy from.

	@return none.

	Copy constructor that only takes the first 3 value of the vec3 and last value
	as 0.
	*/
	Vec4::Vec4(Vec3 const& rhs) : mData{ rhs.mData[0],rhs.mData[1],rhs.mData[2],0.f } {}

	/*  _________________________________________________________________________ */
	/*! operator +=

	@param rhs
	Vector to add from.

	@return Vec4&
	Reference to the modified vector.

	Adds the rhs vector to its current vector.
	*/
	Vec4& Vec4::operator+=(Vec4 const& rhs) {
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator-=

	@param rhs
	Vector to subtract from.

	@return Vec4&
	Reference to the modified vector.

	Subtracts the rhs vector from its current vector.
	*/
	Vec4& Vec4::operator-=(Vec4 const& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator*=

	@param rhs
	Scalar value to multiply with.

	@return Vec4&
	Reference to the modified vector.

	Multiplies the vector by a scalar value.
	*/
	Vec4& Vec4::operator*=(float rhs) {
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator/=

	@param rhs
	Scalar value to divide by.

	@return Vec4&
	Reference to the modified vector.

	Divides the vector by a scalar value.
	*/
	Vec4& Vec4::operator/=(float rhs) {
		x /= rhs;
		y /= rhs;
		z /= rhs;
		z /= rhs;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param none.

	@return Vec4
	A new vector with negated components.

	Negates the vector components.
	*/
	Vec4 Vec4::operator-() const {
		Vec4 temp{ *this };
		temp.x = -x;
		temp.y = -y;
		temp.z = -z;
		temp.w = -w;
		return temp;
	}

	/*  _________________________________________________________________________ */
	/*! length

	@param rhs
	Vector whose length is to be calculated.

	@return float
	Length of the vector.

	Computes the length of the given vector.
	*/
	float length(Vec4 const& rhs) {
		return std::sqrt(rhs.mData[0] * rhs.mData[0] + rhs.mData[1] * rhs.mData[1] + rhs.mData[2] * rhs.mData[2]+ rhs.mData[3]* rhs.mData[3]);
	}

	/*  _________________________________________________________________________ */
	/*! normalized

	@param rhs
	Right-hand side vector.

	@return Vec4
	Normalized vector

	Computes the normalized of the vector.
	*/
	Vec4 normalized(Vec4 const& rhs) {
		float len = length(rhs);
		assert((len != 0) && "Divide by 0");
		return { rhs.mData[0] / len, rhs.mData[1] / len,rhs.mData[2] / len ,rhs.mData[3]/len};
	}

	/*  _________________________________________________________________________ */
	/*! dot

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return float
	Dot product of the two vectors.

	Computes the dot product of two vectors.
	*/
	float dot(Vec4 const& lhs, Vec4 const& rhs) {
		return lhs.mData[0] * rhs.mData[0] + lhs.mData[1] * rhs.mData[1] + lhs.mData[2] * rhs.mData[2] + lhs.mData[3] * rhs.mData[3];
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return Vec4
	Addition of the two vectors.

	Computes the addition of two vectors.
	*/
	Vec4 operator+(Vec4 const& lhs, Vec4 const& rhs) {
		return Vec4(lhs.mData[0] + rhs.mData[0], lhs.mData[1] + rhs.mData[1], lhs.mData[2] + rhs.mData[2],lhs.mData[3]+rhs.mData[3]);
	}
	
	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return Vec4
	Subtraction of the two vectors.

	Computes the Subtraction of two vectors.
	*/
	Vec4 operator-(Vec4 const& lhs, Vec4 const& rhs) {
		return Vec4(lhs.mData[0] - rhs.mData[0], lhs.mData[1] - rhs.mData[1], lhs.mData[2] - rhs.mData[2], lhs.mData[3] - rhs.mData[3]);
	}


	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return Vec4
	Product of the two vectors.

	Computes the product of two vectors.
	*/
	Vec4 operator*(Vec4 const& lhs, Vec4 const& rhs) {
		return Vec4(lhs.mData[0] * rhs.mData[0], lhs.mData[1] * rhs.mData[1], lhs.mData[2] * rhs.mData[2], lhs.mData[3] * rhs.mData[3]);
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec4
	Addition between vector and a float

	Computes the addition of a vector and a float
	*/
	Vec4 operator+(Vec4 const& lhs, float val) {
		return Vec4(lhs.mData[0] + val, lhs.mData[1] + val, lhs.mData[2] + val,lhs.mData[3]+val);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec4
	Subtraction between vector and a float

	Computes the Subtraction of a vector and a float
	*/
	Vec4 operator-(Vec4 const& lhs, float val) {
		return Vec4(lhs.mData[0] - val, lhs.mData[1] - val, lhs.mData[2] - val, lhs.mData[3] - val);
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec4
	Product between vector and a float

	Computes the Product of a vector and a float
	*/
	Vec4 operator*(Vec4 const& lhs, float val) {
		return Vec4(lhs.mData[0] * val, lhs.mData[1] * val, lhs.mData[2] * val, lhs.mData[3] * val);
	}
	
	/*  _________________________________________________________________________ */
	/*! operator/

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec4
	Division between vector and a float

	Computes the Division of a vector and a float
	*/
	Vec4 operator/(Vec4 const& lhs, float val) {
		return Vec4(lhs.mData[0] / val, lhs.mData[1] / val, lhs.mData[2] / val, lhs.mData[3] / val);
	}

	/*  _________________________________________________________________________ */
	/*! operator==

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return bool
	True if the vectors as the same else false

	Checks if the vector are similar
	*/
	bool operator==(Vec4 const& lhs, Vec4 const& rhs) {
		return (lhs.mData[0] == rhs.mData[0] && lhs.mData[1] == rhs.mData[1] && lhs.mData[2] == rhs.mData[2] && lhs.mData[3]==rhs.mData[3]);
	}

	/*  _________________________________________________________________________ */
	/*! operator[]

	@param idx
	Index of the component to access.

	@return float
	The value of the component at the given index.

	Accesses the vector component at the specified index.
	*/
	float Vec4::operator[](int idx) const {
		assert((idx >= 0 && idx < 4) && "Ensure index is within bounds");
		return mData[idx];
	}

	/*  _________________________________________________________________________ */
	/*! operator[]

	@param idx
	Index of the component to access.

	@return float&
	Reference to the component at the given index.

	Accesses the vector component at the specified index.
	*/
	float& Vec4::operator[](int idx) {
		assert((idx >= 0 && idx < 4) && "Ensure index is within bounds");
		return mData[idx];
	}

	/*  _________________________________________________________________________ */
	/*! operator<<

	@param os
	Output stream.

	@param rhs
	Vector to be printed.

	@return std::ostream&
	Reference to the output stream.

	Outputs the vector to the given stream.
	*/
	std::ostream& operator<<(std::ostream& os, Vec4 const& rhs) {
		os << rhs.mData[0] << ", " << rhs.mData[1] << ", " << rhs.mData[2] << ", " << rhs.mData[3];
		return os;
	}

}
