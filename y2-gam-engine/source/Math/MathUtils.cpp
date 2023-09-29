/******************************************************************************/
/*!
\par        Image Engine
\file       MathUtils.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 15, 2023

\brief      Implementation file for addition math function

			This file contains additional math functions. The functions 
			includes degree and radian.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Math/MathUtils.h"
namespace Image {
	/*  _________________________________________________________________________ */
	/*! Degree

	@param rad
	Radian to convert

	@return float.
	Degree after conversion

	This function is responsible for converting radian to degree
	*/
	float Degree(float rad) {
		return rad * (180.f / gPI);
	}
	/*  _________________________________________________________________________ */
	/*! Radian

	@param degree
	Degree to convert

	@return float.
	Radian after conversion

	This function is responsible for converting degree to radian
	*/
	float Radian(float degree) {
		return degree * (gPI / 180.f);
	}

}
