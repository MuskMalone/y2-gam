/******************************************************************************/
/*!
\par        Image Engine
\file       Mat3.h

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 4, 2023

\brief      Header file for Mat3 math

			This file contains the declaration for a Matrix 3 class.
			It provides various constructors, operators, and utility functions
			for matrix operations such as addition, subtraction, multiplication,
			transposition, and inversion.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include <iostream>
#include <Math.h>
#include "Vec3.h"

namespace Image {
	union Vec3;
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	typedef union Mat33 {
		struct {
			Vec3 col1, col2,col3;
		};
		Vec3 mMat[3];
		Mat33();
		Mat33(float const& val);
		Mat33(Mat33 const& rhs);
		Mat33(float _00, float _01, float _02, float _10, float _11, float _12, float _20, float _21, float _22);
		Mat33(Vec3 col1, Vec3 col2, Vec3 col3);
	}Mat33;
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
	bool operator==(Mat33 const& lhs, Mat33 const& rhs);
	Mat33 operator+(Mat33 const& lhs, Mat33 const& rhs);
	Mat33 operator-(Mat33 const& lhs, Mat33 const& rhs);
	Mat33 operator*(Mat33 const& lhs, Mat33 const& rhs);
	Mat33 operator+(Mat33 const& lhs, float const& val);
	Mat33 operator-(Mat33 const& lhs, float const& val);
	Mat33 operator*(Mat33 const& lhs, float const& val);
	Mat33 operator/(Mat33 const& lhs, float const& val);
	Mat33& operator+=(Mat33& lhs, Mat33 const& rhs);
	Mat33& operator-=(Mat33& lhs, Mat33 const& rhs);
	Vec3 operator*(Mat33 const& lhs,  Vec3 const& rhs);

	void Mat33Identity(Mat33& results);
	void Mat33Translate(Mat33& results, float x, float y);
	void Mat33Scale(Mat33& results, float x, float y);
	void Mat33RotRad(Mat33& results, float angle);
	void Mat33RotDeg(Mat33& results, float angle);
	void Mat33Transpose(Mat33& results, Mat33 const& pMtx);
	void Mat33Inverse(Mat33& results, Mat33 const& pMtx);
	std::ostream& operator<<(std::ostream& os, Mat33& m);
}
