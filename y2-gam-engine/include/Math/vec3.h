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
        Vec3& operator += (const Vec3& rhs);
        Vec3& operator -= (const Vec3& rhs);
        Vec3& operator *= (float rhs);
        Vec3& operator /= (float rhs);

        // Unary operators
        Vec3 operator -() const;
        float operator[](int idx)const;
        float& operator[](int idx);
    }Vec3;


        ////template<typename vec>
        ////Vec3(vec v) : mData{ v.x,v.y,v.z } {}

        ////template <typename vec>
        ////operator vec() {
        ////    return vec{ mData[0],mData[1],mData[2] };
        ////}
        //float length() const;
        //float dot(Vec3 const& rhs) const;
        //Vec3 cross(Vec3 const& rhs) const;
        //Vec3 normalized() const;

        ////Overloads
        //Vec3 operator+(Vec3 const& rhs) const;
        //Vec3 operator-(Vec3 const& rhs) const;
        //Vec3 operator+(float val) const;
        //Vec3 operator-(float val) const;
        //Vec3 operator*(float val) const;
        //Vec3 operator/(float val) const;
        //Vec3& operator+=(Vec3 const& rhs);
        //Vec3& operator-=(Vec3 const& rhs);
        //Vec3& operator*=(float const& val);
        //Vec3& operator/=(float const& val);
        //bool operator==(Vec3 rhs);
        //float operator[](int idx)const;
        //float& operator[](int idx);

    

#ifdef _MSC_VER
    // Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
    // Overloads
    float length(Vec3 const& v);
    Vec3 normalized(Vec3 const& v);
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