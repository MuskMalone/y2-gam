#pragma once
#include "Vec2.h"
#include <iostream>
#include <Math.h>

namespace Image {
	class Mat22 {


	public:
		Mat22();
		Mat22(float const& val);
		Mat22(float _00, float _01, float _10, float _11);
		Mat22(Vec2 col1, Vec2 col2);

		Vec2 operator[](int idx)const;
		Vec2& operator[](int idx);
		bool operator==(Mat22 rhs);
		Mat22 operator+(Mat22 const& rhs);
		Mat22 operator-(Mat22 const& rhs);
		Mat22 operator*(Mat22 const& rhs);

		/* Overloading scalar operators */
		Mat22 operator+(float const& val);
		Mat22 operator-(float const& val);
		Mat22 operator*(float const& val);
		Mat22 operator/(float const& val);

		/* Overloading scalar shorthand operators */
		Mat22& operator+=(float const& val);
		Mat22& operator-=(float const& val);
		Mat22& operator*=(float const& val);
		Mat22& operator/=(float const& val);

		/* Overloading Matrix shorthand operators */
		Mat22& operator+=(Mat22 const& rhs);
		Mat22& operator-=(Mat22 const& rhs);


		Vec2 mMat[2];
	};

	void Mat22Identity(Mat22& results);
	//void Mat22Translate(Mat22& results, float x, float y);
	void Mat22Scale(Mat22& results, float x, float y);
	void Mat22RotRad(Mat22& results, float angle);
	void Mat22RotDeg(Mat22& results, float angle);
	void Mat22Transpose(Mat22& results, Mat22 const& pMtx);
	void Mat22Inverse(Mat22& results, Mat22 const& pMtx);
	std::ostream& operator<<(std::ostream& os, Mat22& m);
}

