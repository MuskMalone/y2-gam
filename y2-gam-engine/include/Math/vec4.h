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
        Vec4& operator += (const Vec4& rhs);
        Vec4& operator -= (const Vec4& rhs);
        Vec4& operator *= (float rhs);
        Vec4& operator /= (float rhs);

        // Unary operators
        Vec4 operator -() const;
        float operator[](int idx)const;
        float& operator[](int idx);

    }Vec4;
        //float length() const;
        //float dot(Vec4 const& rhs) const;
        //Vec4 normalized() const;

        ////Overloads
        //Vec4 operator+(Vec4 const& rhs) const;
        //Vec4 operator-(Vec4 const& rhs) const;
        //Vec4 operator+(float val) const;
        //Vec4 operator-(float val) const;
        //Vec4 operator*(float val) const;
        //Vec4 operator/(float val) const;
        //Vec4& operator+=(Vec4 const& rhs);
        //Vec4& operator-=(Vec4 const& rhs);
        //Vec4& operator*=(float const& val);
        //Vec4& operator/=(float const& val);
        //bool operator==(Vec4 rhs);
        //float operator[](int idx)const;
        //float& operator[](int idx);

#ifdef _MSC_VER
// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
    // Overloads
    float length(Vec4 const& v);
    Vec4 normalized(Vec4 const& v);
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
