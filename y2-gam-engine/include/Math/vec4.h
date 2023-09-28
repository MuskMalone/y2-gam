/******************************************************************************/
/*!
\par        Image Engine
\file       Vec4.hpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Header file for Vec4 math

            This file contains the declaration for a vector 4 class.
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
#include "Vec2.h"
namespace Image {
    union Vec3;
    union Vec2;
#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
    typedef union Vec4
    {
        struct
        {
            float x, y, z,w;
        };

        float mData[4];
        //Constructors
        Vec4();
        Vec4(float val);
        Vec4(float x, float y, float z, float w);
        Vec4(Vec2 const& rhs);
        Vec4(Vec3 const& rhs);
        Vec4(Vec4 const& rhs);
        
        // Assignment operators
        Vec4& operator += (Vec4 const& rhs);
        Vec4& operator -= (Vec4 const& rhs);
        Vec4& operator *= (float rhs);
        Vec4& operator /= (float rhs);

        // Unary operators
        Vec4 operator -() const;
        float operator[](int idx)const;
        float& operator[](int idx);

    }Vec4;

#ifdef _MSC_VER
// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
    // Overloads
    float length(Vec4 const& rhs);
    Vec4 normalized(Vec4 const& rhs);
    float dot(Vec4 const& lhs, Vec4 const& rhs);
    Vec4 operator+(Vec4 const& lhs, Vec4 const& rhs);
    Vec4 operator-(Vec4 const& lhs, Vec4 const& rhs);
    Vec4 operator*(Vec4 const& lhs, Vec4 const& rhs);
    Vec4 operator+(Vec4 const& lhs, float val);
    Vec4 operator-(Vec4 const& lhs, float val);
    Vec4 operator*(Vec4 const& lhs, float val);
    Vec4 operator/(Vec4 const& lhs, float val);
    bool operator==(Vec4 const& lhs, Vec4 const& rhs);

    std::ostream& operator<<(std::ostream& os, Vec4 const& v);
}
