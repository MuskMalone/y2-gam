#include "Math/Mat2.h"
#include "Math/MathUtils.h"
namespace Image {
	Mat22::Mat22() :mMat{} {}

	Mat22::Mat22(float const& val) :mMat{ val,val} {}

	/**************************************************************************/
	/*!
		Constructor that has 9 parameters to initialise the Matrix
	*/
	/**************************************************************************/
	Mat22::Mat22(float a00, float a01,
		float a10, float a11) : mMat{ Vec2(a00,a01) ,
								Vec2(a10,a11) } {}

	Mat22::Mat22(Vec2 col1, Vec2 col2) : mMat{ col1, col2 } {}

	Vec2 Mat22::operator[](int idx) const {
		assert(idx >= 0 && idx < 2 && "Index out of bounds");
		return mMat[idx];
	}

	Vec2& Mat22::operator[](int idx) {
		assert(idx >= 0 && idx < 2 && "Index out of bounds");
		return mMat[idx];
	}

	bool Mat22::operator==(Mat22 rhs) {
		return (mMat[0] == rhs.mMat[0] && mMat[1] == rhs.mMat[1]);
	}

	Mat22 Mat22::operator+(Mat22 const& rhs)const {
		return Mat22(mMat[0] + rhs.mMat[0], mMat[1] + rhs.mMat[1]);
	}

	Mat22 Mat22::operator-(Mat22 const& rhs)const {
		return Mat22{ mMat[0] - rhs.mMat[0], mMat[1] - rhs.mMat[1]};
	}

	Mat22 Mat22::operator*(Mat22 const& rhs) const {
		Mat22 result;
		for (int i{}; i < 2; ++i) {
			for (int j{}; j < 2; ++j) {
				result[i][j] = 0;
				for (int k = 0; k < 2; ++k) {
					result[i][j] += mMat[i][k] * rhs[k][j];
				}
			}
		}
		return result;
	}

	Mat22 Mat22::operator+(float const& val)const {
		return Mat22(mMat[0]+val, mMat[1] + val);
	}

	Mat22 Mat22::operator-(float const& val) const {
		return Mat22(mMat[0] - val, mMat[1] - val);
	}

	Mat22 Mat22::operator*(float const& val)const {
		return Mat22(mMat[0] * val, mMat[1] * val);
	}

	Mat22 Mat22::operator/(float const& val)const {
		return Mat22(mMat[0] / val, mMat[1] / val);
	}

	Mat22& Mat22::operator+=(float const& val) {
		mMat[0] += val;
		mMat[1] += val;
		return *this;
	}

	Mat22& Mat22::operator-=(float const& val) {
		mMat[0] -= val;
		mMat[1] -= val;
		return *this;
	}

	Mat22& Mat22::operator*=(float const& val) {
		mMat[0] *= val;
		mMat[1] *= val;
		return *this;
	}

	Mat22& Mat22::operator/=(float const& val) {
		mMat[0] /= val;
		mMat[1] /= val;
		return *this;
	}

	Mat22& Mat22::operator+=(Mat22 const& rhs) {
		mMat[0] += rhs.mMat[0];
		mMat[1] += rhs.mMat[1];
		return *this;
	}

	Mat22& Mat22::operator-=(Mat22 const& rhs) {
		mMat[0] -= rhs.mMat[0];
		mMat[1] -= rhs.mMat[1];
		return *this;
	}

	Vec2 Mat22::operator*(Vec2 const& rhs)const {
		return Vec2(mMat[0][0]*rhs.mData[0]+ mMat[1][0] * rhs.mData[1], mMat[0][1] * rhs.mData[0] + mMat[1][1] * rhs.mData[1]);
	}

	void Mat22Identity(Mat22& results) {
		results = { Vec2(1.f,0) ,
					Vec2(0,1.f) };
	}


	void Mat22Scale(Mat22& results, float x, float y) {
		results = { Vec2(x,0) ,
					Vec2(0,y) };
	}

	void Mat22RotRad(Mat22& results, float angle) {
		results = { Vec2(cosf(angle),-sinf(angle)) ,
				Vec2(sinf(angle),cosf(angle))};
	}

	void Mat22RotDeg(Mat22& results, float angle) {
		//Find the anagle in rad
		float rad = angle * static_cast<float>(gPI) / 180.f;
		Mat22RotRad(results, rad);
	}

	void Mat22Transpose(Mat22& results, Mat22 const& Mtx) {
		for (int i{}; i < 2; i++) {
			for (int j{}; j < 2; j++) {
				results.mMat[i][j] = Mtx.mMat[j][i];
			}
		}
	}

	Mat22 Mat22Transpose(Mat22 const& Mtx){
		Mat22 temp{};
		for (int i{}; i < 2; i++) {
			for (int j{}; j < 2; j++) {
				temp.mMat[i][j] = Mtx.mMat[j][i];
			}
		}
		return temp;
	}

	void Mat22Inverse(Mat22& results,  Mat22 const& Mtx) {
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


	std::ostream& operator<<(std::ostream& os, Mat22& m) {
		for (int i{}; i < 2; ++i) {
			os << m[0][i] << " " << m[1][i] << std::endl;
		}
		return os;
	}


}
