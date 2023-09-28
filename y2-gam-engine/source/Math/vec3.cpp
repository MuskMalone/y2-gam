#include "Math/Vec3.h"


namespace Image {
	Vec3::Vec3() :mData{ 0,0,0 } {}

	Vec3::Vec3(float val) :mData{ val,val,val } {}

	Vec3::Vec3(float x, float y, float z) : mData{ x,y,z } {}

	Vec3::Vec3(Vec3 const& rhs) :mData{ rhs.x,rhs.y,rhs.z } {}

	Vec3::Vec3(Vec2 const& rhs) : mData{ rhs.mData[0],rhs.mData[1],0.f } {}

	Vec3::Vec3(Vec4 const& rhs) : mData{ rhs.mData[0],rhs.mData[1],rhs.mData[2] } {}

	Vec3& Vec3::operator+=(const Vec3& rhs){
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& rhs){
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vec3& Vec3::operator*=(float rhs){
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	Vec3& Vec3::operator/=(float rhs){
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}



	//float Vec3::length() const {
	//	return std::sqrt(mData[0] * mData[0] + mData[1] * mData[1] + mData[2] * mData[2]);
	//}

	///**************************************************************************/
	///*!
	//	returns a float of the dot product between 2 Vectors
	// */
	// /**************************************************************************/
	//float Vec3::dot(Vec3 const& rhs) const {
	//	return mData[0] * rhs.mData[0] + mData[1] * rhs.mData[1] + mData[2] * rhs.mData[2];
	//}

	///**************************************************************************/
	///*!
	//	returns a Vector of the cross product between 2 Vectors
	// */
	// /**************************************************************************/
	//Vec3 Vec3::cross(Vec3 const& rhs) const {
	//	return Vec3(mData[1] * rhs.mData[2] - mData[2] * rhs.mData[1], mData[2] * rhs.mData[0] - mData[0] * rhs.mData[2], mData[0] * rhs.mData[1] - mData[1] * rhs.mData[0]);
	//}

	///**************************************************************************/
	///*!
	//	returns a normalise Vector
	// */
	// /**************************************************************************/
	//Vec3 Vec3::normalized() const {
	//	float len = length();
	//	assert((len != 0) && "Divide by 0");
	//	return Vec3(mData[0] / len, mData[1] / len, mData[2] / len);
	//}

	//Vec3 Vec3::operator+(Vec3 const& rhs) const {
	//	return Vec3(mData[0] + rhs.mData[0], mData[1] + rhs.mData[1], mData[2] + rhs.mData[2]);
	//}

	//Vec3 Vec3::operator-(Vec3 const& rhs) const {
	//	return Vec3(mData[0] - rhs.mData[0], mData[1] - rhs.mData[1], mData[2] - rhs.mData[2]);
	//}

	//Vec3 Vec3::operator+(float val)const {
	//	return Vec3(mData[0] + val, mData[1] + val, mData[2] + val);
	//}

	//Vec3 Vec3::operator-(float val)const {
	//	return Vec3(mData[0] - val, mData[1] - val, mData[2] - val);
	//}

	//Vec3 Vec3::operator*(float val) const {
	//	return Vec3(mData[0] * val, mData[1] * val, mData[2] * val);
	//}

	//Vec3 Vec3::operator/(float val) const {
	//	assert((val != 0) && "Divide by 0");
	//	return Vec3(mData[0] / val, mData[1] / val, mData[2] / val);
	//}

	//Vec3& Vec3::operator+=(Vec3 const& rhs) {
	//	mData[0] += rhs.mData[0];
	//	mData[1] += rhs.mData[1];
	//	mData[2] += rhs.mData[2];
	//	return *this;
	//}

	//Vec3& Vec3::operator-=(Vec3 const& rhs) {
	//	mData[0] -= rhs.mData[0];
	//	mData[1] -= rhs.mData[1];
	//	mData[2] -= rhs.mData[2];
	//	return *this;
	//}

	//Vec3& Vec3::operator*=(float const& val) {
	//	mData[0] *= val;
	//	mData[1] *= val;
	//	mData[2] *= val;
	//	return *this;
	//}

	//Vec3& Vec3::operator/=(float const& val) {
	//	assert((val != 0) && "Divide by 0");
	//	mData[0] /= val;
	//	mData[1] /= val;
	//	mData[2] /= val;
	//	return *this;
	//}

	//bool Vec3::operator==(Vec3 rhs) {
	//	return (mData[0] == rhs.mData[0] && mData[1] == rhs.mData[1] && mData[2] == rhs.mData[2]);
	//}

	Vec3 Vec3::operator-() const{
		Vec3 temp{ *this };
		temp.x = -x;
		temp.y = -y;
		temp.z = -z;
		return temp;
	}

	float Vec3::operator[](int idx) const {
		assert((idx >= 0 && idx < 3) && "Ensure index is within bounds");
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

	float length(Vec3 const& v){
		return std::sqrt(v.mData[0] * v.mData[0] + v.mData[1] * v.mData[1] + v.mData[2] * v.mData[2]);
	}

	Vec3 normalized(Vec3 const& v){
		float len = length(v);
		assert((len != 0) && "Divide by 0");
		return { v.mData[0] / len, v.mData[1] / len,v.mData[2]/len};
	}

	Vec3 cross(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[1] * rhs.mData[2] - lhs.mData[2] * rhs.mData[1], lhs.mData[2] * rhs.mData[0] - lhs.mData[0] * rhs.mData[2], lhs.mData[0] * rhs.mData[1] - lhs.mData[1] * rhs.mData[0]);

	}

	float dot(Vec3 const& lhs, Vec3 const& rhs){
		return lhs.mData[0] * rhs.mData[0] + lhs.mData[1] * rhs.mData[1] + lhs.mData[2] * rhs.mData[2];
	}

	Vec3 operator+(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[0] + rhs.mData[0], lhs.mData[1] + rhs.mData[1], lhs.mData[2] + rhs.mData[2]);
	}

	Vec3 operator-(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[0] - rhs.mData[0], lhs.mData[1] - rhs.mData[1], lhs.mData[2] - rhs.mData[2]);
	}

	Vec3 operator*(Vec3 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mData[0] * rhs.mData[0], lhs.mData[1] * rhs.mData[1], lhs.mData[2] * rhs.mData[2]);
	}

	Vec3 operator+(Vec3 const& lhs, float val){
		return Vec3(lhs.mData[0] + val, lhs.mData[1] + val, lhs.mData[2] + val);
	}

	Vec3 operator-(Vec3 const& lhs, float val)	{
		return Vec3(lhs.mData[0] - val, lhs.mData[1] - val, lhs.mData[2] - val);
	}

	Vec3 operator*(Vec3 const& lhs, float val){
		return Vec3(lhs.mData[0] * val, lhs.mData[1] * val, lhs.mData[2] * val);
	}

	Vec3 operator/(Vec3 const& lhs, float val){
		return Vec3(lhs.mData[0] / val, lhs.mData[1] / val, lhs.mData[2] / val);
	}

	bool operator==(Vec3 const& lhs, Vec3 const& rhs){
		return (lhs. mData[0] == rhs.mData[0] && lhs.mData[1] == rhs.mData[1] && lhs.mData[2] == rhs.mData[2]);
	}

}
