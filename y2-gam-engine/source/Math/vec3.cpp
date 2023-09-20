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
		mData[0] /= val;
		mData[1] /= val;
		mData[2] /= val;
		return *this;
	}

	bool Vec3::operator==(Vec3 rhs) {
		return (mData[0] == rhs.mData[0] && mData[1] == rhs.mData[1] && mData[2] == rhs.mData[2]);
	}

	float Vec3::operator[](int idx) const {
		return mData[idx];
	}

	float& Vec3::operator[](int idx) {
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

//namespace hemp {
//	Vec3::Vec3() :x{}, y{}, z{} {}
//
//	Vec3::Vec3(float val) :x{ val }, y{ val }, z{ val } {}
//
//	Vec3::Vec3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
//
//	float Vec3::length() const {
//		return std::sqrt(x * x + y * y + z * z);
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a float of the dot product between 2 Vectors
//	 */
//	 /**************************************************************************/
//	float Vec3::dot(Vec3 const& rhs) const {
//		return x * rhs.x + y * rhs.y + z * rhs.z;
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a Vector of the cross product between 2 Vectors
//	 */
//	 /**************************************************************************/
//	Vec3 Vec3::cross(Vec3 const& rhs) const {
//		return Vec3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a normalise Vector
//	 */
//	 /**************************************************************************/
//	Vec3 Vec3::normalized() const {
//		float len = length();
//		return Vec3(x / len, y / len, z / len);
//	}
//
//	Vec3 Vec3::operator+(Vec3 const& rhs) const {
//		return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
//	}
//
//	Vec3 Vec3::operator-(Vec3 const& rhs) const {
//		return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
//	}
//
//	Vec3 Vec3::operator*(float val) const {
//		return Vec3(x * val, y * val, z * val);
//	}
//
//	Vec3 Vec3::operator/(float val) const {
//		return Vec3(x / val, y / val, z / val);
//	}
//
//	Vec3& Vec3::operator+=(Vec3 const& rhs) {
//		x += rhs.x;
//		y += rhs.y;
//		z += rhs.z;
//		return *this;
//	}
//
//	Vec3& Vec3::operator-=(Vec3 const& rhs) {
//		x -= rhs.x;
//		y -= rhs.y;
//		z -= rhs.z;
//		return *this;
//	}
//
//	Vec3& Vec3::operator*=(float const& val) {
//		x *= val;
//		y *= val;
//		z *= val;
//		return *this;
//	}
//
//	Vec3& Vec3::operator/=(float const& val) {
//		x /= val;
//		y /= val;
//		z /= val;
//		return *this;
//	}
//
//	bool Vec3::operator==(Vec3 rhs) {
//		return (x == rhs.x && y == rhs.y && z == rhs.z);
//	}
//
//	float Vec3::operator[](int idx) const {
//		if (idx == 0) {
//			return x;
//		}
//		else if (idx == 1) {
//			return y;
//		}
//		else if (idx == 2) {
//			return z;
//		}
//		else {
//			try {
//				throw std::out_of_range("Index out of range");
//			}
//			catch (std::out_of_range const& e) {
//				// Handle the exception.
//				std::cerr << "Caught an exception: " << e.what() << std::endl;
//			}
//			return NAN;
//		}
//	}
//
//	float& Vec3::operator[](int idx) {
//		if (idx == 0) {
//			return x;
//		}
//		else if (idx == 1) {
//			return y;
//		}
//		else if (idx == 2) {
//			return z;
//		}
//		else {
//			// not sure about this parts
//			try {
//				throw std::out_of_range("Index out of range");
//			}
//			catch (std::out_of_range const& e) {
//				// Handle the exception.
//				std::cerr << "Caught an exception: " << e.what() << std::endl;
//			}
//			return x;
//		}
//	}
//
//	/**************************************************************************/
//	/*!
//		overloaded << operator
//		prints x, y, z and endl in this order
//	 */
//	 /**************************************************************************/
//	std::ostream& operator<<(std::ostream& os, Vec3 const& rhs) {
//		os << rhs.x << ", " << rhs.y << ", " << rhs.z;
//		return os;
//	}
//
//}