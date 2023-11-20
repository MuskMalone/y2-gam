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
constexpr float SAME_LINE_SPACING = 125.f;
constexpr float TEXT_BOX_WIDTH = 100.f;

constexpr const char* NAME_OF_FILE = "Layers";
constexpr const char* NAME_OF_SERIALIZED_LAYER_NAMES = "layerNames";
constexpr const char* NAME_OF_SERIALIZED_VISIBILITY_FLAGS = "layerVisibility";

class LayeringSystem : public System {
public:
	void Init();
	void Update();
	void Exit();

	void ImguiLayeringWindow();

	// Serialization Related
	void ReadFromJson(std::string const& filename);
	bool SerializeToFile(std::string const& filename);
	void Serialize(rapidjson::Value& obj);

	// Getters
	static std::vector<std::string>const& GetLayerNames() { return mLayerNames; }
	static std::vector<int>const& GetLayerVisibility() { return mLayerVisibility; }

	// Additional Functionality
	static bool IsLayerVisible(std::string const& layerName);
	static bool IsCollidable(std::string const& lhsName, std::string const& rhsName);

private:
	static std::vector<std::string> mLayerNames;
	static std::vector<int> mLayerVisibility;
	static std::vector<std::vector<int>> mCollisionMatrix;
};