/******************************************************************************/
/*!
\par        Image Engine
\file       LayeringSystem.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 11, 2023

\brief      The source file for the layering system.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Systems/LayeringSystem.hpp"
#include "Core/Coordinator.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void LayeringSystem::Init() {
	// Should be reading from serialized data
	mLayerNames.reserve(MAX_LAYERS);
	for (int i{}; i < MAX_LAYERS; ++i) {
    mLayerNames.emplace_back("");
  }
	mLayerNames[0] = "Default";
}

void LayeringSystem::Update() {

}

void LayeringSystem::Exit() {

}

void LayeringSystem::ImguiLayeringWindow() {
  ImGui::Begin("Layers");
  std::string treeNodeLabel = "Define Layers##";

  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
  if (ImGui::TreeNodeEx(treeNodeLabel.c_str(), flags)) {
    ImGui::Text("%s", "Built-in Layer:");
    ImGui::SameLine();
    ImGui::SetCursorPosX(SAME_LINE_SPACING);
    ImGui::Text("%s", "Default");

    for (int i = 1; i <= MAX_USER_DEFINED_LAYERS; ++i) {
      std::string description = "User Layer " + std::to_string(i) + ":";
      ImGui::Text("%s", description.c_str());
      ImGui::SameLine();
      ImGui::SetCursorPosX(SAME_LINE_SPACING);
      ImGui::SetNextItemWidth(TEXT_BOX_WIDTH);
      char tempLayerName[256]; // Temp Buffer
      strncpy(tempLayerName, mLayerNames[i].c_str(), sizeof(tempLayerName) - 1);
      tempLayerName[sizeof(tempLayerName) - 1] = '\0';

      if (ImGui::InputText(("##readonly" + std::to_string(i)).c_str(), tempLayerName, sizeof(tempLayerName), ImGuiInputTextFlags_EnterReturnsTrue)) {
        mLayerNames[i] = tempLayerName;
      }
    }

    ImGui::TreePop();
  }

  ImGui::End();
}
