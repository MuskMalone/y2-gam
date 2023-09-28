#include <iostream>
#include "Vec2.h"
#include "Vec3.h" 
#include "Vec4.h"
#include "Mat3.h"
#include "mat4.h"
#include "Mat2.h"
#include "MathUtils.h"
int main(){
    // Test cases for Vec2
    //Image::Vec2 v2a(1.0, 2.0);
    //Image::Vec2 v2b(0.5, 1.5);

    //Image::Vec2 v2Add = v2a + v2b;
    //Image::Vec2 v2Sub = v2a - v2b;
    //Image::Vec2 v2Mul = v2a * 2.0;
    //Image::Vec2 v2Div = v2a / 2.0;
    //float v2Length = length(v2a);
    //float v2Dot = dot(v2a,v2b);

    //std::cout << "Vec2 Addition: " << v2Add << std::endl;
    //std::cout << "Vec2 Subtraction: " << v2Sub << std::endl;
    //std::cout << "Vec2 Multiplication: " << v2Mul << std::endl;
    //std::cout << "Vec2 Division: " << v2Div << std::endl;
    //std::cout << "Vec2 Length: " << v2Length << std::endl;
    //std::cout << "Vec2 Dot Product: " << v2Dot << std::endl;

    //// Test cases for Vec3
    //Image::Vec3 v3a(1.0, 2.0, 3.0);
    //Image::Vec3 v3b(0.5, 1.5, 2.5);

    //Image::Vec3 v3Add = v3a + v3b;
    //Image::Vec3 v3Sub = v3a - v3b;
    //Image::Vec3 v3Mul = v3a * 2.0;
    //Image::Vec3 v3Div = v3a / 2.0;
    ////Vec3 v3v3 = v3a * v3b;
    //float v3Length = length(v3a);
    //float v3Dot = dot(v3a,v3b);
    //Image::Vec3 v3Cross = cross(v3a,v3b);
    ////v3Add[5];
    //std::cout << "Vec3 Addition: " << v3Add << std::endl;
    //std::cout << "Vec3 Subtraction: " << v3Sub << std::endl;
    //std::cout << "Vec3 Multiplication: " << v3Mul << std::endl;
    //std::cout << "Vec3 Division: " << v3Div << std::endl;
    //std::cout << "Vec3 Length: " << v3Length << std::endl;
    //std::cout << "Vec3 Dot Product: " << v3Dot << std::endl;
    //std::cout << "Vec3 Cross Product: " << v3Cross << std::endl;

    //// Test cases for Vec4
    //Image::Vec4 v4a(1.0, 2.0, 3.0, 4.0);
    //Image::Vec4 v4b(0.5, 1.5, 2.5, 3.5);

    //Image::Vec4 v4Add = v4a + v4b;
    //Image::Vec4 v4Sub = v4a - v4b;
    //Image::Vec4 v4Mul = v4a * 2.0;
    //Image::Vec4 v4Div = v4a / 2.0;
    //float v4Length = length(v4a);
    //float v4Dot = dot(v4a,v4b);

    //std::cout << "Vec4 Addition: " << v4Add << std::endl;
    //std::cout << "Vec4 Subtraction: " << v4Sub << std::endl;
    //std::cout << "Vec4 Multiplication: " << v4Mul << std::endl;
    //std::cout << "Vec4 Division: " << v4Div << std::endl;
    //std::cout << "Vec4 Length: " << v4Length << std::endl;
    //std::cout << "Vec4 Dot Product: " << v4Dot << std::endl;

    // Test default constructor
    //Image::Mat33 MatDefault;
    //std::cout << "Default constructor:" << std::endl << MatDefault << std::endl;

    // Test constructor with a diagonal value
    //Image::Mat33 MatDiagonal(2.0f);
    //std::cout << "Constructor with diagonal value:" << std::endl << MatDiagonal << std::endl;

    // Test constructor with column Vectors
    //Image::Vec3 col1(1.0f, 3.0f, 0.0f);
    //Image::Vec3 col2(0.0f, 1.0f, 0.0f);
    //Image::Vec3 col3(0.0f, 0.0f, 1.0f);
    //Image::Mat33 MatCols(col1, col2, col3);
    //std::cout << "Constructor with column Vectors:" << std::endl << MatCols << std::endl;

    // Test addition, subtraction, multiplication
    //Image::Mat33 MatA(Image::Vec3(1.0f, 2.0f, 3.0f), Image::Vec3(4.0f, 5.0f, 6.0f), Image::Vec3(7.0f, 8.0f, 9.0f));
    //Image::Mat33 MatB(Image::Vec3(9.0f, 8.0f, 7.0f), Image::Vec3(6.0f, 5.0f, 4.0f), Image::Vec3(3.0f, 2.0f, 1.0f));

    //Image::Mat33 MatAdd = MatA + MatB;
    //Image::Mat33 MatSub = MatA - MatB;
    //Image::Mat33 MatMul = MatA * MatB;

    //std::cout << "Matrix addition:" << std::endl << MatAdd << std::endl;
    //std::cout << "Matrix subtraction:" << std::endl << MatSub << std::endl;
    //std::cout << "Matrix multiplication:" << std::endl << MatMul << std::endl;

    // Test scalar operations
    //Image::Mat33 MatScalar(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
    //float scalar = 2.0f;

    //Image::Mat33 MatScalarAdd = MatScalar + scalar;
    //Image::Mat33 MatScalarSub = MatScalar - scalar;
    //Image::Mat33 MatScalarMul = MatScalar * scalar;
    //Image::Mat33 MatScalarDiv = MatScalar / scalar;

    //std::cout << "Matrix scalar addition:" << std::endl << MatScalarAdd << std::endl;
    //std::cout << "Matrix scalar subtraction:" << std::endl << MatScalarSub << std::endl;
    //std::cout << "Matrix scalar multiplication:" << std::endl << MatScalarMul << std::endl;
    //std::cout << "Matrix scalar division:" << std::endl << MatScalarDiv << std::endl;

    // Test shorthand operators
    //Image::Mat33 MatShorthand(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
    //std::cout << MatShorthand<< std::endl;
    //Image::Mat33 Mattemp{};
    //Image::Mat33 Mattrans{}, Matinv{1,0,5,2,1,6,3,4,0};
    //Mat33Transpose(Mattrans, MatShorthand);
    //Mat33Inverse(Matinv, Matinv);
    //std::cout << "Mat inv" << std::endl << Matinv << std::endl;
    //MatShorthand += 2.0f;
    //MatShorthand -= 1.0f;
    //MatShorthand *= 3.0f;
    //MatShorthand /= 2.0f;
    //Image::Mat33 testide{};
    //Mat33Identity(testide);
    //std::cout << "Matrix shorthand operators:" << std::endl << MatShorthand<<testide<<Mattrans << std::endl;


Image::Mat33 identity;
Image::Mat33Identity(identity);
std::cout << "Identity matrix:\n" << identity << std::endl;

// Test constructor with a single value
Image::Mat33 filledMatrix(2.0f);
std::cout << "Filled matrix:\n" << filledMatrix << std::endl;

// Test constructor with individual values
Image::Mat33 customMatrix(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f);
std::cout << "Custom matrix:\n" << customMatrix << std::endl;

Image::Vec3 test1(2.0f, 2.0f, 3.0f);
Image::Vec3 test2(4.0f, 5.0f, 6.0f);
Image::Vec3 test3(7.0f, 8.0f, 9.0f);
 //Test constructor with vectors
Image::Mat33 customVecMatrix(test1,test2,test3);
std::cout << "Custom vector matrix:\n" << customVecMatrix << std::endl;

// Test operator[] and matrix element access
std::cout << "Element at (0, 2) of customMatrix: " << customVecMatrix.mMat[0][2] << std::endl;

// Test operator+
Image::Mat33 sum = customMatrix + filledMatrix;
std::cout << "\nSum:\n" << sum << std::endl;
std::cout << "Element at (1, 2) of sum: " << sum.mMat[1][2] << std::endl;

// Test operator-
Image::Mat33 diff = customMatrix - filledMatrix;
std::cout << "\nDifference:\n" << diff << std::endl;

// Test operator*
Image::Mat33 product = customMatrix * filledMatrix;
std::cout << "Product:\n" << product << std::endl;

// Test scalar operators
Image::Mat33 scaled = customMatrix * 2.0f;
std::cout << "Scaled:\n" << scaled << std::endl;

// Test operator+=
Image::Mat33 increment = identity;
increment += customMatrix;
std::cout << "Incremented:\n" << increment << std::endl;

// Test operator-=
Image::Mat33 decrement = identity;
decrement -= customMatrix;
std::cout << "Decremented:\n" << decrement << std::endl;

// Test transpose
Image::Mat33 transposed;
Image::Mat33Transpose(transposed, customMatrix);
std::cout << "Transposed:\n" << transposed << std::endl;

// Test inverse (Note: You need to implement this function)
Image::Mat33 Matinv{ 1,0,5,2,1,6,3,4,0 };
Image::Mat33Inverse(Matinv, Matinv);
std::cout << "Inverted:\n" << Matinv << std::endl;

// Test equality operator
bool isEqual = (customMatrix == customMatrix);
std::cout << "Equality: " << isEqual << std::endl;
float rad = Image::Degree(0.523598);
std::cout << rad;
    // Test default constructor and Image::Mat44Identity
    //Image::Mat44 identity;
    //Image::Mat44Identity(identity);
    //std::cout << "Identity matrix:\n" << identity << std::endl;

    //// Test constructor with a single value
    //Image::Mat44 filledMatrix(2.0f);

    //// Test constructor with individual values
    //Image::Mat44 customMatrix{ 1.0f, 2.0f, 3.0f, 4.0f,5.0f, 6.0f, 7.0f, 8.0f,9.0f, 10.0f, 11.0f, 12.0f,13.0f, 14.0f, 15.0f, 16.0f };
    //std::cout <<"testing customMat \n" << customMatrix << std::endl;

    //Image::Mat44 customvecMatrix(
    //    Image::Vec4(1.0f, 2.0f, 3.0f, 4.0f),
    //    Image::Vec4(5.0f, 6.0f, 7.0f, 8.0f),
    //    Image::Vec4(9.0f, 10.0f, 11.0f, 12.0f),
    //    Image::Vec4(13.0f, 14.0f, 15.0f, 16.0f)
    //);
    //std::cout << "testing veccustomMat \n" << customvecMatrix <<customvecMatrix.mMat[2]<< std::endl;


    //// Test operator[] and matrix element access
    //std::cout << "Element at (1, 2) of customMatrix:" << customMatrix.mMat[1][2] << std::endl;

    //// Test operator+
    //Image::Mat44 sum = customMatrix + filledMatrix;

    //// Test operator-
    //Image::Mat44 diff = customMatrix - filledMatrix;

    //// Test operator*
    //Image::Mat44 product = customMatrix * filledMatrix;// check this

    //// Test scalar operators
    //Image::Mat44 scaled = customMatrix * 2.0f;
    //Image::Mat44 added = customMatrix + 1.0f;

    //// Test operator+=
    //Image::Mat44 increment = identity;
    //increment += customMatrix;

    //// Test operator-=
    //Image::Mat44 decrement = customMatrix;
    //decrement -= identity;

    //// Test transpose
    //Image::Mat44 transposed;
    //Image::Mat44Transpose(transposed, customMatrix);

    //// Test inverse
    //Image::Mat44 inverted{1,0,2,2,0,2,1,0,0,1,0,1,1,2,1,4};
    //std::cout << "to Inverted:\n" << inverted << std::endl;
    //Image::Mat44Inverse(inverted, inverted);

    //// Test equality operator
    //bool isEqual = (customMatrix == customMatrix);

    //// Output the results
    //std::cout << "Sum:\n" << sum << std::endl;
    //std::cout << "Difference:\n" << diff << std::endl;
    //std::cout << "Product:\n" << product << std::endl;
    //std::cout << "Scaled:\n" << scaled << std::endl;
    //std::cout << "Added:\n" << added << std::endl;
    //std::cout << "Incremented:\n" << increment << std::endl;
    //std::cout << "Decremented:\n" << decrement << std::endl;
    //std::cout << "Transposed:\n" << transposed << std::endl;
    //std::cout << "Inverted:\n"<< inverted << std::endl;
    //std::cout << "Equality: " << isEqual << std::endl;
//
//Image::Mat22 mat1;
//std::cout << mat1;
//Image::Mat22 mat2(5.0f);
//std::cout << mat2;
//Image::Mat22 Mat33(1.0f, 2.0f, 3.0f, 4.0f);
//std::cout << Mat33;
//Image::Mat22 mat4(1.0f, 2.0f, 3.0f, 4.0f);
//Image::Mat22 mat5(4.0f, 3.0f, 2.0f, 1.0f);
//Image::Mat22 result1 = mat4 + mat5;
//std::cout << result1;
//Image::Mat22 mat6(1.0f, 2.0f, 3.0f, 4.0f);
//Image::Mat22 result2 = mat6 * 2.0f;
//std::cout << result2;
//Image::Mat22 mat7;
//Image::Mat22RotDeg(mat7, 90.0f);
//std::cout << mat7;
//Image::Mat22 mat8(1.0f, 2.0f, 3.0f, 4.0f);
//Image::Mat22 result3;
//Image::Mat22Transpose(result3, mat8);
//std::cout << result3;
//Image::Mat22 mat9(1.0f, 2.0f, 3.0f, 4.0f);
//Image::Mat22 result4;
//Image::Mat22Inverse(result4, mat9);
//std::cout << result4;
//Image::Mat22 mat10;
//Image::Mat22Scale(mat10, 2.0f, 3.0f);
//std::cout << mat10;
//Image::Mat22 mat11;
//Image::Mat22RotDeg(mat11, 45.0f);
//std::cout << mat11;
//Image::Mat22 mat12;
//Image::Mat22RotRad(mat12, 3.14159f / 4.0f);
//std::cout << mat12;
//Image::Mat22 mat13;
//Image::Mat22Identity(mat13);
//std::cout << mat13;
    return 0;
}
