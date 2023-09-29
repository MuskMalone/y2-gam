/******************************************************************************/
/*!
\par        Image Engine
\file       Mat2.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 4, 2023

\brief      Implementation file for Mat2 math

			This file contains the Implementation for a Matrix 2 class.
			It provides various constructors, operators, and utility functions 
			for matrix operations such as addition, subtraction, multiplication, 
			transposition, and inversion.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include <Math/MathUtils.h>
namespace Image {
	/*  _________________________________________________________________________ */
	/*! Mat22

	@param none.

	@return none.

	Default constructor. Set the values to 0
	*/
	Mat22::Mat22() :mMat{} {}

	/*  _________________________________________________________________________ */
	/*! Mat22

	@param val
	Value for the Matrix

	@return none.

	Constructor that sets both values to val
	*/
	Mat22::Mat22(float const& val) :mMat{ val,val } {}

	/*  _________________________________________________________________________ */
	/*! Mat22

	@param rhs
	Matrix to copy from

	@return none.

	Copy constructor.
	*/
	Mat22::Mat22(Mat22 const& rhs) :mMat{ rhs.mMat[0],rhs.mMat[1] } {}
	
	/*  _________________________________________________________________________ */
	/*! Mat22

	@param a00
	Element at position (0,0).
	@param a01
	Element at position (0,1).
	@param a10
	Element at position (1,0).
	@param a11
	Element at position (1,1).
	@return none.

	Constructor to initialize matrix with given values.
	*/
	Mat22::Mat22(float a00, float a01,
		float a10, float a11) : mMat{ Vec2(a00,a01) ,
								Vec2(a10,a11) } {}

	/*  _________________________________________________________________________ */
	/*! Mat22

	@param col1
	First column of the matrix

	@param col2
	Second column of the matrix

	@return none.

	Constructor to initialize matrix with two vectors.
	*/
	Mat22::Mat22(Vec2 col1, Vec2 col2) : mMat{ col1, col2 } {}

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
	bool operator==(Mat22 const& lhs, Mat22 const& rhs) {
		return (lhs.mMat[0] == rhs.mMat[0] && lhs.mMat[1] == rhs.mMat[1]);
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat22
	Resultant matrix after addition.

	Adds two matrices.
	*/
	Mat22 operator+(Mat22 const& lhs, Mat22 const& rhs) {
		return Mat22(lhs.mMat[0] + rhs.mMat[0], lhs.mMat[1] + rhs.mMat[1]);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat22
	Resultant matrix after subtraction.

	Subtracts one matrix from another.
	*/
	Mat22 operator-(Mat22 const& lhs, Mat22 const& rhs) {
		return Mat22(lhs.mMat[0] - rhs.mMat[0], lhs.mMat[1] - rhs.mMat[1]);
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat22
	Resultant matrix after multiplication.

	Multiplies two matrices.
	*/
	Mat22 operator*(Mat22 const& lhs, Mat22 const& rhs) {
		Mat22 result;

		result.mMat[0][0] = lhs.mMat[0][0] * rhs.mMat[0][0] + lhs.mMat[1][0] * rhs.mMat[0][1];
		result.mMat[1][0] = lhs.mMat[0][0] * rhs.mMat[1][0] + lhs.mMat[1][0] * rhs.mMat[1][1];

		result.mMat[0][1] = lhs.mMat[0][1] * rhs.mMat[0][0] + lhs.mMat[1][1] * rhs.mMat[0][1];
		result.mMat[1][1] = lhs.mMat[0][1] * rhs.mMat[1][0] + lhs.mMat[1][1] * rhs.mMat[1][1];

		return result;
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to be added to each element of the matrix.

	@return Mat22
	Resultant matrix after addition.

	Adds a scalar value to each element of the matrix.
	*/
	Mat22 operator+(Mat22 const& lhs, float const& val) {
		return Mat22(lhs.mMat[0] + val, lhs.mMat[1] + val);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to be subtracted from each element of the matrix.

	@return Mat22
	Resultant matrix after subtraction.

	Subtracts a scalar value from each element of the matrix.
	*/
	Mat22 operator-(Mat22 const& lhs, float const& val) {
		return Mat22(lhs.mMat[0] - val, lhs.mMat[1] - val);
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to multiply each element of the matrix.

	@return Mat22
	Resultant matrix after multiplication.

	Multiplies each element of the matrix by a scalar value.
	*/
	Mat22 operator*(Mat22 const& lhs, float const& val) {
		return Mat22(lhs.mMat[0] * val, lhs.mMat[1] * val);
	}

	/*  _________________________________________________________________________ */
	/*! operator/

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to divide each element of the matrix.

	@return Mat22
	Resultant matrix after division.

	Divides each element of the matrix by a scalar value.
	*/
	Mat22 operator/(Mat22 const& lhs, float const& val) {
		return Mat22(lhs.mMat[0] / val, lhs.mMat[1] / val);
	}

	/*  _________________________________________________________________________ */
	/*! operator+=

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat22&
	Reference to the left-hand side matrix after addition.

	Adds the right-hand side matrix to the left-hand side matrix.
	*/
	Mat22& operator+=(Mat22& lhs, Mat22 const& rhs) {
		lhs.mMat[0] += rhs.mMat[0];
		lhs.mMat[1] += rhs.mMat[1];
		return lhs;
	}

	/*  _________________________________________________________________________ */
	/*! operator-=

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat22&
	Reference to the left-hand side matrix after subtraction.

	Subtracts the right-hand side matrix from the left-hand side matrix.
	*/
	Mat22& operator-=(Mat22& lhs, Mat22 const& rhs) {
		lhs.mMat[0] -= rhs.mMat[0];
		lhs.mMat[1] -= rhs.mMat[1];
		return lhs;
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side vector.

	@return Vec2
	Resultant vector after multiplication.

	Multiplies the matrix by a vector.
	*/
	Vec2 operator*(Mat22 const& lhs, Vec2 const& rhs) {
		return Vec2(lhs.mMat[0][0] * rhs.mData[0] + lhs.mMat[1][0] * rhs.mData[1], lhs.mMat[0][1] * rhs.mData[0] + lhs.mMat[1][1] * rhs.mData[1]);
	}

	/*  _________________________________________________________________________ */
	/*! Mat22Identity

	@param results
	Matrix to be set to identity.

	@return none.

	Sets the matrix to identity.
	*/
	void Mat22Identity(Mat22& results) {
		results = { Vec2(1.f,0) ,
					Vec2(0,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat22Scale

	@param results
	Matrix to be scaled.

	@param x
	Scaling factor for the x-axis.

	@param y
	Scaling factor for the y-axis.

	@return none.

	Scales the matrix by given factors.
	*/
	void Mat22Scale(Mat22& results, float x, float y) {
		results = { Vec2(x,0) ,
					Vec2(0,y) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat22RotRad

	@param results
	Matrix to be rotated.

	@param angle
	Rotation angle in radians.

	@return none.

	Rotates the matrix by a given angle in radians.
	*/
	void Mat22RotRad(Mat22& results, float angle) {
		results = { Vec2(cosf(angle),-sinf(angle)) ,
				Vec2(sinf(angle),cosf(angle)) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat22RotDeg

	@param results
	Matrix to be rotated.

	@param angle
	Rotation angle in degrees.

	@return none.

	Rotates the matrix by a given angle in degrees.
	*/
	void Mat22RotDeg(Mat22& results, float angle) {
		//Find the anagle in rad
		float rad = angle * static_cast<float>(gPI) / 180.f;
		Mat22RotRad(results, rad);
	}

	/*  _________________________________________________________________________ */
	/*! Mat22Transpose

	@param results
	Matrix to store the transposed result.

	@param Mtx
	Matrix to be transposed.
	
	@return none.

	Transposes the given matrix.
	*/
	void Mat22Transpose(Mat22& results, Mat22 const& Mtx) {
		for (int i{}; i < 2; i++) {
			for (int j{}; j < 2; j++) {
				results.mMat[i][j] = Mtx.mMat[j][i];
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! Mat22Transpose

	@param Mtx
	Matrix to be transposed.

	@return Mat22.
	Transposed matrix.

	Transposes the given matrix and returns the result.
	*/
	Mat22& Mat22Transpose(Mat22 const& Mtx) {
		Mat22 results{};
		for (int i{}; i < 2; i++) {
			for (int j{}; j < 2; j++) {
				results.mMat[i][j] = Mtx.mMat[j][i];
			}
		}
		return results;
	}


	/*  _________________________________________________________________________ */
	/*! Mat22Inverse

	@param results
	Matrix to store the inverted result.

	@param Mtx
	Matrix to be inverted.

	@return none.

	Inverts the given matrix.
	*/
	void Mat22Inverse(Mat22& results, Mat22 const& Mtx) {
		float det = Mtx.mMat[0][0] * Mtx.mMat[1][1] - Mtx.mMat[1][0] * Mtx.mMat[0][1];
		assert((det != 0) && "Matrix cannot be inverted");
		if (det == 0) {
			// Handle singular matrix. For now, set it to identity.
			Mat22Identity(results);
			return;
		}

		results.mMat[0][0] = Mtx.mMat[1][1] / det;
		results.mMat[1][0] = -Mtx.mMat[1][0] / det;
		results.mMat[0][1] = -Mtx.mMat[0][1] / det;
		results.mMat[1][1] = Mtx.mMat[0][0] / det;
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
	std::ostream& operator<<(std::ostream& os, Mat22& m) {
		for (int i{}; i < 2; ++i) {
			os << m.mMat[0][i] << " " << m.mMat[1][i] << std::endl;
		}
		return os;
	}


}
