/******************************************************************************/
/*!
\par        Image Engine
\file       LayeringSystem.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 11, 2023

\brief      The header file for the layering system.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Core/System.hpp"

constexpr size_t MAX_USER_DEFINED_LAYERS = 10;
constexpr size_t MAX_BUILTIN_LAYERS = 1;
constexpr size_t MAX_LAYERS = MAX_USER_DEFINED_LAYERS + MAX_BUILTIN_LAYERS;
constexpr float SAME_LINE_SPACING = 150.f;
constexpr float TEXT_BOX_WIDTH = 100.f;

class LayeringSystem : public System {
public:
	void Init();
	void Update();
	void Exit();
	void ImguiLayeringWindow();

	std::vector<std::string>const& GetLayerNames() const { return mLayerNames; }

private:
	std::vector<std::string> mLayerNames;
};