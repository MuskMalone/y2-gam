#include <Math/MathUtils.h>
namespace Image {
	Mat22::Mat22() :mMat{} {}

	Mat22::Mat22(float const& val) :mMat{ val,val } {}

	Mat22::Mat22(Mat22 const& rhs) :mMat{ rhs.mMat[0],rhs.mMat[1] } {}

	Mat22::Mat22(float a00, float a01,
		float a10, float a11) : mMat{ Vec2(a00,a01) ,
								Vec2(a10,a11) } {}

	Mat22::Mat22(Vec2 col1, Vec2 col2) : mMat{ col1, col2 } {}

	// Overloaded operators


	bool operator==(const Mat22& lhs, const Mat22& rhs) {
		return (lhs.mMat[0] == rhs.mMat[0] && lhs.mMat[1] == rhs.mMat[1]);
	}

	Mat22 operator+(const Mat22& lhs, const Mat22& rhs) {
		return Mat22(lhs.mMat[0] + rhs.mMat[0], lhs.mMat[1] + rhs.mMat[1]);
	}

	Mat22 operator-(const Mat22& lhs, const Mat22& rhs) {
		return Mat22(lhs.mMat[0] - rhs.mMat[0], lhs.mMat[1] - rhs.mMat[1]);
	}

	Mat22 operator*(const Mat22& lhs, const Mat22& rhs) {
		Mat22 result;

		result.mMat[0][0] = lhs.mMat[0][0] * rhs.mMat[0][0] + lhs.mMat[1][0] * rhs.mMat[0][1];
		result.mMat[1][0] = lhs.mMat[0][0] * rhs.mMat[1][0] + lhs.mMat[1][0] * rhs.mMat[1][1];

		result.mMat[0][1] = lhs.mMat[0][1] * rhs.mMat[0][0] + lhs.mMat[1][1] * rhs.mMat[0][1];
		result.mMat[1][1] = lhs.mMat[0][1] * rhs.mMat[1][0] + lhs.mMat[1][1] * rhs.mMat[1][1];

		return result;
	}

	Mat22 operator+(const Mat22& lhs, float const& val) {
		return Mat22(lhs.mMat[0] + val, lhs.mMat[1] + val);
	}

	Mat22 operator-(const Mat22& lhs, float const& val) {
		return Mat22(lhs.mMat[0] - val, lhs.mMat[1] - val);
	}

	Mat22 operator*(const Mat22& lhs, float const& val) {
		return Mat22(lhs.mMat[0] * val, lhs.mMat[1] * val);
	}

	Mat22 operator/(const Mat22& lhs, float const& val) {
		return Mat22(lhs.mMat[0] / val, lhs.mMat[1] / val);
	}

	Mat22& operator+=(Mat22& lhs, const Mat22& rhs) {
		lhs.mMat[0] += rhs.mMat[0];
		lhs.mMat[1] += rhs.mMat[1];
		return lhs;
	}

	Mat22& operator-=(Mat22& lhs, const Mat22& rhs) {
		lhs.mMat[0] -= rhs.mMat[0];
		lhs.mMat[1] -= rhs.mMat[1];
		return lhs;
	}

	Vec2 operator*(const Mat22& lhs, const Vec2& rhs) {
		return Vec2(lhs.mMat[0][0] * rhs.mData[0] + lhs.mMat[1][0] * rhs.mData[1], lhs.mMat[0][1] * rhs.mData[0] + lhs.mMat[1][1] * rhs.mData[1]);
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
				Vec2(sinf(angle),cosf(angle)) };
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
	Mat22& Mat22Transpose(Mat22 const& Mtx) {
		Mat22 results{};
		for (int i{}; i < 2; i++) {
			for (int j{}; j < 2; j++) {
				results.mMat[i][j] = Mtx.mMat[j][i];
			}
		}
		return results;
	}



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


	std::ostream& operator<<(std::ostream& os, Mat22& m) {
		for (int i{}; i < 2; ++i) {
			os << m.mMat[0][i] << " " << m.mMat[1][i] << std::endl;
		}
		return os;
	}


}
