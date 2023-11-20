/******************************************************************************/
/*!
\par        Image Engine
\file       Vec3.h

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Header file for Vec3 math

            This file contains the declaration for a vector 3 class.
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
#include "Vec2.h"
#include "Vec4.h"
namespace Image {
    union Vec2;
    union Vec4;
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    typedef union Vec3
    {
        struct
        {
            float x, y, z;
        };

        float mData[3];
        //Constructors
        Vec3();
        Vec3(float val);
        Vec3(float x, float y, float z);
        Vec3(Vec2 const& rhs);
        Vec3(Vec3 const& rhs);
        Vec3(Vec4 const& rhs);

        // Assignment operators
        Vec3& operator += (Vec3 const& rhs);
        Vec3& operator -= (Vec3 const& rhs);
        Vec3& operator *= (float rhs);
        Vec3& operator /= (float rhs);

        // Unary operators
        Vec3 operator -() const;
        float operator[](int idx)const;
        float& operator[](int idx);
    }Vec3;

#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
    // Overloads
    float length(Vec3 const& rhs);
    Vec3 normalized(Vec3 const& rhs);
    Vec3 cross(Vec3 const& lhs, Vec3 const& rhs);
    float dot(Vec3 const& lhs, Vec3 const& rhs);
    Vec3 operator+(Vec3 const& lhs, Vec3 const& rhs);
    Vec3 operator-(Vec3 const& lhs, Vec3 const& rhs);
    Vec3 operator*(Vec3 const& lhs, Vec3 const& rhs);
    Vec3 operator+(Vec3 const& lhs, float val);
    Vec3 operator-(Vec3 const& lhs, float val);
    Vec3 operator*(Vec3 const& lhs, float val);
    Vec3 operator/(Vec3 const& lhs, float val);
    bool operator==(Vec3 const& lhs, Vec3 const& rhs);

    std::ostream& operator<<(std::ostream& os, Vec3 const& v);


}