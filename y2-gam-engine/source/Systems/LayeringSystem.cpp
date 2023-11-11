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
#include "Core/Serialization/SerializationManager.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void LayeringSystem::Init() {
  // Read from serialized data
  ReadFromJson(NAME_OF_FILE);
}

void LayeringSystem::ReadFromJson(std::string const& filename) {
  std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
  if (!sm->OpenJSON(filename)) { // JSON file does not exist
    mLayerNames.reserve(MAX_LAYERS);
    for (int i{}; i < MAX_LAYERS; ++i) {
      mLayerNames.emplace_back("");
    }
    mLayerNames[0] = "Default";
    return;
  }

  if (!sm->At("Layers").IsArray()) return;

  for (auto const& item : sm->At("Layers").GetArray()) {
    const auto arr = item[NAME_OF_SERIALIZED_ARRAY].GetArray();
    mLayerNames.reserve(MAX_LAYERS);
    for (auto const& v : arr) {
      mLayerNames.emplace_back(v.GetString());
    }
  }
}

void LayeringSystem::Update() {

}

void LayeringSystem::Exit() {
  SerializeToFile(NAME_OF_FILE);
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

bool LayeringSystem::SerializeToFile(std::string const& filename) {
  std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

  sm->ClearJSON(filename);
  sm->SetObject(filename);

  Serializer::JSONObj finalArr{ JSON_ARR_TYPE };

  Serializer::JSONObj obj{ JSON_OBJ_TYPE };
  Serialize(obj);
  if (!obj.ObjectEmpty())
    sm->PushToArray(filename, finalArr, obj);

  sm->InsertValue("Layers", finalArr);

  if (!sm->FlushJSON(filename)) {
    return false;
  }

  return true;
}

bool LayeringSystem::Serialize(rapidjson::Value& obj) {
  std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

  rapidjson::Value objArr{ rapidjson::kArrayType };
  objArr.SetArray();
  // Push each layer name to the array
  for (auto& layerNames : mLayerNames) {
    sm->PushToArray(objArr, layerNames);
  }

  sm->InsertValue(obj, NAME_OF_SERIALIZED_ARRAY, objArr);

  return true;
}
