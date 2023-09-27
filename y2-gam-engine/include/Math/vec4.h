#pragma once

#include <iostream>
#include <math.h>
#include <stdexcept>
#include <cassert>
#include "vec3.h"
#include "vec2.h"
namespace Image {
    class Vec2; // forward decl
    class Vec3; // forward decl
    class Vec4 {

    public:
        //Constructors
        Vec4();
        Vec4(float val);
        Vec4(float x, float y, float z, float w);
        Vec4(Vec2 const& rhs);
        Vec4(Vec3 const& rhs);
        //template <typename vec>
        //Vec4(vec v) : mData{ v.x,v.y,v.z,v.w } {}

        //template <typename vec>
        //operator vec() const noexcept {
        //    vec out{};
        //    std::memcpy(&out, mData, sizeof(vec) / sizeof(float));
        //    return out;
        //}

        float length() const;
        float dot(Vec4 const& rhs) const;
        Vec4 normalized() const;

        //Overloads
        Vec4 operator+(Vec4 const& rhs) const;
        Vec4 operator-(Vec4 const& rhs) const;
        Vec4 operator+(float val) const;
        Vec4 operator-(float val) const;
        Vec4 operator*(float val) const;
        Vec4 operator/(float val) const;
        Vec4& operator+=(Vec4 const& rhs);
        Vec4& operator-=(Vec4 const& rhs);
        Vec4& operator*=(float const& val);
        Vec4& operator/=(float const& val);
        bool operator==(Vec4 rhs);
        float operator[](int idx)const;
        float& operator[](int idx);
        //Members
        float mData[4];
    };
    std::ostream& operator<<(std::ostream& os, Vec4 const& v);
}
