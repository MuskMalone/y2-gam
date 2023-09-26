#pragma once

#include <iostream>
#include <math.h>
#include <stdexcept>
#include <cassert>
namespace Image {
    class Vec2 {

    public:
        //Constructors
        Vec2();
        Vec2(float val);
        Vec2(float x, float y);

        float length() const;
        float dot(Vec2 const& rhs) const;
        float cross(Vec2 const& rhs) const;
        Vec2 normalized() const;

        //Overloads
        Vec2 operator+(Vec2 const& rhs) const;
        Vec2 operator-(Vec2 const& rhs) const;
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
}
