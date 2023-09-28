#include "Math\Vec2.h"


namespace Image {

	Vec2::Vec2() :mData{0,0} {}

	Vec2::Vec2(float val) :mData{val,val} {}

	Vec2::Vec2(float x, float y) : mData{x,y} {}

	float Vec2::length() const {
		return std::sqrt(mData[0] * mData[0] + mData[1] * mData[1]);
	}

	/**************************************************************************/
	/*!
		returns a float of the dot product between 2 Vectors
	 */
	 /**************************************************************************/
	float Vec2::dot(Vec2 const& rhs) const {
		return mData[0] * rhs.mData[0] + mData[1] * rhs.mData[1];
	}

	/**************************************************************************/
	/*!
		returns a Vector of the cross product between 2 Vectors
	 */
	 /**************************************************************************/
	float Vec2::cross(Vec2 const& rhs) const {
		return mData[0] * rhs.mData[1] - rhs.mData[0] * mData[1];
	}

	/**************************************************************************/
	/*!
		returns a normalise Vector
	 */
	 /**************************************************************************/
	Vec2 Vec2::normalized() const {
		float len = length();
		assert((len != 0) && "Divide by 0");
		return Vec2(mData[0] / len, mData[1] / len);
	}

	Vec2 Vec2::operator+(Vec2 const& rhs) const {
		return Vec2(mData[0] + rhs.mData[0], mData[1] + rhs.mData[1]);
	}

	Vec2 Vec2::operator-(Vec2 const& rhs) const {
		return Vec2(mData[0] - rhs.mData[0], mData[1] - rhs.mData[1]);
	}

	Vec2 Vec2::operator*(float val) const {
		return Vec2(mData[0] * val, mData[1] * val);
	}

	Vec2 Vec2::operator/(float val) const {
		assert((val != 0) && "Divide by 0");
		return Vec2(mData[0] / val, mData[1] / val);
	}

	Vec2& Vec2::operator+=(Vec2 const& rhs) {
		mData[0] += rhs.mData[0];
		mData[1] += rhs.mData[1];
		return *this;
	}

	Vec2& Vec2::operator-=(Vec2 const& rhs) {
		mData[0] -= rhs.mData[0];
		mData[1] -= rhs.mData[1];
		return *this;
	}

	Vec2& Vec2::operator*=(float const& val) {
		mData[0] *= val;
		mData[1] *= val;
		return *this;
	}

	Vec2& Vec2::operator/=(float const& val) {
		assert((val != 0) && "Divide by 0");
		mData[0] /= val;
		mData[1] /= val;
		return *this;
	}

	bool Vec2::operator==(Vec2 rhs) {
		return (mData[0] == rhs.mData[0] && mData[1] == rhs.mData[1]);
	}
	float Vec2::operator[](int idx) const {
		assert((idx >= 0 && idx < 2) && "Ensure index is within bounds");
		return mData[idx];
	}

	float& Vec2::operator[](int idx) {
		assert((idx >= 0 && idx < 2) && "Ensure index is within bounds");
		return mData[idx];
	}
	/**************************************************************************/
	/*!
		overloaded << operator
		prints x, y, z and endl in this order
	 */
	 /**************************************************************************/
	std::ostream& operator<<(std::ostream& os, Vec2 const& rhs) {
		os << rhs.mData[0] << ", " << rhs.mData[1];
		return os;
	}

}

//namespace hemp {
//
//	Vec2::Vec2() :x{}, y{} {}
//
//	Vec2::Vec2(float val) :x{ val }, y{ val } {}
//
//	Vec2::Vec2(float x, float y) : x{ x }, y{ y } {}
//
//	float Vec2::length() const {
//		return std::sqrt(x * x + y * y);
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a float of the dot product between 2 Vectors
//	 */
//	 /**************************************************************************/
//	float Vec2::dot(Vec2 const& rhs) const {
//		return x * rhs.x + y * rhs.y;
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a Vector of the cross product between 2 Vectors
//	 */
//	 /**************************************************************************/
//	Vec2 Vec2::cross(Vec2 const& rhs) const {
//		return Vec2(x * rhs.y - rhs.x * y);
//	}
//
//	/**************************************************************************/
//	/*!
//		returns a normalise Vector
//	 */
//	 /**************************************************************************/
//	Vec2 Vec2::normalized() const {
//		float len = length();
//		return Vec2(x / len, y / len);
//	}
//
//	Vec2 Vec2::operator+(Vec2 const& rhs) const {
//		return Vec2(x + rhs.x, y + rhs.y);
//	}
//
//	Vec2 Vec2::operator-(Vec2 const& rhs) const {
//		return Vec2(x - rhs.x, y - rhs.y);
//	}
//
//	Vec2 Vec2::operator*(float val) const {
//		return Vec2(x * val, y * val);
//	}
//
//	Vec2 Vec2::operator/(float val) const {
//		return Vec2(x / val, y / val);
//	}
//
//	Vec2& Vec2::operator+=(Vec2 const& rhs) {
//		x += rhs.x;
//		y += rhs.y;
//		return *this;
//	}
//
//	Vec2& Vec2::operator-=(Vec2 const& rhs) {
//		x -= rhs.x;
//		y -= rhs.y;
//		return *this;
//	}
//
//	Vec2& Vec2::operator*=(float const& val) {
//		x *= val;
//		y *= val;
//		return *this;
//	}
//
//	Vec2& Vec2::operator/=(float const& val) {
//		x /= val;
//		y /= val;
//		return *this;
//	}
//
//	bool Vec2::operator==(Vec2 rhs) {
//		return (x == rhs.x && y == rhs.y);
//	}
//	float Vec2::operator[](int idx) const {
//		if (idx == 0) {
//			return x;
//		}
//		else if (idx == 1) {
//			return y;
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
//	float& Vec2::operator[](int idx) {
//		if (idx == 0) {
//			return x;
//		}
//		else if (idx == 1) {
//			return y;
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
//	/**************************************************************************/
//	/*!
//		overloaded << operator
//		prints x, y, z and endl in this order
//	 */
//	 /**************************************************************************/
//	std::ostream& operator<<(std::ostream& os, Vec2 const& rhs) {
//		os << rhs.x << ", " << rhs.y;
//		return os;
//	}
//
//}