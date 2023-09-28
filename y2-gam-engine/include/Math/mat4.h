#pragma once
#include <iostream>
#include <Math.h>
#include "Vec4.h"

namespace Image {
	union Vec4;
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	typedef union Mat44 {
		struct {
			Vec4 col1, col2,col3,col4;
		};
		Vec4 mMat[4];
		Mat44();
		Mat44(float const& val);
		Mat44(Mat44 const& rhs);
		Mat44(float _00, float _01, float _02, float _03,
			float _10, float _11, float _12, float _13,
			float _20, float _21, float _22, float _23,
			float _30, float _31, float _32, float _33);
		Mat44(Vec4 col1, Vec4 col2, Vec4 col3, Vec4 col4);
		Mat44 operator/=(float const& rhs);
	}Mat44;
	//	Vec4 operator[](int idx)const;
	//	Vec4& operator[](int idx);
	//	bool operator==(Mat44 rhs);
	//	Mat44 operator+(Mat44 const& rhs);
	//	Mat44 operator-(Mat44 const& rhs);
	//	Mat44 operator*(Mat44 const& rhs);

	//	/* Overloading scalar operators */
	//	Mat44 operator+(float const& val);
	//	Mat44 operator-(float const& val);
	//	Mat44 operator*(float const& val);
	//	Mat44 operator/(float const& val);

	//	/* Overloading scalar shorthand operators */
	//	Mat44& operator+=(float const& val);
	//	Mat44& operator-=(float const& val);
	//	Mat44& operator*=(float const& val);
	//	Mat44& operator/=(float const& val);

	//	/* Overloading Matrix shorthand operators */
	//	Mat44& operator+=(Mat44 const& rhs);
	//	Mat44& operator-=(Mat44 const& rhs);


	//	Vec4 mMat[4];
	//};
	// Overloaded operators
	bool operator==(const Mat44& lhs, const Mat44& rhs);
	Mat44 operator+(const Mat44& lhs, const Mat44& rhs);
	Mat44 operator-(const Mat44& lhs, const Mat44& rhs);
	Mat44 operator*(const Mat44& lhs, const Mat44& rhs);
	Mat44 operator+(const Mat44& lhs, float const& val);
	Mat44 operator-(const Mat44& lhs, float const& val);
	Mat44 operator*(const Mat44& lhs, float const& val);
	Mat44 operator/(const Mat44& lhs, float const& val);
	Mat44& operator+=(Mat44& lhs, const Mat44& rhs);
	Mat44& operator-=(Mat44& lhs, const Mat44& rhs);


	void Mat44Identity(Mat44& results);
	void Mat44Translate(Mat44& results, float x, float y, float z);
	void Mat44Scale(Mat44& results, float x, float y, float z);
	void Mat44RotRad(Mat44& results, float angle);
	void Mat44RotDeg(Mat44& results, float angle);
	void Mat44Transpose(Mat44& results, Mat44 const& pMtx);
	void Mat44Inverse(Mat44& results, Mat44 const& pMtx);
	std::ostream& operator<<(std::ostream& os, Mat44& m);
}

