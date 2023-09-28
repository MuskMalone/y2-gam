#pragma once
#include "Vec2.h"
#include <iostream>
#include <Math.h>

namespace Image {
	union Vec2;
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	typedef union Mat22 {
		struct {
			Vec2 col1, col2;
		};
		Vec2 mMat[2];
		Mat22();
		Mat22(Mat22 const& rhs);
		Mat22(float const& val);
		Mat22(float a00, float a01,float a10, float a11);
		Mat22(Vec2 col1, Vec2 col2);
		} Mat22;
		


	// Overloaded operators
	bool operator==(const Mat22& lhs, const Mat22& rhs);
	Mat22 operator+(const Mat22& lhs, const Mat22& rhs);
	Mat22 operator-(const Mat22& lhs, const Mat22& rhs);
	Mat22 operator*(const Mat22& lhs, const Mat22& rhs);
	Mat22 operator+(const Mat22& lhs, float const& val);
	Mat22 operator-(const Mat22& lhs, float const& val);
	Mat22 operator*(const Mat22& lhs, float const& val);
	Mat22 operator/(const Mat22& lhs, float const& val);
	Mat22& operator+=(Mat22& lhs, const Mat22& rhs);
	Mat22& operator-=(Mat22& lhs, const Mat22& rhs);
	Vec2 operator*(const Mat22& lhs, const Vec2& rhs);


	void Mat22Identity(Mat22& results);
	//void Mat22Translate(Mat22& results, float x, float y);
	void Mat22Scale(Mat22& results, float x, float y);
	void Mat22RotRad(Mat22& results, float angle);
	void Mat22RotDeg(Mat22& results, float angle);
	void Mat22Transpose(Mat22& results, Mat22 const& pMtx);
	Mat22& Mat22Transpose( Mat22 const& pMtx);
	void Mat22Inverse(Mat22& results, Mat22 const& pMtx);
	std::ostream& operator<<(std::ostream& os, Mat22& m);
}



//Vec2 operator[](int idx)const;
//Vec2& operator[](int idx);
//bool operator==(Mat22 rhs);
//Mat22 operator+(Mat22 const& rhs)const;
//Mat22 operator-(Mat22 const& rhs)const;
//Mat22 operator*(Mat22 const& rhs)const;

///* Overloading scalar operators */
//Mat22 operator+(float const& val)const;
//Mat22 operator-(float const& val)const;
//Mat22 operator*(float const& val)const;
//Mat22 operator/(float const& val)const;

///* Overloading scalar shorthand operators */
//Mat22& operator+=(float const& val);
//Mat22& operator-=(float const& val);
//Mat22& operator*=(float const& val);
//Mat22& operator/=(float const& val);

///* Overloading Matrix shorthand operators */
//Mat22& operator+=(Mat22 const& rhs);
//Mat22& operator-=(Mat22 const& rhs);

//Vec2 operator*(Vec2 const& rhs) const;

//Vec2 mMat[2];
