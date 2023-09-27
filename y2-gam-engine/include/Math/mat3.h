#pragma once

#include <iostream>
#include <Math.h>
#include "Vec3.h"

namespace Image {
	class Mat33 {

	public:
		Mat33();
		Mat33(float const& val);
		Mat33(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21, float _22);
		Mat33(Vec3 col1, Vec3 col2, Vec3 col3);

		Vec3 operator[](int idx)const;
		Vec3& operator[](int idx);
		bool operator==(Mat33 rhs);
		Mat33 operator+(Mat33 const& rhs);
		Mat33 operator-(Mat33 const& rhs);
		Mat33 operator*(Mat33 const& rhs);

		/* Overloading scalar operators */
		Mat33 operator+(float const& val);
		Mat33 operator-(float const& val);
		Mat33 operator*(float const& val);
		Mat33 operator/(float const& val);

		/* Overloading scalar shorthand operators */
		Mat33& operator+=(float const& val);
		Mat33& operator-=(float const& val);
		Mat33& operator*=(float const& val);
		Mat33& operator/=(float const& val);

		/* Overloading Matrix shorthand operators */
		Mat33& operator+=(Mat33 const& rhs);
		Mat33& operator-=(Mat33 const& rhs);

		Vec3 operator*(Vec3 const& rhs);


		Vec3 mMat[3];
	};

	void Mat33Identity(Mat33& results);
	void Mat33Translate(Mat33& results, float x, float y);
	void Mat33Scale(Mat33& results, float x, float y);
	void Mat33RotRad(Mat33& results, float angle);
	void Mat33RotDeg(Mat33& results, float angle);
	void Mat33Transpose(Mat33& results, Mat33 const& pMtx);
	void Mat33Inverse(Mat33& results, Mat33 const& pMtx);
	std::ostream& operator<<(std::ostream& os, Mat33& m);
}
