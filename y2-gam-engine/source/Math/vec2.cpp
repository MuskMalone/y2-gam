/******************************************************************************/
/*!
\par        Image Engine
\file       Vec2.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Implementation file for Vec2 math

            This file contains the Implementation for a vector 2 class.
            It provides various constructors, operators, and utility functions
            for vector operations such as dot product, cross product,
            and normalization.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "Math/Vec2.h"


namespace Image {
    /*  _________________________________________________________________________ */
    /*! Vec2

    @param none.

    @return none.

    Default constructor. Set the values to 0
    */
	Vec2::Vec2() :mData{ 0,0 } {}
    /*  _________________________________________________________________________ */
    /*! Vec2

    @param val
    Value for the vector

    @return none.

    Constructor that sets both values to val
    */
	Vec2::Vec2(float val) :mData{ val,val } {}
    /*  _________________________________________________________________________ */
    /*! Vec2

    @param x
    X component of the vector

    @param y
    Y component of the vector

    @return none.

    Constructor that sets the x and y
    */
	Vec2::Vec2(float x, float y) : mData{ x,y } {}
    /*  _________________________________________________________________________ */
    /*! Vec2

    @param rhs
    Vector to copy from

    @return none.

    Copy constructor.
    */
	Vec2::Vec2(Vec2 const& rhs) : mData{ rhs.x,rhs.y } {}
    /*  _________________________________________________________________________ */
    /*! Vec2

    @param rhs
    Vector to copy from

    @return none.

    Copy constructor that only takes the first 2 value of the vec3
    */
	Vec2::Vec2(Vec3 const& rhs) :mData{ rhs.mData[0],rhs.mData[1] } {}
    /*  _________________________________________________________________________ */
    /*! Vec2

    @param rhs
    Vector to copy from.

    @return none.

    Copy constructor that only takes the first 2 value of the vec4.
    */
	Vec2::Vec2(Vec4 const& rhs) :mData{ rhs.mData[0],rhs.mData[1] } {}

    /*  _________________________________________________________________________ */
    /*! operator +=

    @param rhs
    Vector to add from.

    @return Vec2&
    Reference to the modified vector.

    Adds the rhs vector to its current vector.
    */
    Vec2& Vec2::operator += (Vec2 const& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    /*  _________________________________________________________________________ */
    /*! operator-=

    @param rhs
    Vector to subtract from.

    @return Vec2&
    Reference to the modified vector.

    Subtracts the rhs vector from its current vector.
    */
    Vec2& Vec2::operator -= (Vec2 const& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    /*  _________________________________________________________________________ */
    /*! operator*=

    @param rhs
    Scalar value to multiply with.

    @return Vec2&
    Reference to the modified vector.

    Multiplies the vector by a scalar value.
    */
    Vec2& Vec2::operator *= (float rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    /*  _________________________________________________________________________ */
    /*! operator/=

    @param rhs
    Scalar value to divide by.

    @return Vec2&
    Reference to the modified vector.

    Divides the vector by a scalar value.
    */
    Vec2& Vec2::operator /= (float rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }

    /*  _________________________________________________________________________ */
    /*! operator-

    @param none.

    @return Vec2
    A new vector with negated components.

    Negates the vector components.
    */
    Vec2 Vec2::operator -() const {
        Vec2 temp{ *this };
        temp.x = -x;
        temp.y = -y;
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
    float Vec2::operator[](int idx) const {
        assert((idx >= 0 && idx < 2) && "Ensure index is within bounds");
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
    float& Vec2::operator[](int idx) {
        assert((idx >= 0 && idx < 2) && "Ensure index is within bounds");
        return mData[idx];
    }

    /*  _________________________________________________________________________ */
    /*! length

    @param rhs
    Vector whose length is to be calculated.

    @return float
    Length of the vector.

    Computes the length of the given vector.
    */
    float length(Vec2 const& rhs) {
        return std::sqrt(rhs.mData[0] * rhs.mData[0] + rhs.mData[1] * rhs.mData[1]);
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
    float dot(Vec2 const& lhs,  Vec2 const& rhs) {
        return lhs.mData[0] * rhs.mData[0] + lhs.mData[1] * rhs.mData[1];
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
    float cross(Vec2 const& lhs, Vec2 const& rhs) {
        return lhs.mData[0] * rhs.mData[1] - lhs.mData[1]* rhs.mData[0];
    }

    /*  _________________________________________________________________________ */
    /*! cross

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side float.

    @return Vec2
    Cross product of the vector and float.

    Computes the Cross product of vector and float.
    */
    Vec2 cross(Vec2 const& lhs, float const& rhs) {
        return Vec2{ lhs.mData[1] * rhs , -rhs * lhs.mData[0] };
    }

    /*  _________________________________________________________________________ */
    /*! cross

    @param lhs
    Left-hand side float.

    @param rhs
    Right-hand side vector.

    @return Vec2
    Cross product of the vector and float.

    Computes the Cross product of vector and float.
    */
    Vec2 cross(float const& lhs, Vec2 const& rhs) {
        return  Vec2{ -lhs * rhs.mData[1] , rhs.mData[0] * lhs };
    }

    /*  _________________________________________________________________________ */
    /*! normalized

    @param rhs
    Right-hand side vector.

    @return Vec2
    Normalized vector

    Computes the normalized of the vector.
    */
    Vec2 normalized( Vec2 const& rhs) {
        float len = length(rhs);
        assert((len != 0) && "Divide by 0");
        return { rhs.mData[0] / len, rhs.mData[1] / len };
    }

    /*  _________________________________________________________________________ */
    /*! operator+

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side vector.

    @return Vec2
    Addition of the two vectors.

    Computes the addition of two vectors.
    */
    Vec2 operator+(Vec2 const& lhs, Vec2 const& rhs) {
        return { lhs.mData[0] + rhs.mData[0], lhs.mData[1] + rhs.mData[1] };
    }

    /*  _________________________________________________________________________ */
    /*! operator-

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side vector.

    @return Vec2
    Subtraction of the two vectors.

    Computes the Subtraction of two vectors.
    */
    Vec2 operator-(Vec2 const& lhs, Vec2 const& rhs) {
        return { lhs.mData[0] - rhs.mData[0], lhs.mData[1] - rhs.mData[1] };
    }

    /*  _________________________________________________________________________ */
    /*! operator*

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side vector.

    @return Vec2
    Product of the two vectors.

    Computes the product of two vectors.
    */
    Vec2 operator*(Vec2 const& lhs, Vec2 const& rhs) {
        return { lhs.mData[0] * rhs.mData[0], lhs.mData[1] * rhs.mData[1] };
    }

    /*  _________________________________________________________________________ */
    /*! operator+

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side float.

    @return Vec2
    Addition between vector and a float

    Computes the addition of a vector and a float
    */
    Vec2 operator+(Vec2 const& lhs, float val){
        return Vec2(lhs.mData[0] + val, lhs.mData[1] + val);
    }
    /*  _________________________________________________________________________ */
    /*! operator-

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side float.

    @return Vec2
    Subtraction between vector and a float

    Computes the Subtraction of a vector and a float
    */
    Vec2 operator-(Vec2 const& lhs, float val){
        return { lhs.mData[0] - val, lhs.mData[1] - val };
    }

    /*  _________________________________________________________________________ */
    /*! operator*

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side float.

    @return Vec2
    Product between vector and a float

    Computes the Product of a vector and a float
    */
    Vec2 operator*(Vec2 const& lhs, float val) {
        return { lhs.mData[0] * val, lhs.mData[1] * val };
    }

    /*  _________________________________________________________________________ */
    /*! operator/

    @param lhs
    Left-hand side vector.

    @param rhs
    Right-hand side float.

    @return Vec2
    Division between vector and a float

    Computes the Division of a vector and a float
    */
    Vec2 operator/(Vec2 const& lhs, float val) {
        assert((val != 0) && "Divide by 0");
        return { lhs.mData[0] / val, lhs.mData[1] / val };
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
    bool operator==(Vec2 const& lhs, Vec2 const& rhs) {
        return (lhs.mData[0] == rhs.mData[0] && lhs.mData[1] == rhs.mData[1]);
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
    std::ostream& operator<<(std::ostream& os, Vec2 const& rhs) {
        os << rhs.mData[0] << ", " << rhs.mData[1];
        return os;
    }

}