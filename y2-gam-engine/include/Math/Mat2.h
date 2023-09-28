/******************************************************************************/
/*!
\par        Image Engine
\file       Mat2.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Header file for Mat2 math

			This file contains the declaration for a Matrix 2 class.
			It provides various constructors, operators, and utility functions
			for matrix operations such as addition, subtraction, multiplication,
			transposition, and inversion.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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
		
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif
	// Overloaded operators
	bool operator==(Mat22 const& lhs, Mat22 const& rhs);
	Mat22 operator+(Mat22 const& lhs, Mat22 const& rhs);
	Mat22 operator-(Mat22 const& lhs, Mat22 const& rhs);
	Mat22 operator*(Mat22 const& lhs, Mat22 const& rhs);
	Mat22 operator+(Mat22 const& lhs, float const& val);
	Mat22 operator-(Mat22 const& lhs, float const& val);
	Mat22 operator*(Mat22 const& lhs, float const& val);
	Mat22 operator/(Mat22 const& lhs, float const& val);
	Mat22& operator+=(Mat22& lhs, Mat22 const& rhs);
	Mat22& operator-=(Mat22& lhs, Mat22 const& rhs);
	Vec2 operator*(Mat22 const& lhs, Vec2 const& rhs);


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
