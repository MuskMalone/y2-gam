/******************************************************************************/
/*!
\par        Image Engine
\file       TextSystem.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 23, 2023

\brief      The header file for the text system.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Core/System.hpp"

class TextSystem : public System {
public:
	void Init();
	void Update();
	void Exit();

	static std::vector<const char*> FontTypes;
	static Vec2 WorldToScreenCoordinates(Vec2 worldCoordinates);

private:

};