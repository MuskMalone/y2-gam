#include "Math\Mat3.h"
#include "Math\MathUtils.h"

namespace Image {
	Mat33::Mat33() :mMat{} {}

	Mat33::Mat33(float const& val) :mMat{val,val,val} {}

	/**************************************************************************/
	/*!
		Constructor that has 9 parameters to initialise the Matrix
	*/
	/**************************************************************************/
	Mat33::Mat33(float a00, float a01, float a02,
				float a10, float a11, float a12,
				float a20, float a21, float a22) : mMat{ Vec3(a00,a01,a02) ,
														Vec3(a10,a11,a12) ,
														Vec3(a20,a21,a22) } {}

	Mat33::Mat33(Vec3 col1, Vec3 col2, Vec3 col3) :mMat{ col1, col2, col3 } {}

	Vec3 Mat33::operator[](int idx) const{
		assert((idx >= 0) && idx < 3 && "Index out of bounds");
		return mMat[idx];
	}

	Vec3& Mat33::operator[](int idx) {
		assert((idx >= 0) && idx < 3 && "Index out of bounds");
		return mMat[idx];
	}

	bool Mat33::operator==(Mat33 rhs) {
		return (mMat[0] == rhs.mMat[0] && mMat[1] == rhs.mMat[1] && mMat[2] == rhs.mMat[2]);
	}

	Mat33 Mat33::operator+(Mat33 const& rhs) {
		return Mat33(mMat[0] + rhs.mMat[0], mMat[1] + rhs.mMat[1], mMat[2] + rhs.mMat[2]);
	}

	Mat33 Mat33::operator-(Mat33 const& rhs) {
		return Mat33{ mMat[0] - rhs.mMat[0], mMat[1] - rhs.mMat[1], mMat[2] - rhs.mMat[2] };
	}

	Mat33 Mat33::operator*(Mat33 const& rhs) {
		Mat33 result;
		for (int i{}; i < 3; ++i) {
			for (int j{}; j < 3; ++j) {
				result[i][j] = 0;
				for (int k = 0; k < 3; ++k) {
					result[i][j] += mMat[i][k] * rhs[k][j];
				}
			}
		}
		return result;
	}

	Mat33 Mat33::operator+(float const& val) {
		return Mat33(mMat[0] + val, mMat[1] + val, mMat[2] + val);
	}

	Mat33 Mat33::operator-(float const& val) {
		return Mat33(mMat[0] - val, mMat[1] - val, mMat[2] - val);
	}

	Mat33 Mat33::operator*(float const& val) {
		return Mat33(mMat[0] * val, mMat[1] * val, mMat[2] * val);
	}

	Mat33 Mat33::operator/(float const& val) {
		return Mat33(mMat[0] / val, mMat[1] / val, mMat[2] / val);
	}

	Mat33& Mat33::operator+=(float const& val) {
		mMat[0] += val;
		mMat[1] += val;
		mMat[2] += val;
		return *this;
	}

	Mat33& Mat33::operator-=(float const& val) {
		mMat[0] -= val;
		mMat[1] -= val;
		mMat[2] -= val;
		return *this;
	}

	Mat33& Mat33::operator*=(float const& val) {
		mMat[0] *= val;
		mMat[1] *= val;
		mMat[2] *= val;
		return *this;
	}

	Mat33& Mat33::operator/=(float const& val) {
		mMat[0] /= val;
		mMat[1] /= val;
		mMat[2] /= val;
		return *this;
	}

	Mat33& Mat33::operator+=(Mat33 const& rhs) {
		mMat[0] += rhs.mMat[0];
		mMat[1] += rhs.mMat[1];
		mMat[2] += rhs.mMat[2];
		return *this;
	}

	Mat33& Mat33::operator-=(Mat33 const& rhs) {
		mMat[0] -= rhs.mMat[0];
		mMat[1] -= rhs.mMat[1];
		mMat[2] -= rhs.mMat[2];
		return *this;
	}

	void Mat33Identity(Mat33& results) {
		results = { Vec3(1.f,0,0) ,
					Vec3(0,1.f,0) ,
					Vec3(0,0,1.f) };
	}

	void Mat33Translate(Mat33& results, float x, float y) {
		results = { Vec3(1.f,0,x) ,
					Vec3(0,1.f,y) ,
					Vec3(0,0,1.f) };
	}

	void Mat33Scale(Mat33& results, float x, float y) {
		results = { Vec3(x,0,0) ,
					Vec3(0,y,0) ,
					Vec3(0,0,1.f) };
	}

	void Mat33RotRad(Mat33& results, float angle) {
		results = { Vec3(cosf(angle),-sinf(angle),0) , Vec3(sinf(angle),cosf(angle),0) , Vec3(0,0,1.f) };
	}

	void Mat33RotDeg(Mat33& results, float angle) {
		//Find the anagle in rad
		float rad = angle * static_cast<float>(gPI) / 180.f;
		Mat33RotRad(results, rad);
	}

	void Mat33Transpose(Mat33& results, Mat33 const& Mtx) {
		for (int i{}; i < 3; i++) {
			for (int j{}; j < 3; j++) {
				results.mMat[i][j] = Mtx.mMat[j][i];
			}
		}
	}

	void Mat33Inverse(Mat33& results, const Mat33& Mtx) {
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


	std::ostream& operator<<(std::ostream& os, Mat33& m) {
		for (int i{}; i < 3; ++i) {
			os << m[0][i] << " " << m[1][i] << " " << m[2][i] << std::endl;
		}
		return os;
	}

}
