#include "Math\Vec4.h"

namespace Image {

	Vec4::Vec4() :mData{0,0,0,0} {}

	Vec4::Vec4(float val) :mData{val,val,val,val} {}

	Vec4::Vec4(float x, float y, float z, float w) : mData{x,y,z,w} {}

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
		return Vec4(mData[0] / len, mData[1] / len, mData[2] / len, mData[3] / len);
	}

	Vec4 Vec4::operator+(Vec4 const& rhs) const {
		return Vec4(mData[0] + rhs.mData[0], mData[1] + rhs.mData[1], mData[2] + rhs.mData[2], mData[3] + rhs.mData[3]);
	}

	Vec4 Vec4::operator-(Vec4 const& rhs) const {
		return Vec4(mData[0] - rhs.mData[0], mData[1] - rhs.mData[1], mData[2] - rhs.mData[2], mData[3] - rhs.mData[3]);
	}

	Vec4 Vec4::operator*(float val) const {
		return Vec4(mData[0] * val, mData[1] * val, mData[2] * val, mData[3] * val);
	}

	Vec4 Vec4::operator/(float val) const {
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
		return mData[idx];
	}

	float& Vec4::operator[](int idx) {
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
//namespace hemp {
//
//	Vec4::Vec4() :x{}, y{}, z{}, w{} {}
//
//	Vec4::Vec4(float val) :x{ val }, y{ val }, z{ val }, w{ val } {}
//
//	Vec4::Vec4(float x, float y, float z, float w) : x{ x }, y{ y }, z{ z }, w{ w } {}
//
//	float Vec4::length() const {
//		return std::sqrt(x * x + y * y + z * z + w * w);
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a float of the dot product between 2 Vectors
//	 */
//	 /**************************************************************************/
//	float Vec4::dot(Vec4 const& rhs) const {
//		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a normalise Vector
//	 */
//	 /**************************************************************************/
//	Vec4 Vec4::normalized() const {
//		float len = length();
//		return Vec4(x / len, y / len, z / len, w / len);
//	}
//
//	Vec4 Vec4::operator+(Vec4 const& rhs) const {
//		return Vec4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
//	}
//
//	Vec4 Vec4::operator-(Vec4 const& rhs) const {
//		return Vec4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
//	}
//
//	Vec4 Vec4::operator*(float val) const {
//		return Vec4(x * val, y * val, z * val, w * val);
//	}
//
//	Vec4 Vec4::operator/(float val) const {
//		return Vec4(x / val, y / val, z / val, w / val);
//	}
//
//	Vec4& Vec4::operator+=(Vec4 const& rhs) {
//		x += rhs.x;
//		y += rhs.y;
//		z += rhs.z;
//		w += rhs.w;
//		return *this;
//	}
//
//	Vec4& Vec4::operator-=(Vec4 const& rhs) {
//		x -= rhs.x;
//		y -= rhs.y;
//		z -= rhs.z;
//		w -= rhs.w;
//		return *this;
//	}
//
//	Vec4& Vec4::operator*=(float const& val) {
//		x *= val;
//		y *= val;
//		z *= val;
//		w *= val;
//		return *this;
//	}
//
//	Vec4& Vec4::operator/=(float const& val) {
//		x /= val;
//		y /= val;
//		z /= val;
//		w /= val;
//		return *this;
//	}
//
//	bool Vec4::operator==(Vec4 rhs) {
//		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
//	}
//
//	float Vec4::operator[](int idx) const{
//		if (idx == 0) {
//			return x;
//		}
//		else if (idx == 1) {
//			return y;
//		}
//		else if (idx == 2) {
//			return z;
//		}
//		else if (idx == 3) {
//			return w;
//		}
//		else {
//			try {
//				throw std::out_of_range("Index out of range");
//			}
//			catch (std::out_of_range const& e) {
//				// Handle the exception.
//				std::cerr << "Caught an exception: " << e.what() << std::endl;
//			}
//		}
//	}
//
//	float& Vec4::operator[](int idx){
//		if (idx == 0) {
//			return x;
//		}
//		else if (idx == 1) {
//			return y;
//		}
//		else if (idx == 2) {
//			return z;
//		}
//		else if (idx == 3) {
//			return w;
//		}
//		else {
//			try {
//				throw std::out_of_range("Index out of range");
//			}
//			catch (std::out_of_range const& e) {
//				// Handle the exception.
//				std::cerr << "Caught an exception: " << e.what() << std::endl;
//			}
//		}
//	}
//
//	/**************************************************************************/
//	/*!
//		overloaded << operator
//		prints x, y, z and endl in this order
//	 */
//	 /**************************************************************************/
//	std::ostream& operator<<(std::ostream& os, Vec4 const& rhs) {
//		os << rhs.x << ", " << rhs.y << ", " << rhs.z << ", " << rhs.w;
//		return os;
//	}
//
//}