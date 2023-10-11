/******************************************************************************/
/*!
\par        Image Engine
\file       Mat3.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 4, 2023

\brief      Implementation file for Mat3 math

			This file contains the Implementation for a Matrix 3 class.
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
	/*! Mat33

	@param none.

	@return none.

	Default constructor. Set the values to 0
	*/
	Mat33::Mat33() :mMat{} {}

	/*  _________________________________________________________________________ */
	/*! Mat33

	@param val
	Value for the Matrix

	@return none.

	Constructor that sets both values to val
	*/
	Mat33::Mat33(float const& val) :mMat{ val,val,val } {}

	/*  _________________________________________________________________________ */
	/*! Mat33

	@param rhs
	Matrix to copy from

	@return none.

	Copy constructor.
	*/
	Mat33::Mat33(Mat33 const& rhs) :mMat{ rhs.mMat[0],rhs.mMat[1],rhs.mMat[2]} {}

	/*  _________________________________________________________________________ */
	/*! Mat33

	@param  a00,  a01,  a02, a10,  a11,  a12, a20,  a21,  a22
	Matrix Elements

	@return none.

	Constructor to initialize matrix with given values.
	*/
	Mat33::Mat33(float a00, float a01, float a02,
		float a10, float a11, float a12,
		float a20, float a21, float a22) : mMat{ Vec3(a00,a01,a02) ,
												Vec3(a10,a11,a12) ,
												Vec3(a20,a21,a22) } {}

	/*  _________________________________________________________________________ */
	/*! Mat33

	@param col1
	First column of the matrix

	@param col2
	Second column of the matrix

	@param col3
	Third column of the matrix

	@return none.

	Constructor to initialize matrix with 3 vectors.
	*/
	Mat33::Mat33(Vec3 col1, Vec3 col2, Vec3 col3) :mMat{ col1, col2, col3 } {}

	/*  _________________________________________________________________________ */
	/*! Mat33Identity

	@param results
	Matrix to be set to identity.

	@return none.

	Sets the matrix to identity.
	*/
	void Mat33Identity(Mat33& results) {
		results = { Vec3(1.f,0,0) ,
					Vec3(0,1.f,0) ,
					Vec3(0,0,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat33Translate

	@param results
	Matrix to be set to identity.

	@param x
	Translate factor for the x-axis.

	@param y
	Translate factor for the y-axis.

	@return none.

	Translate the matrix by its x and y.
	*/
	void Mat33Translate(Mat33& results, float x, float y) {
		results = { Vec3(1.f,0,0) ,
					Vec3(0,1.f,0) ,
					Vec3(x,y,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat33Scale

	@param results
	Matrix to be scaled.

	@param x
	Scaling factor for the x-axis.

	@param y
	Scaling factor for the y-axis.

	@return none.

	Scales the matrix by given factors.
	*/
	void Mat33Scale(Mat33& results, float x, float y) {
		results = { Vec3(x,0,0) ,
					Vec3(0,y,0) ,
					Vec3(0,0,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat33RotRad

	@param results
	Matrix to be rotated.

	@param angle
	Rotation angle in radians.

	@return none.

	Rotates the matrix by a given angle in radians.
	*/
	void Mat33RotRad(Mat33& results, float angle) {
		results = { Vec3(cosf(angle),sinf(angle),0) , 
					Vec3(-sinf(angle),cosf(angle),0) , Vec3(0,0,1.f) };
	}

	/*  _________________________________________________________________________ */
	/*! Mat33RotDeg

	@param results
	Matrix to be rotated.

	@param angle
	Rotation angle in degrees.

	@return none.

	Rotates the matrix by a given angle in degrees.
	*/
	void Mat33RotDeg(Mat33& results, float angle) {
		//Find the anagle in rad
		float rad = angle * static_cast<float>(gPI) / 180.f;
		Mat33RotRad(results, rad);
	}

	/*  _________________________________________________________________________ */
	/*! Mat33Transpose

	@param results
	Matrix to store the transposed result.

	@param Mtx
	Matrix to be transposed.

	@return none.

	Transposes the given matrix.
	*/
	void Mat33Transpose(Mat33& results, Mat33 const& Mtx) {
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				results.mMat[i][j] = Mtx.mMat[j][i];
			}
		}
	}

	/*  _________________________________________________________________________ */
	/*! Mat33Inverse

	@param results
	Matrix to store the inverted result.

	@param Mtx
	Matrix to be inverted.

	@return none.

	Inverts the given matrix.
	*/
	void Mat33Inverse(Mat33& results, Mat33 const& Mtx) {
		//To find the determinant
		float det = Mtx.mMat[0][0] * Mtx.mMat[2][2] * Mtx.mMat[1][1] + Mtx.mMat[0][1] * Mtx.mMat[1][2] * Mtx.mMat[2][0]
			+ Mtx.mMat[0][2] * Mtx.mMat[1][0] * Mtx.mMat[2][1]
			- (Mtx.mMat[2][0] * Mtx.mMat[1][1] * Mtx.mMat[0][2]
				+ Mtx.mMat[2][1] * Mtx.mMat[1][2] * Mtx.mMat[0][0]
				+ Mtx.mMat[2][2] * Mtx.mMat[1][0] * Mtx.mMat[0][1]);
		//if determinant is less than or equal 0 set results to null
		assert((det != 0) && "Matrix cannot be inverted");
		if (det == 0) {
			// Handle singular matrix. For now, set it to identity.
			Mat33Identity(results);
			return;
		}
		else {
			//To find the Adj Matrix
			Mat33 temp{};
			temp.mMat[0][0] = (Mtx.mMat[1][1] * Mtx.mMat[2][2] - Mtx.mMat[1][2] * Mtx.mMat[2][1]) * (1 / det);
			temp.mMat[0][1] = (Mtx.mMat[0][2] * Mtx.mMat[2][1] - Mtx.mMat[0][1] * Mtx.mMat[2][2]) * (1 / det);
			temp.mMat[0][2] = (Mtx.mMat[0][1] * Mtx.mMat[1][2] - Mtx.mMat[0][2] * Mtx.mMat[1][1]) * (1 / det);
			temp.mMat[1][0] = (Mtx.mMat[1][2] * Mtx.mMat[2][0] - Mtx.mMat[1][0] * Mtx.mMat[2][2]) * (1 / det);
			temp.mMat[1][1] = (Mtx.mMat[0][0] * Mtx.mMat[2][2] - Mtx.mMat[0][2] * Mtx.mMat[2][0]) * (1 / det);
			temp.mMat[1][2] = (Mtx.mMat[0][2] * Mtx.mMat[1][0] - Mtx.mMat[0][0] * Mtx.mMat[1][2]) * (1 / det);
			temp.mMat[2][0] = (Mtx.mMat[1][0] * Mtx.mMat[2][1] - Mtx.mMat[1][1] * Mtx.mMat[2][0]) * (1 / det);
			temp.mMat[2][1] = (Mtx.mMat[0][1] * Mtx.mMat[2][0] - Mtx.mMat[0][0] * Mtx.mMat[2][1]) * (1 / det);
			temp.mMat[2][2] = (Mtx.mMat[0][0] * Mtx.mMat[1][1] - Mtx.mMat[0][1] * Mtx.mMat[1][0]) * (1 / det);
			results = temp;
		}
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
	std::ostream& operator<<(std::ostream& os, Mat33& m) {
		for (int i{}; i < 3; ++i) {
			os << m.mMat[0][i] << " " << m.mMat[1][i] << " " << m.mMat[2][i] << std::endl;
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
	bool operator==(Mat33 const& lhs, Mat33 const& rhs){
		return (lhs.mMat[0] == rhs.mMat[0] && lhs.mMat[1] == rhs.mMat[1] && lhs.mMat[2] == rhs.mMat[2]);
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat33
	Resultant matrix after addition.

	Adds two matrices.
	*/
	Mat33 operator+(Mat33 const& lhs, Mat33 const& rhs){
		return Mat33(lhs.mMat[0] + rhs.mMat[0], lhs.mMat[1] + rhs.mMat[1], lhs.mMat[2] + rhs.mMat[2]);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat33
	Resultant matrix after subtraction.

	Subtracts one matrix from another.
	*/
	Mat33 operator-(Mat33 const& lhs, Mat33 const& rhs){
		return Mat33(lhs.mMat[0] - rhs.mMat[0], lhs.mMat[1] - rhs.mMat[1], lhs.mMat[2] - rhs.mMat[2]);

	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat33
	Resultant matrix after multiplication.

	Multiplies two matrices.
	*/
	Mat33 operator*(Mat33 const& lhs, Mat33 const& rhs) {
		Mat33 result;

		result.mMat[0][0] = lhs.mMat[0][0] * rhs.mMat[0][0] + lhs.mMat[1][0] * rhs.mMat[0][1] + lhs.mMat[2][0] * rhs.mMat[0][2];
		result.mMat[1][0] = lhs.mMat[0][0] * rhs.mMat[1][0] + lhs.mMat[1][0] * rhs.mMat[1][1] + lhs.mMat[2][0] * rhs.mMat[1][2];
		result.mMat[2][0] = lhs.mMat[0][0] * rhs.mMat[2][0] + lhs.mMat[1][0] * rhs.mMat[2][1] + lhs.mMat[2][0] * rhs.mMat[2][2];

		result.mMat[0][1] = lhs.mMat[0][1] * rhs.mMat[0][0] + lhs.mMat[1][1] * rhs.mMat[0][1] + lhs.mMat[2][1] * rhs.mMat[0][2];
		result.mMat[1][1] = lhs.mMat[0][1] * rhs.mMat[1][0] + lhs.mMat[1][1] * rhs.mMat[1][1] + lhs.mMat[2][1] * rhs.mMat[1][2];
		result.mMat[2][1] = lhs.mMat[0][1] * rhs.mMat[2][0] + lhs.mMat[1][1] * rhs.mMat[2][1] + lhs.mMat[2][1] * rhs.mMat[2][2];

		result.mMat[0][2] = lhs.mMat[0][2] * rhs.mMat[0][0] + lhs.mMat[1][2] * rhs.mMat[0][1] + lhs.mMat[2][2] * rhs.mMat[0][2];
		result.mMat[1][2] = lhs.mMat[0][2] * rhs.mMat[1][0] + lhs.mMat[1][2] * rhs.mMat[1][1] + lhs.mMat[2][2] * rhs.mMat[1][2];
		result.mMat[2][2] = lhs.mMat[0][2] * rhs.mMat[2][0] + lhs.mMat[1][2] * rhs.mMat[2][1] + lhs.mMat[2][2] * rhs.mMat[2][2];

		return result;
	}

	/*  _________________________________________________________________________ */
	/*! operator+

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to be added to each element of the matrix.

	@return Mat33
	Resultant matrix after addition.

	Adds a scalar value to each element of the matrix.
	*/
	Mat33 operator+(Mat33 const& lhs, float const& val)	{
		return Mat33(lhs.mMat[0] + val, lhs.mMat[1] + val,lhs.mMat[2]+val);
	}

	/*  _________________________________________________________________________ */
	/*! operator-

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to be subtracted from each element of the matrix.

	@return Mat33
	Resultant matrix after subtraction.

	Subtracts a scalar value from each element of the matrix.
	*/
	Mat33 operator-(Mat33 const& lhs, float const& val){
		return Mat33(lhs.mMat[0] - val, lhs.mMat[1] - val, lhs.mMat[2] - val);
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to multiply each element of the matrix.

	@return Mat33
	Resultant matrix after multiplication.

	Multiplies each element of the matrix by a scalar value.
	*/
	Mat33 operator*(Mat33 const& lhs, float const& val){
		return Mat33(lhs.mMat[0] * val, lhs.mMat[1] * val, lhs.mMat[2] * val);
	}

	/*  _________________________________________________________________________ */
	/*! operator/

	@param lhs
	Left-hand side matrix.

	@param val
	Scalar value to divide each element of the matrix.

	@return Mat33
	Resultant matrix after division.

	Divides each element of the matrix by a scalar value.
	*/
	Mat33 operator/(Mat33 const& lhs, float const& val){
		return Mat33(lhs.mMat[0] / val, lhs.mMat[1] / val, lhs.mMat[2] / val);
	}

	/*  _________________________________________________________________________ */
	/*! operator+=

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat33&
	Reference to the left-hand side matrix after addition.

	Adds the right-hand side matrix to the left-hand side matrix.
	*/
	Mat33& operator+=(Mat33& lhs, Mat33 const& rhs){
		lhs.mMat[0] += rhs.mMat[0];
		lhs.mMat[1] += rhs.mMat[1];
		lhs.mMat[2] += rhs.mMat[2];
		return lhs;
	}

	/*  _________________________________________________________________________ */
	/*! operator-=

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side matrix.

	@return Mat33&
	Reference to the left-hand side matrix after subtraction.

	Subtracts the right-hand side matrix from the left-hand side matrix.
	*/
	Mat33& operator-=(Mat33& lhs, Mat33 const& rhs){
		lhs.mMat[0] -= rhs.mMat[0];
		lhs.mMat[1] -= rhs.mMat[1];
		lhs.mMat[2] -= rhs.mMat[2];
		return lhs;
	}

	/*  _________________________________________________________________________ */
	/*! operator*

	@param lhs
	Left-hand side matrix.

	@param rhs
	Right-hand side vector.

	@return Vec3
	Resultant vector after multiplication.

	Multiplies the matrix by a vector.
	*/
	Vec3 operator*(Mat33 const& lhs, Vec3 const& rhs){
		return Vec3(lhs.mMat[0][0] * rhs.mData[0] + lhs.mMat[1][0] * rhs.mData[1] + lhs.mMat[2][0] * rhs.mData[2],
			lhs.mMat[0][1] * rhs.mData[0] + lhs.mMat[1][1] * rhs.mData[1] + lhs.mMat[2][1] * rhs.mData[2],
			lhs.mMat[0][2] * rhs.mData[0] + lhs.mMat[1][2] * rhs.mData[1] + lhs.mMat[2][2] * rhs.mData[2]);
	}
}
