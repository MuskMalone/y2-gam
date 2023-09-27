#include "Math/Vec4.h"

namespace Image {

	Vec4::Vec4() :mData{0,0,0,0} {}

	Vec4::Vec4(float val) :mData{val,val,val,val} {}

	Vec4::Vec4(float x, float y, float z, float w) : mData{x,y,z,w} {}

	Vec4::Vec4(Vec2 const& rhs) : mData{ rhs.mData[0],rhs.mData[1],0.f,0.f } {}

	Vec4::Vec4(Vec3 const& rhs) : mData{rhs.mData[0],rhs.mData[1],rhs.mData[2],0.f} {}



	float Vec4::length() const {
		return std::sqrt(mData[0] * mData[0] + mData[1] * mData[1] + mData[2] * mData[2] + mData[3] * mData[3]);
	}

	/**************************************************************************/
	/*!
		returns a float of the dot product betmData[3]een 2 Vectors
	 */
	 /**************************************************************************/
	float Vec4::dot(Vec4 const& rhs) const {
		return mData[0] * rhs.mData[0] + mData[1] * rhs.mData[1] + mData[2] * rhs.mData[2] + mData[3] * rhs.mData[3];
	}

	/**************************************************************************/
	/*!
		returns a normalise Vector
	 */
	 /**************************************************************************/
	Vec4 Vec4::normalized() const {
		float len = length();
		assert((len != 0) && "Divide by 0");
		return Vec4(mData[0] / len, mData[1] / len, mData[2] / len, mData[3] / len);
	}

	Vec4 Vec4::operator+(Vec4 const& rhs) const {
		return Vec4(mData[0] + rhs.mData[0], mData[1] + rhs.mData[1], mData[2] + rhs.mData[2], mData[3] + rhs.mData[3]);
	}

	Vec4 Vec4::operator-(Vec4 const& rhs) const {
		return Vec4(mData[0] - rhs.mData[0], mData[1] - rhs.mData[1], mData[2] - rhs.mData[2], mData[3] - rhs.mData[3]);
	}

	Vec4 Vec4::operator+(float val) const{
		return Vec4(mData[0] + val, mData[1] + val, mData[2] + val,mData[3]+val);
	}

	Vec4 Vec4::operator-(float val) const {
		return Vec4(mData[0] - val, mData[1] - val, mData[2] - val, mData[3] - val);
	}

	Vec4 Vec4::operator*(float val) const {
		return Vec4(mData[0] * val, mData[1] * val, mData[2] * val, mData[3] * val);
	}

	Vec4 Vec4::operator/(float val) const {
		assert((val != 0) && "Divide by 0");

		return Vec4(mData[0] / val, mData[1] / val, mData[2] / val, mData[3] / val);
	}

	Vec4& Vec4::operator+=(Vec4 const& rhs) {
		mData[0] += rhs.mData[0];
		mData[1] += rhs.mData[1];
		mData[2] += rhs.mData[2];
		mData[3] += rhs.mData[3];
		return *this;
	}

	Vec4& Vec4::operator-=(Vec4 const& rhs) {
		mData[0] -= rhs.mData[0];
		mData[1] -= rhs.mData[1];
		mData[2] -= rhs.mData[2];
		mData[3] -= rhs.mData[3];
		return *this;
	}

	Vec4& Vec4::operator*=(float const& val) {
		mData[0] *= val;
		mData[1] *= val;
		mData[2] *= val;
		mData[3] *= val;
		return *this;
	}

	Vec4& Vec4::operator/=(float const& val) {
		assert((val != 0) && "Divide by 0");
		mData[0] /= val;
		mData[1] /= val;
		mData[2] /= val;
		mData[3] /= val;
		return *this;
	}

	bool Vec4::operator==(Vec4 rhs) {
		return (mData[0] == rhs.mData[0] && mData[1] == rhs.mData[1] && mData[2] == rhs.mData[2] && mData[3] == rhs.mData[3]);
	}

	float Vec4::operator[](int idx) const {
		assert((idx >= 0 && idx < 4) && "Ensure index is within bounds");
		return mData[idx];
	}

	float& Vec4::operator[](int idx) {
		assert((idx >= 0 && idx < 4) && "Ensure index is within bounds");
		return mData[idx];
	}

	/**************************************************************************/
	/*!
		overloaded << operator
		prints mData[0], mData[1], mData[2] and endl in this order
	 */
	 /**************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vec4 const& rhs) {
		os << rhs.mData[0] << ", " << rhs.mData[1] << ", " << rhs.mData[2] << ", " << rhs.mData[3];
		return os;
	}

}
