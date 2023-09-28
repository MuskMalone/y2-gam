#include "Math/Vec2.h"


namespace Image {

	Vec2::Vec2() :mData{ 0,0 } {}

	Vec2::Vec2(float val) :mData{ val,val } {}

	Vec2::Vec2(float x, float y) : mData{ x,y } {}

	Vec2::Vec2(Vec2 const& rhs) : mData{ rhs.x,rhs.y } {}

	Vec2::Vec2(Vec3 const& rhs) :mData{ rhs.mData[0],rhs.mData[1] } {}

	Vec2::Vec2(Vec4 const& rhs) :mData{ rhs.mData[0],rhs.mData[1] } {}


    /**************************************************************************/
   /*!
       Addition equal operator
       Adds the rhs vector to its current vector
    */
    /**************************************************************************/
    Vec2& Vec2::operator += (const Vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    /**************************************************************************/
    /*!
        Subtraction equal operator
        Subtract the rhs vector from its current vector
     */
     /**************************************************************************/
    Vec2& Vec2::operator -= (const Vec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    /**************************************************************************/
    /*!
        Multiply equal operator
        Multiply the rhs vector to its current vector
     */
     /**************************************************************************/
    Vec2& Vec2::operator *= (float rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    /**************************************************************************/
    /*!
        Divide equal operator
        Divide the rhs vector with its current vector
     */
     /**************************************************************************/
    Vec2& Vec2::operator /= (float rhs) {
        x /= rhs;
        y /= rhs;
        return *this;
    }
    /**************************************************************************/
    /*!
        Unary operator
        Negates the vector components
     */
     /**************************************************************************/
    Vec2 Vec2::operator -() const {
        Vec2 temp{ *this };
        temp.x = -x;
        temp.y = -y;
        return temp;
    }
    
    float Vec2::operator[](int idx) const {
        assert((idx >= 0 && idx < 3) && "Ensure index is within bounds");
        return mData[idx];
    }

    float& Vec2::operator[](int idx) {
        assert((idx >= 0 && idx < 3) && "Ensure index is within bounds");
        return mData[idx];
    }
    // Non-member function implementations
    float length(Vec2 const& v) {
        return std::sqrt(v.mData[0] * v.mData[0] + v.mData[1] * v.mData[1]);
    }

    float dot(Vec2 const& lhs,  Vec2 const& rhs) {
        return lhs.mData[0] * rhs.mData[0] + lhs.mData[1] * rhs.mData[1];
    }

    float cross(Vec2 const& lhs, Vec2 const& rhs) {
        return lhs.mData[0] * rhs.mData[1] - lhs.mData[1]* rhs.mData[0];
    }
    Vec2 cross(Vec2 const& lhs, float const& rhs) {
        return lhs.mData[0] * rhs - rhs * lhs.mData[1];
    }
    Vec2 cross(float const& lhs, Vec2 const& rhs) {
        return lhs * rhs.mData[1] - rhs.mData[0] * lhs;
    }

    Vec2 normalized( Vec2 const& v) {
        float len = length(v);
        assert((len != 0) && "Divide by 0");
        return { v.mData[0] / len, v.mData[1] / len };
    }

    // Overloaded operators
    Vec2 operator+(Vec2 const& lhs, Vec2 const& rhs) {
        return { lhs.mData[0] + rhs.mData[0], lhs.mData[1] + rhs.mData[1] };
    }

    Vec2 operator-(Vec2 const& lhs, Vec2 const& rhs) {
        return { lhs.mData[0] - rhs.mData[0], lhs.mData[1] - rhs.mData[1] };
    }

    Vec2 operator*(Vec2 const& lhs, Vec2 const& rhs) {
        return { lhs.mData[0] * rhs.mData[0], lhs.mData[1] * rhs.mData[1] };
    }

    Vec2 operator+(Vec2 const& lhs, float val){
        return Vec2(lhs.mData[0] + val, lhs.mData[1] + val);
    }

    Vec2 operator-(Vec2 const& lhs, float val){
        return { lhs.mData[0] - val, lhs.mData[1] - val };
    }

    Vec2 operator*(Vec2 const& lhs, float val) {
        return { lhs.mData[0] * val, lhs.mData[1] * val };
    }

    Vec2 operator/(Vec2 const& lhs, float val) {
        assert((val != 0) && "Divide by 0");
        return { lhs.mData[0] / val, lhs.mData[1] / val };
    }

    bool operator==(Vec2 const& lhs, Vec2 const& rhs) {
        return (lhs.mData[0] == rhs.mData[0] && lhs.mData[1] == rhs.mData[1]);
    }


    std::ostream& operator<<(std::ostream& os, Vec2 const& rhs) {
        os << rhs.mData[0] << ", " << rhs.mData[1];
        return os;
    }

}