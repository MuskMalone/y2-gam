#include "Math\Mat4.h"
#include "Math\MathUtils.h"
const float gPI{ 3.14159f };
namespace Image {
	Mat44::Mat44() :mMat{} {}

	Mat44::Mat44(float const& val) :mMat{val,val,val,val} {}

	/**************************************************************************/
	/*!
		Constructor that has 9 parameters to initialise the mMatrix
	*/
	/**************************************************************************/
	Mat44::Mat44(float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23, 
		float a30, float a31, float a32, float a33 ) : mMat{ Vec4(a00,a01,a02,a03) ,
															Vec4(a10,a11,a12,a13) ,
															Vec4(a20,a21,a22,a23) ,
															Vec4(a30,a31,a32,a33) }{}

	Mat44::Mat44(Vec4 col1, Vec4 col2, Vec4 col3, Vec4 col4) : mMat{ col1, col2, col3,col4 } {}

	Vec4 Mat44::operator[](int idx) const{
		return mMat[idx];
	}

	Vec4& Mat44::operator[](int idx) {
		return mMat[idx];
	}

	bool Mat44::operator==(Mat44 rhs) {
		return (mMat[0] == rhs.mMat[0] && mMat[1] == rhs.mMat[1] && mMat[2] == rhs.mMat[2] && mMat[3] == rhs.mMat[3]);
	}

	Mat44 Mat44::operator+(Mat44 const& rhs) {
		return Mat44(mMat[0] + rhs.mMat[0], mMat[1] + rhs.mMat[1], mMat[2] + rhs.mMat[2], mMat[3] + rhs.mMat[3]);
	}

	Mat44 Mat44::operator-(Mat44 const& rhs) {
		return Mat44(mMat[0] - rhs.mMat[0], mMat[1] - rhs.mMat[1], mMat[2] - rhs.mMat[2], mMat[3] - rhs.mMat[3]);
	}

	Mat44 Mat44::operator*(Mat44 const& rhs) {
		Mat44 result;
		for (int i{}; i < 4; ++i) {
			for (int j{}; j < 4; ++j) {
				result[i][j] = 0;
				for (int k = 0; k < 4; ++k) {
					result[i][j] += mMat[i][k] * rhs[k][j];
				}
			}
		}
		return result;
	}

	Mat44 Mat44::operator+(float const& val) {
		return Mat44(mMat[0] + val, mMat[1] + val, mMat[2] + val, mMat[3] + val);
	}

	Mat44 Mat44::operator-(float const& val) {
		return Mat44(mMat[0] - val, mMat[1] - val, mMat[2] - val, mMat[3] - val);
	}

	Mat44 Mat44::operator*(float const& val) {
		return Mat44(mMat[0] * val, mMat[1] * val, mMat[2] * val, mMat[3] * val);
	}

	Mat44 Mat44::operator/(float const& val) {
		return Mat44(mMat[0] / val, mMat[1] / val, mMat[2] / val, mMat[3] / val);
	}

	Mat44& Mat44::operator+=(float const& val) {
		mMat[0] += val;
		mMat[1] += val;
		mMat[2] += val;
		mMat[3] += val;
		return *this;
	}

	Mat44& Mat44::operator-=(float const& val) {
		mMat[0] -= val;
		mMat[1] -= val;
		mMat[2] -= val;
		mMat[3] -= val;
		return *this;
	}

	Mat44& Mat44::operator*=(float const& val) {
		mMat[0] *= val;
		mMat[1] *= val;
		mMat[2] *= val;
		mMat[3] *= val;
		return *this;
	}

	Mat44& Mat44::operator/=(float const& val) {
		mMat[0] /= val;
		mMat[1] /= val;
		mMat[2] /= val;
		mMat[3] /= val;
		return *this;
	}

	Mat44& Mat44::operator+=(Mat44 const& rhs) {
		mMat[0] += rhs.mMat[0];
		mMat[1] += rhs.mMat[1];
		mMat[2] += rhs.mMat[2];
		mMat[3] += rhs.mMat[3];
		return *this;
	}

	Mat44& Mat44::operator-=(Mat44 const& rhs) {
		mMat[0] -= rhs.mMat[0];
		mMat[1] -= rhs.mMat[1];
		mMat[2] -= rhs.mMat[2];
		mMat[3] -= rhs.mMat[3];
		return *this;
	}

	void Mat44Identity(Mat44& results) {
		results = { Vec4(1.f,0.f,0.f,0.f) ,
					Vec4(0.f,1.f,0.f,0.f) ,
					Vec4(0.f,0.f,1.f,0.f) ,
					Vec4(0.f,0.f,0.f,1.f) };
	}

