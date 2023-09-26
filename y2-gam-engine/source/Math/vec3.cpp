#include "Math\Vec3.h"


namespace Image {
	Vec3::Vec3() :mData{0,0,0} {}

	Vec3::Vec3(float val) :mData{val,val,val} {}

	Vec3::Vec3(float x, float y, float z) : mData{x,y,z} {}

	float Vec3::length() const {
		return std::sqrt(mData[0] * mData[0] + mData[1] * mData[1] + mData[2] * mData[2]);
	}

	/**************************************************************************/
	/*!
		returns a float of the dot product between 2 Vectors
	 */
	 /**************************************************************************/
	float Vec3::dot(Vec3 const& rhs) const {
		return mData[0] * rhs.mData[0] + mData[1] * rhs.mData[1] + mData[2] * rhs.mData[2];
	}

	/**************************************************************************/
	/*!
		returns a Vector of the cross product between 2 Vectors
	 */
	 /**************************************************************************/
	Vec3 Vec3::cross(Vec3 const& rhs) const {
		return Vec3(mData[1] * rhs.mData[2] - mData[2] * rhs.mData[1], mData[2] * rhs.mData[0] - mData[0] * rhs.mData[2], mData[0] * rhs.mData[1] - mData[1] * rhs.mData[0]);
	}

	/**************************************************************************/
	/*!
		returns a normalise Vector
	 */
	 /**************************************************************************/
	Vec3 Vec3::normalized() const {
		float len = length();
		assert((len != 0) && "Divide by 0");
		return Vec3(mData[0] / len, mData[1] / len, mData[2] / len);
	}

	Vec3 Vec3::operator+(Vec3 const& rhs) const {
		return Vec3(mData[0] + rhs.mData[0], mData[1] + rhs.mData[1], mData[2] + rhs.mData[2]);
	}

	Vec3 Vec3::operator-(Vec3 const& rhs) const {
		return Vec3(mData[0] - rhs.mData[0], mData[1] - rhs.mData[1], mData[2] - rhs.mData[2]);
	}

	Vec3 Vec3::operator*(float val) const {
		return Vec3(mData[0] * val, mData[1] * val, mData[2] * val);
	}

	Vec3 Vec3::operator/(float val) const {
		assert((val != 0) && "Divide by 0");
		return Vec3(mData[0] / val, mData[1] / val, mData[2] / val);
	}

	Vec3& Vec3::operator+=(Vec3 const& rhs) {
		mData[0] += rhs.mData[0];
		mData[1] += rhs.mData[1];
		mData[2] += rhs.mData[2];
		return *this;
	}

	Vec3& Vec3::operator-=(Vec3 const& rhs) {
		mData[0] -= rhs.mData[0];
		mData[1] -= rhs.mData[1];
		mData[2] -= rhs.mData[2];
		return *this;
	}

	Vec3& Vec3::operator*=(float const& val) {
		mData[0] *= val;
		mData[1] *= val;
		mData[2] *= val;
		return *this;
	}

	Vec3& Vec3::operator/=(float const& val) {
		assert((val != 0) && "Divide by 0");
		mData[0] /= val;
		mData[1] /= val;
		mData[2] /= val;
		return *this;
	}

	bool Vec3::operator==(Vec3 rhs) {
		return (mData[0] == rhs.mData[0] && mData[1] == rhs.mData[1] && mData[2] == rhs.mData[2]);
	}

	float Vec3::operator[](int idx) const {
		assert((idx >= 0 && idx < 3)&&"Ensure index is within bounds");
		return mData[idx];
	}

	float& Vec3::operator[](int idx) {
		assert((idx >= 0 && idx < 3) && "Ensure index is within bounds");
		return mData[idx];
	}

	/**************************************************************************/
	/*!
		overloaded << operator
		prints x, y, z and endl in this order
	 */
	 /**************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vec3 const& rhs) {
		os << rhs.mData[0] << ", " << rhs.mData[1] << ", " << rhs.mData[2];
		return os;
	}

}
