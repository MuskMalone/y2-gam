#pragma once

#include <iostream>
#include <math.h>
#include <stdexcept>

namespace Image {
    class Vec3 {

    public:
        //Constructors
        Vec3();
        Vec3(float val);
        Vec3(float x, float y, float z);

        float length() const;
        float dot(Vec3 const& rhs) const;
        Vec3 cross(Vec3 const& rhs) const;
        Vec3 normalized() const;

        //Overloads
        Vec3 operator+(Vec3 const& rhs) const;
        Vec3 operator-(Vec3 const& rhs) const;
        Vec3 operator*(float val) const;
        Vec3 operator/(float val) const;
        Vec3& operator+=(Vec3 const& rhs);
        Vec3& operator-=(Vec3 const& rhs);
        Vec3& operator*=(float const& val);
        Vec3& operator/=(float const& val);
        bool operator==(Vec3 rhs);
        float operator[](int idx)const;
        float& operator[](int idx);


        //Members
        float mData[3];
    };
    std::ostream& operator<<(std::ostream& os, Vec3 const& v);
}