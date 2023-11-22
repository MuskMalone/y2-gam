/******************************************************************************/
/*!
\par        Image Engine
\file       TextSystem.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       22 Nov, 2023

\brief      The header file for the UI system.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Core/System.hpp"
#include "Components/Transform.hpp"
#include "Math/MathUtils.h"

class UISystem : public System {
public:
	void Init();
	void Update();
	void Exit();

	static bool IsMouseOverButton(Vec2 const& mousePos, Transform const& transform);
};