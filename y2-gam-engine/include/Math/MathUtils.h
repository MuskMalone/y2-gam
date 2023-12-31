/******************************************************************************/
/*!
\par        Image Engine
\file       MathUtils.h

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 15, 2023

\brief      Header file for addition math function

			This file contains additional math functions. The functions
			includes degree and radian.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <math.h>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat2.h"
#include "Mat3.h"
#include "Mat4.h"

using namespace Image;
namespace Image {
	constexpr float gPI{ 3.141592653589793238462643383279502884197169399375105f };

	float Degree(float rad);

	float Radian(float degree);
}