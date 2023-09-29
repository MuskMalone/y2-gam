/******************************************************************************/
/*!
\par        Image Engine
\file       Vec3.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Implementation file for Vec3 math

			This file contains the Implementation for a vector 3 class.
			It provides various constructors, operators, and utility functions
			for vector operations such as dot product, cross product,
			and normalization.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "Math/Vec3.h"


namespace Image {
	/*  _________________________________________________________________________ */
	/*! Vec3

	@param none.

	@return none.

	Default constructor. Set the values to 0
	*/
	Vec3::Vec3() :mData{ 0,0,0 } {}

	/*  _________________________________________________________________________ */
	/*! Vec3

	@param val
	Value for the vector

	@return none.

	Constructor that sets both values to val
	*/
	Vec3::Vec3(float val) :mData{ val,val,val } {}

	/*  _________________________________________________________________________ */
	/*! Vec3

	@param x
	X component of the vector

	@param y
	Y component of the vector

	@param z
	Z component of the vector

	@return none.

	Constructor that sets the x, y and z
	*/
	Vec3::Vec3(float x, float y, float z) : mData{ x,y,z } {}

	/*  _________________________________________________________________________ */
	/*! Vec3

	@param rhs
	Vector to copy from

	@return none.

	Copy constructor.
	*/
	Vec3::Vec3(Vec3 const& rhs) :mData{ rhs.x,rhs.y,rhs.z } {}

	/*  _________________________________________________________________________ */
	/*! Vec3

	@param rhs
	Vector to copy from

	@return none.

	Copy constructor that only takes the first 2 value of the vec2 and last value
	as 0
	*/
	Vec3::Vec3(Vec2 const& rhs) : mData{ rhs.mData[0],rhs.mData[1],0.f } {}

	/*  _________________________________________________________________________ */
	/*! Vec3

	@param rhs
	Vector to copy from.

	@return none.

	Copy constructor that only takes the first 3 value of the vec4.
	*/
	Vec3::Vec3(Vec4 const& rhs) : mData{ rhs.mData[0],rhs.mData[1],rhs.mData[2] } {}

	/*  _________________________________________________________________________ */
	/*! operator +=

	@param rhs
	Vector to add from.

	@return Vec3&
	Reference to the modified vector.

	Adds the rhs vector to its current vector.
	*/
	Vec3& Vec3::operator+=(Vec3 const& rhs){
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator-=

	@param rhs
	Vector to subtract from.

	@return Vec3&
	Reference to the modified vector.

	Subtracts the rhs vector from its current vector.
	*/
	Vec3& Vec3::operator-=(Vec3 const& rhs){
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator*=

	@param rhs
	Scalar value to multiply with.

	@return Vec3&
	Reference to the modified vector.

	Multiplies the vector by a scalar value.
	*/
	Vec3& Vec3::operator*=(float rhs){
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator/=

	@param rhs
	Scalar value to divide by.

	@return Vec3&
	Reference to the modified vector.

	Divides the vector by a scalar value.
	*/
	Vec3& Vec3::operator/=(float rhs){
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param none.

	@return Vec3
	A new vector with negated components.

	Negates the vector components.
	*/
	Vec3 Vec3::operator-() const{
		Vec3 temp{ *this };
		temp.x = -x;
		temp.y = -y;
		temp.z = -z;
		return temp;
	}

	/*  _________________________________________________________________________ */
	/*! operator[]

	@param idx
	Index of the component to access.

	@return float
	The value of the component at the given index.

	Accesses the vector component at the specified index.
	*/
	float Vec3::operator[](int idx) const {
		assert((idx >= 0 && idx < 3) && "Ensure index is within bounds");
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
	float& Vec3::operator[](int idx) {
		assert((idx >= 0 && idx < 3) && "Ensure index is within bounds");
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
	std::ostream& operator<<(std::ostream& os, Vec3 const& rhs) {
		os << rhs.mData[0] << ", " << rhs.mData[1] << ", " << rhs.mData[2];
		return os;
	}
	/*  _________________________________________________________________________ */
	/*! length

	@param rhs
	Vector whose length is to be calculated.

	@return float
	Length of the vector.

	Computes the length of the given vector.
	*/
	float length(Vec3 const& rhs){
		return std::sqrt(rhs.mData[0] * rhs.mData[0] + rhs.mData[1] * rhs.mData[1] + rhs.mData[2] * rhs.mData[2]);
	}

	/*  _________________________________________________________________________ */
	/*! normalized

	@param rhs
	Right-hand side vector.

	@return Vec3
	Normalized vector

	Computes the normalized of the vector.
	*/
	Vec3 normalized(Vec3 const& rhs){
		float len = length(rhs);
		assert((len != 0) && "Divide by 0");
		return { rhs.mData[0] / len, rhs.mData[1] / len,rhs.mData[2]/len};
	}

	/*  _________________________________________________________________________ */
	/*! cross

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return float
	Cross product of the two vectors.

	Computes the Cross product of two vectors.
	*/
	Vec3 cross(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[1] * rhs.mData[2] - lhs.mData[2] * rhs.mData[1], lhs.mData[2] * rhs.mData[0] - lhs.mData[0] * rhs.mData[2], lhs.mData[0] * rhs.mData[1] - lhs.mData[1] * rhs.mData[0]);

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
	float dot(Vec3 const& lhs, Vec3 const& rhs){
		return lhs.mData[0] * rhs.mData[0] + lhs.mData[1] * rhs.mData[1] + lhs.mData[2] * rhs.mData[2];
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return Vec3
	Addition of the two vectors.

	Computes the addition of two vectors.
	*/
	Vec3 operator+(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[0] + rhs.mData[0], lhs.mData[1] + rhs.mData[1], lhs.mData[2] + rhs.mData[2]);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return Vec3
	Subtraction of the two vectors.

	Computes the Subtraction of two vectors.
	*/
	Vec3 operator-(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[0] - rhs.mData[0], lhs.mData[1] - rhs.mData[1], lhs.mData[2] - rhs.mData[2]);
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side vector.

	@return Vec3
	Product of the two vectors.

	Computes the product of two vectors.
	*/
	Vec3 operator*(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[0] * rhs.mData[0], lhs.mData[1] * rhs.mData[1], lhs.mData[2] * rhs.mData[2]);
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec3
	Addition between vector and a float

	Computes the addition of a vector and a float
	*/
	Vec3 operator+(Vec3 const& lhs, float val){
		return Vec3(lhs.mData[0] + val, lhs.mData[1] + val, lhs.mData[2] + val);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec3
	Subtraction between vector and a float

	Computes the Subtraction of a vector and a float
	*/
	Vec3 operator-(Vec3 const& lhs, float val)	{
		return Vec3(lhs.mData[0] - val, lhs.mData[1] - val, lhs.mData[2] - val);
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec3
	Product between vector and a float

	Computes the Product of a vector and a float
	*/
	Vec3 operator*(Vec3 const& lhs, float val){
		return Vec3(lhs.mData[0] * val, lhs.mData[1] * val, lhs.mData[2] * val);
	}

	/*  _________________________________________________________________________ */
	/*! operator/

	@param lhs
	Left-hand side vector.

	@param rhs
	Right-hand side float.

	@return Vec3
	Division between vector and a float

	Computes the Division of a vector and a float
	*/
	Vec3 operator/(Vec3 const& lhs, float val){
		return Vec3(lhs.mData[0] / val, lhs.mData[1] / val, lhs.mData[2] / val);
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
	bool operator==(Vec3 const& lhs, Vec3 const& rhs){
		return (lhs. mData[0] == rhs.mData[0] && lhs.mData[1] == rhs.mData[1] && lhs.mData[2] == rhs.mData[2]);
	}

}
