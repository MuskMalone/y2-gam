#pragma once
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <cassert>
#include "vec3.h"
#include "vec4.h"
namespace Image {
    class Vec3; // forward decl
    class Vec4; // forward decl

    class Vec2 {

    public:
        //Constructors
        Vec2();
        Vec2(float val);
        Vec2(float x, float y);
        Vec2(Vec3 const& rhs);
        Vec2(Vec4 const& rhs);
        //template<typename vec>
        //Vec2(vec v) : mData{ v.x,v.y } {}



        float length() const;
        float dot(Vec2 const& rhs) const;
        float cross(Vec2 const& rhs) const;
        Vec2 normalized() const;

        //Overloads
        Vec2 operator+(Vec2 const& rhs) const;
        Vec2 operator-(Vec2 const& rhs) const;
        Vec2 operator*(Vec2 const& rhs)const;
        Vec2 operator+(float val) const;
        Vec2 operator-(float val) const;
        Vec2 operator*(float val) const;
        Vec2 operator/(float val) const;
        Vec2& operator+=(Vec2 const& rhs);
        Vec2& operator-=(Vec2 const& rhs);
        Vec2& operator*=(float const& val);
        Vec2& operator/=(float const& val);
        bool operator==(Vec2 rhs);
        float operator[](int idx)const;
        float& operator[](int idx);
        //Members
        float mData[2];
    };
    std::ostream& operator<<(std::ostream& os, Vec2 const& v);
    float cross(Vec2 const& lhs, Vec2 const& rhs);
    float dot(Vec2 const& lhs, Vec2 const& rhs);

}