	void Mat44Translate(Mat44& results, float x, float y, float z) {
		results = { Vec4(1.f,0.f,0.f,x) ,
					Vec4(0.f,1.f,0.f,y) ,
					Vec4(0.f,0.f,1.f,z) ,
					Vec4(0.f,0.f,0.f,1.f) };
	}

	void Mat44Scale(Mat44& results, float x, float y, float z) {
		results = { Vec4(x,0.f,0.f,0.f) ,
					Vec4(0.f,y,0.f,0.f) ,
					Vec4(0.f,0.f,z,0.f) ,
					Vec4(0.f,0.f,0.f,1.f) };
	}

	void Mat44RotRad(Mat44& results, float angle) {
		results = { Vec4(cosf(angle),-sinf(angle),0.f,0.f) , Vec4(sinf(angle),cosf(angle),0.f,0.f) , Vec4(0.f,0.f,1.f,0.f), Vec4(0.f,0.f,0.f,1.f) };
	}

	void Mat44RotDeg(Mat44& results, float angle) {
		//Find the anagle in rad
		float rad = angle * static_cast<float>(gPI) / 180.f;
		Mat44RotRad(results, rad);
	}

	void Mat44Transpose(Mat44& results, Mat44 const& pMtx) {
		for (int i{}; i < 4; i++) {
			for (int j{}; j < 4; j++) {
				results.mMat[i][j] = pMtx.mMat[j][i];
			}
		}
	}

	void Mat44Inverse(Mat44& results, const Mat44& Mtx) {
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
		temp[0][0] = +(toTrans.mMat[1][1] * data00 - toTrans.mMat[1][2] * data01 + toTrans.mMat[1][3] * data02);
		temp[0][1] = -(toTrans.mMat[1][0] * data00 - toTrans.mMat[1][2] * data03 + toTrans.mMat[1][3] * data04);
		temp[0][2] = +(toTrans.mMat[1][0] * data01 - toTrans.mMat[1][1] * data03 + toTrans.mMat[1][3] * data05);
		temp[0][3] = -(toTrans.mMat[1][0] * data02 - toTrans.mMat[1][1] * data04 + toTrans.mMat[1][2] * data05);

		temp[1][0] = -(toTrans.mMat[0][1] * data00 - toTrans.mMat[0][2] * data01 + toTrans.mMat[0][3] * data02);
		temp[1][1] = +(toTrans.mMat[0][0] * data00 - toTrans.mMat[0][2] * data03 + toTrans.mMat[0][3] * data04);
		temp[1][2] = -(toTrans.mMat[0][0] * data01 - toTrans.mMat[0][1] * data03 + toTrans.mMat[0][3] * data05);
		temp[1][3] = +(toTrans.mMat[0][0] * data02 - toTrans.mMat[0][1] * data04 + toTrans.mMat[0][2] * data05);

		temp[2][0] = +(toTrans.mMat[0][1] * data06 - toTrans.mMat[0][2] * data07 + toTrans.mMat[0][3] * data08);
		temp[2][1] = -(toTrans.mMat[0][0] * data06 - toTrans.mMat[0][2] * data09 + toTrans.mMat[0][3] * data10);
		temp[2][2] = +(toTrans.mMat[0][0] * data07 - toTrans.mMat[0][1] * data09 + toTrans.mMat[0][3] * data11);
		temp[2][3] = -(toTrans.mMat[0][0] * data08 - toTrans.mMat[0][1] * data10 + toTrans.mMat[0][2] * data11);

		temp[3][0] = -(toTrans.mMat[0][1] * data12 - toTrans.mMat[0][2] * data13 + toTrans.mMat[0][3] * data14);
		temp[3][1] = +(toTrans.mMat[0][0] * data12 - toTrans.mMat[0][2] * data15 + toTrans.mMat[0][3] * data16);
		temp[3][2] = -(toTrans.mMat[0][0] * data13 - toTrans.mMat[0][1] * data15 + toTrans.mMat[0][3] * data17);
		temp[3][3] = +(toTrans.mMat[0][0] * data14 - toTrans.mMat[0][1] * data16 + toTrans.mMat[0][2] * data17);
		float Determinant =
			+ toTrans[0][0] * temp[0][0]
			+ toTrans[0][1] * temp[0][1]
			+ toTrans[0][2] * temp[0][2]
			+ toTrans[0][3] * temp[0][3];

		temp /= Determinant;
		results = temp;
	}


	std::ostream& operator<<(std::ostream& os, Mat44& m) {
		for (int i{}; i < 4; ++i) {
			os << m[0][i] << " " << m[1][i] << " " << m[2][i] << " " << m[3][i] << std::endl;
		}
		return os;
	}

}

