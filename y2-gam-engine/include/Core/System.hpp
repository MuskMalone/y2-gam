#pragma once
/******************************************************************************/
/*!
\par        Image Engine
\file       System.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "Types.hpp"
#include <set>


class System
{
public:
	std::set<Entity> mEntities;
};
