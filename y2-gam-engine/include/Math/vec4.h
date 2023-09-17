#pragma once

#include <iostream>
#include <math.h>
#include <stdexcept>

namespace Image {
    class Vec4 {

    public:
        //Constructors
        Vec4();
        Vec4(float val);
        Vec4(float x, float y, float z, float w);

        float length() const;
        float dot(Vec4 const& rhs) const;
        Vec4 normalized() const;

        //Overloads
        Vec4 operator+(Vec4 const& rhs) const;
        Vec4 operator-(Vec4 const& rhs) const;
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
