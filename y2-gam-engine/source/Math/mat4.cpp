/******************************************************************************/
/*!
\par        Image Engine
\file       Mat4.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 4, 2023

\brief      Implementation file for Mat4 math

			This file contains the Implementation for a Matrix 4 class.
			It provides various constructors, operators, and utility functions
			for matrix operations such as addition, subtraction, multiplication,
			transposition, and inversion.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Math/MathUtils.h"
namespace Image {
	/*  _________________________________________________________________________ */
	/*! Mat44

	@param none.

	@return none.

	Default constructor. Set the values to 0
	*/
	Mat44::Mat44() :mMat{} {}

	/*  _________________________________________________________________________ */
	/*! Mat44

	@param val
	Value for the Matrix

	@return none.

	Constructor that sets both values to val
	*/
	Mat44::Mat44(float const& val) :mMat{ val,val,val,val } {}

	/*  _________________________________________________________________________ */
	/*! Mat44

	@param rhs
	Matrix to copy from

	@return none.

	Copy constructor.
	*/
	Mat44::Mat44(Mat44 const& rhs) : mMat{rhs.mMat[0],rhs.mMat[1],rhs.mMat[2],rhs.mMat[3]} {}

	/*  _________________________________________________________________________ */
	/*! Mat44

	@param  a00,  a01,  a02, a03, a10,  a11,  a12, a13, a20,  a21,  a22, a23, 
			a30, a31, a32, a33
	Matrix Elements

	@return none.

	Constructor to initialize matrix with given values.
	*/
	Mat44::Mat44(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33) : mMat{ Vec4(a00,a01,a02,a03) ,
															Vec4(a10,a11,a12,a13) ,
															Vec4(a20,a21,a22,a23) ,
															Vec4(a30,a31,a32,a33) } {}

	/*  _________________________________________________________________________ */
	/*! Mat44

	@param col1
	First column of the matrix

	@param col2
	Second column of the matrix

	@param col3
	Third column of the matrix

	@param col4
	Fourth column of the matrix

	@return none.

	Constructor to initialize matrix with 3 vectors.
	*/
	Mat44::Mat44(Vec4 col1, Vec4 col2, Vec4 col3, Vec4 col4) : mMat{ col1, col2, col3,col4 } {}

	/*  _________________________________________________________________________ */
	/*! operator/=

	@param rhs
	Scalar value to divide the matrix by.

	@return Mat44
	Modified matrix.

	Divides each element of the matrix by the given scalar value.
	*/
	Mat44 Mat44::operator/=(float const& rhs){
		mMat[0] /= rhs;
		mMat[1] /= rhs;
		mMat[2] /= rhs;
		mMat[3] /= rhs;
		return *this;
	}

	/*  _________________________________________________________________________ */
	/*! Mat44Identity

	@param results
	Matrix to be set to identity.

	@return none.

	Sets the matrix to identity.
	*/
	void Mat44Identity(Mat44& results) {
		results = { Vec4(1.f,0.f,0.f,0.f) ,
					Vec4(0.f,1.f,0.f,0.f) ,
					Vec4(0.f,0.f,1.f,0.f) ,
					Vec4(0.f,0.f,0.f,1.f) };
	}
	/*  _________________________________________________________________________ */
	/*! Mat44Translate

	@param results
	Matrix to be set to identity.

	@param x
	Translate factor for the x-axis.

	@param y
	Translate factor for the y-axis.

	@param z
	Translate factor for the z-axis.

	@return none.

	Translate the matrix by its x and y.
	*/
	void Mat44Translate(Mat44& results, float x, float y, float z) {
		results = { Vec4(1.f,0.f,0.f,x) ,
					Vec4(0.f,1.f,0.f,y) ,
					Vec4(0.f,0.f,1.f,z) ,
					Vec4(0.f,0.f,0.f,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat44Scale

	@param results
	Matrix to be scaled.

	@param x
	Scaling factor for the x-axis.

	@param y
	Scaling factor for the y-axis.

	@param z
	Translate factor for the z-axis.

	@return none.

	Scales the matrix by given factors.
	*/
	void Mat44Scale(Mat44& results, float x, float y, float z) {
		results = { Vec4(x,0.f,0.f,0.f) ,
					Vec4(0.f,y,0.f,0.f) ,
					Vec4(0.f,0.f,z,0.f) ,
					Vec4(0.f,0.f,0.f,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat44RotRad

	@param results
	Matrix to be rotated.

	@param angle
	Rotation angle in radians.

	@return none.

	Rotates the matrix by a given angle in radians.
	*/
	void Mat44RotRad(Mat44& results, float angle) {
		results = { Vec4(cosf(angle),-sinf(angle),0.f,0.f) , Vec4(sinf(angle),cosf(angle),0.f,0.f) , Vec4(0.f,0.f,1.f,0.f), Vec4(0.f,0.f,0.f,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat44RotDeg

	@param results
	Matrix to be rotated.

	@param angle
	Rotation angle in degrees.

	@return none.

	Rotates the matrix by a given angle in degrees.
	*/
	void Mat44RotDeg(Mat44& results, float angle) {
		//Find the anagle in rad
		float rad = angle * static_cast<float>(gPI) / 180.f;
		Mat44RotRad(results, rad);
	}

	/*  _________________________________________________________________________ */
	/*! Mat44Transpose

	@param results
	Matrix to store the transposed result.

	@param Mtx
	Matrix to be transposed.

	@return none.

	Transposes the given matrix.
	*/
	void Mat44Transpose(Mat44& results, Mat44 const& pMtx) {
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				results.mMat[i][j] = pMtx.mMat[j][i];
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! Mat44Inverse

	@param results
	Matrix to store the inverted result.

	@param Mtx
	Matrix to be inverted.

	@return none.

	Inverts the given matrix.
	*/
	void Mat44Inverse(Mat44& results, Mat44 const& Mtx) {
		Mat44 toTrans{};
		Mat44Transpose(toTrans, Mtx);
		float data00 = toTrans.mMat[2][2] * toTrans.mMat[3][3] - toTrans.mMat[3][2] * toTrans.mMat[2][3];
		float data01 = toTrans.mMat[2][1] * toTrans.mMat[3][3] - toTrans.mMat[3][1] * toTrans.mMat[2][3];
		float data02 = toTrans.mMat[2][1] * toTrans.mMat[3][2] - toTrans.mMat[3][1] * toTrans.mMat[2][2];
		float data03 = toTrans.mMat[2][0] * toTrans.mMat[3][3] - toTrans.mMat[3][0] * toTrans.mMat[2][3];
		float data04 = toTrans.mMat[2][0] * toTrans.mMat[3][2] - toTrans.mMat[3][0] * toTrans.mMat[2][2];
		float data05 = toTrans.mMat[2][0] * toTrans.mMat[3][1] - toTrans.mMat[3][0] * toTrans.mMat[2][1];
		float data06 = toTrans.mMat[1][2] * toTrans.mMat[3][3] - toTrans.mMat[3][2] * toTrans.mMat[1][3];
		float data07 = toTrans.mMat[1][1] * toTrans.mMat[3][3] - toTrans.mMat[3][1] * toTrans.mMat[1][3];
		float data08 = toTrans.mMat[1][1] * toTrans.mMat[3][2] - toTrans.mMat[3][1] * toTrans.mMat[1][2];
		float data09 = toTrans.mMat[1][0] * toTrans.mMat[3][3] - toTrans.mMat[3][0] * toTrans.mMat[1][3];
		float data10 = toTrans.mMat[1][0] * toTrans.mMat[3][2] - toTrans.mMat[3][0] * toTrans.mMat[1][2];
		float data11 = toTrans.mMat[1][0] * toTrans.mMat[3][1] - toTrans.mMat[3][0] * toTrans.mMat[1][1];
		float data12 = toTrans.mMat[1][2] * toTrans.mMat[2][3] - toTrans.mMat[2][2] * toTrans.mMat[1][3];
		float data13 = toTrans.mMat[1][1] * toTrans.mMat[2][3] - toTrans.mMat[2][1] * toTrans.mMat[1][3];
		float data14 = toTrans.mMat[1][1] * toTrans.mMat[2][2] - toTrans.mMat[2][1] * toTrans.mMat[1][2];
		float data15 = toTrans.mMat[1][0] * toTrans.mMat[2][3] - toTrans.mMat[2][0] * toTrans.mMat[1][3];
		float data16 = toTrans.mMat[1][0] * toTrans.mMat[2][2] - toTrans.mMat[2][0] * toTrans.mMat[1][2];
		float data17 = toTrans.mMat[1][0] * toTrans.mMat[2][1] - toTrans.mMat[2][0] * toTrans.mMat[1][1];

		Mat44 temp{};
		temp.mMat[0][0] = +(toTrans.mMat[1][1] * data00 - toTrans.mMat[1][2] * data01 + toTrans.mMat[1][3] * data02);
		temp.mMat[0][1] = -(toTrans.mMat[1][0] * data00 - toTrans.mMat[1][2] * data03 + toTrans.mMat[1][3] * data04);
		temp.mMat[0][2] = +(toTrans.mMat[1][0] * data01 - toTrans.mMat[1][1] * data03 + toTrans.mMat[1][3] * data05);
		temp.mMat[0][3] = -(toTrans.mMat[1][0] * data02 - toTrans.mMat[1][1] * data04 + toTrans.mMat[1][2] * data05);

		temp.mMat[1][0] = -(toTrans.mMat[0][1] * data00 - toTrans.mMat[0][2] * data01 + toTrans.mMat[0][3] * data02);
		temp.mMat[1][1] = +(toTrans.mMat[0][0] * data00 - toTrans.mMat[0][2] * data03 + toTrans.mMat[0][3] * data04);
		temp.mMat[1][2] = -(toTrans.mMat[0][0] * data01 - toTrans.mMat[0][1] * data03 + toTrans.mMat[0][3] * data05);
		temp.mMat[1][3] = +(toTrans.mMat[0][0] * data02 - toTrans.mMat[0][1] * data04 + toTrans.mMat[0][2] * data05);

		temp.mMat[2][0] = +(toTrans.mMat[0][1] * data06 - toTrans.mMat[0][2] * data07 + toTrans.mMat[0][3] * data08);
		temp.mMat[2][1] = -(toTrans.mMat[0][0] * data06 - toTrans.mMat[0][2] * data09 + toTrans.mMat[0][3] * data10);
		temp.mMat[2][2] = +(toTrans.mMat[0][0] * data07 - toTrans.mMat[0][1] * data09 + toTrans.mMat[0][3] * data11);
		temp.mMat[2][3] = -(toTrans.mMat[0][0] * data08 - toTrans.mMat[0][1] * data10 + toTrans.mMat[0][2] * data11);

		temp.mMat[3][0] = -(toTrans.mMat[0][1] * data12 - toTrans.mMat[0][2] * data13 + toTrans.mMat[0][3] * data14);
		temp.mMat[3][1] = +(toTrans.mMat[0][0] * data12 - toTrans.mMat[0][2] * data15 + toTrans.mMat[0][3] * data16);
		temp.mMat[3][2] = -(toTrans.mMat[0][0] * data13 - toTrans.mMat[0][1] * data15 + toTrans.mMat[0][3] * data17);
		temp.mMat[3][3] = +(toTrans.mMat[0][0] * data14 - toTrans.mMat[0][1] * data16 + toTrans.mMat[0][2] * data17);
		float Determinant =
			+toTrans.mMat[0][0] * temp.mMat[0][0]
			+ toTrans.mMat[0][1] * temp.mMat[0][1]
			+ toTrans.mMat[0][2] * temp.mMat[0][2]
			+ toTrans.mMat[0][3] * temp.mMat[0][3];

		temp /= Determinant;
		results = temp;
	}

	/*  _________________________________________________________________________ */
	/*! operator<<

	@param os
	Output stream.

	@param m
	Matrix to be printed.

	@return std::ostream&.
	Reference to the output stream.

	Prints the matrix to the output stream.
	*/
	std::ostream& operator<<(std::ostream& os, Mat44& m) {
		for (int i{}; i < 4; ++i) {
			os << m.mMat[0][i] << '\t' << m.mMat[1][i] << '\t' << m.mMat[2][i] <<'\t' << m.mMat[3][i] << std::endl;
		}
		return os;
	}

	/*  _________________________________________________________________________ */
	/*! operator==

	@param lhs
	Left-hand side matrix.

	@param rhs
	right-hand side matrix.

	@return bool
	True if matrices are equal, false otherwise.

	Compares two matrices for equality.
	*/
	bool operator==(Mat44 const& lhs, Mat44 const& rhs) {
		return (lhs.mMat[0] == rhs.mMat[0] && lhs.mMat[1] == rhs.mMat[1] && lhs.mMat[2] == rhs.mMat[2] && lhs.mMat[3] == rhs.mMat[3]);
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat4
	Resultant matrix after addition.

	Adds two matrices.
	*/
	Mat44 operator+(Mat44 const& lhs, Mat44 const& rhs) {
		return Mat44(lhs.mMat[0] + rhs.mMat[0], lhs.mMat[1] + rhs.mMat[1], lhs.mMat[2] + rhs.mMat[2], lhs.mMat[3] + rhs.mMat[3]);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat44
	Resultant matrix after subtraction.

	Subtracts one matrix from another.
	*/
	Mat44 operator-(Mat44 const& lhs, Mat44 const& rhs) {
		return Mat44(lhs.mMat[0] - rhs.mMat[0], lhs.mMat[1] - rhs.mMat[1], lhs.mMat[2] - rhs.mMat[2], lhs.mMat[3] - rhs.mMat[3]);

	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat44
	Resultant matrix after multiplication.

	Multiplies two matrices.
	*/
	Mat44 operator*(Mat44 const& lhs, Mat44 const& rhs) {
		Mat44 result;

		result.mMat[0][0] = lhs.mMat[0][0] * rhs.mMat[0][0] + lhs.mMat[1][0] * rhs.mMat[0][1] + lhs.mMat[2][0] * rhs.mMat[0][2] + lhs.mMat[3][0] * rhs.mMat[0][3];
		result.mMat[0][1] = lhs.mMat[0][1] * rhs.mMat[0][0] + lhs.mMat[1][1] * rhs.mMat[0][1] + lhs.mMat[2][1] * rhs.mMat[0][2] + lhs.mMat[3][1] * rhs.mMat[0][3];
		result.mMat[0][2] = lhs.mMat[0][2] * rhs.mMat[0][0] + lhs.mMat[1][2] * rhs.mMat[0][1] + lhs.mMat[2][2] * rhs.mMat[0][2] + lhs.mMat[3][2] * rhs.mMat[0][3];
		result.mMat[0][3] = lhs.mMat[0][3] * rhs.mMat[0][0] + lhs.mMat[1][3] * rhs.mMat[0][1] + lhs.mMat[2][3] * rhs.mMat[0][2] + lhs.mMat[3][3] * rhs.mMat[0][3];

		result.mMat[1][0] = lhs.mMat[0][0] * rhs.mMat[1][0] + lhs.mMat[1][0] * rhs.mMat[1][1] + lhs.mMat[2][0] * rhs.mMat[1][2] + lhs.mMat[3][0] * rhs.mMat[1][3];
		result.mMat[1][1] = lhs.mMat[0][1] * rhs.mMat[1][0] + lhs.mMat[1][1] * rhs.mMat[1][1] + lhs.mMat[2][1] * rhs.mMat[1][2] + lhs.mMat[3][1] * rhs.mMat[1][3];
		result.mMat[1][2] = lhs.mMat[0][2] * rhs.mMat[1][0] + lhs.mMat[1][2] * rhs.mMat[1][1] + lhs.mMat[2][2] * rhs.mMat[1][2] + lhs.mMat[3][2] * rhs.mMat[1][3];
		result.mMat[1][3] = lhs.mMat[0][3] * rhs.mMat[1][0] + lhs.mMat[1][3] * rhs.mMat[1][1] + lhs.mMat[2][3] * rhs.mMat[1][2] + lhs.mMat[3][3] * rhs.mMat[1][3];

		result.mMat[2][0] = lhs.mMat[0][0] * rhs.mMat[2][0] + lhs.mMat[1][0] * rhs.mMat[2][1] + lhs.mMat[2][0] * rhs.mMat[2][2] + lhs.mMat[3][0] * rhs.mMat[2][3];
		result.mMat[2][1] = lhs.mMat[0][1] * rhs.mMat[2][0] + lhs.mMat[1][1] * rhs.mMat[2][1] + lhs.mMat[2][1] * rhs.mMat[2][2] + lhs.mMat[3][1] * rhs.mMat[2][3];
		result.mMat[2][2] = lhs.mMat[0][2] * rhs.mMat[2][0] + lhs.mMat[1][2] * rhs.mMat[2][1] + lhs.mMat[2][2] * rhs.mMat[2][2] + lhs.mMat[3][2] * rhs.mMat[2][3];
		result.mMat[2][3] = lhs.mMat[0][3] * rhs.mMat[2][0] + lhs.mMat[1][3] * rhs.mMat[2][1] + lhs.mMat[2][3] * rhs.mMat[2][2] + lhs.mMat[3][3] * rhs.mMat[2][3];

		result.mMat[3][0] = lhs.mMat[0][0] * rhs.mMat[3][0] + lhs.mMat[1][0] * rhs.mMat[3][1] + lhs.mMat[2][0] * rhs.mMat[3][2] + lhs.mMat[3][0] * rhs.mMat[3][3];
		result.mMat[3][1] = lhs.mMat[0][1] * rhs.mMat[3][0] + lhs.mMat[1][1] * rhs.mMat[3][1] + lhs.mMat[2][1] * rhs.mMat[3][2] + lhs.mMat[3][1] * rhs.mMat[3][3];
		result.mMat[3][2] = lhs.mMat[0][2] * rhs.mMat[3][0] + lhs.mMat[1][2] * rhs.mMat[3][1] + lhs.mMat[2][2] * rhs.mMat[3][2] + lhs.mMat[3][2] * rhs.mMat[3][3];
		result.mMat[3][3] = lhs.mMat[0][3] * rhs.mMat[3][0] + lhs.mMat[1][3] * rhs.mMat[3][1] + lhs.mMat[2][3] * rhs.mMat[3][2] + lhs.mMat[3][3] * rhs.mMat[3][3];

		return result;
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to be added to each element of the matrix.

	@return Mat44
	Resultant matrix after addition.

	Adds a scalar value to each element of the matrix.
	*/
	Mat44 operator+(Mat44 const& lhs, float const& val) {
		return Mat44(lhs.mMat[0] + val, lhs.mMat[1] + val, lhs.mMat[2] + val, lhs.mMat[3] + val);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to be subtracted from each element of the matrix.

	@return Mat44
	Resultant matrix after subtraction.

	Subtracts a scalar value from each element of the matrix.
	*/
	Mat44 operator-(Mat44 const& lhs, float const& val) {
		return Mat44(lhs.mMat[0] - val, lhs.mMat[1] - val, lhs.mMat[2] - val,lhs.mMat[3] - val);
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to multiply each element of the matrix.

	@return Mat44
	Resultant matrix after multiplication.

	Multiplies each element of the matrix by a scalar value.
	*/
	Mat44 operator*(Mat44 const& lhs, float const& val) {
		return Mat44(lhs.mMat[0] * val, lhs.mMat[1] * val, lhs.mMat[2] * val, lhs.mMat[3] * val);
	}

	/*  _________________________________________________________________________ */
	/*! operator/

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to divide each element of the matrix.

	@return Mat44
	Resultant matrix after division.

	Divides each element of the matrix by a scalar value.
	*/
	Mat44 operator/(Mat44 const& lhs, float const& val) {
		return Mat44(lhs.mMat[0] / val, lhs.mMat[1] / val, lhs.mMat[2] / val, lhs.mMat[3] / val);
	}

	/*  _________________________________________________________________________ */
	/*! operator+=

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat44&
	Reference to the left-hand side matrix after addition.

	Adds the right-hand side matrix to the left-hand side matrix.
	*/
	Mat44& operator+=(Mat44& lhs, Mat44 const& rhs) {
		lhs.mMat[0] += rhs.mMat[0];
		lhs.mMat[1] += rhs.mMat[1];
		lhs.mMat[2] += rhs.mMat[2];
		lhs.mMat[3] += rhs.mMat[3];
		return lhs;
	}

	/*  _________________________________________________________________________ */
	/*! operator-=

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat4&
	Reference to the left-hand side matrix after subtraction.

	Subtracts the right-hand side matrix from the left-hand side matrix.
	*/
	Mat44& operator-=(Mat44& lhs, Mat44 const& rhs) {
		lhs.mMat[0] -= rhs.mMat[0];
		lhs.mMat[1] -= rhs.mMat[1];
		lhs.mMat[2] -= rhs.mMat[2];
		lhs.mMat[3] -= rhs.mMat[3];
		return lhs;
	}
}

