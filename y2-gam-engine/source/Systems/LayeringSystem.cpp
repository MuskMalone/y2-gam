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

// Static Initialization
std::vector<std::string> LayeringSystem::mLayerNames;
std::vector<int> LayeringSystem::mLayerVisibility;
std::vector<std::vector<int>> LayeringSystem::mCollisionMatrix;

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*! Init

@return none.

Initializes the layering system.
*/
void LayeringSystem::Init() {
  ReadFromJson(NAME_OF_FILE);
}

/*  _________________________________________________________________________ */
/*! Update

@return none.

The update function for the layering system.
*/
void LayeringSystem::Update() {

}

/*  _________________________________________________________________________ */
/*! Exit

@return none.

The exit function for the layering system. Called when the program is exiting.
*/
void LayeringSystem::Exit() {
  SerializeToFile(NAME_OF_FILE);
}

/*  _________________________________________________________________________ */
/*! ImguiLayeringWindow

@return none.

This function is called to display the layering window in ImGui. It is called
in ImguiApp.cpp.
*/
void LayeringSystem::ImguiLayeringWindow() {
#ifndef _INSTALLER
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
        // If an entity was using that layer name, change it to the new name
        for (auto const& ent : mEntities) {
          if (Coordinator::GetInstance()->HasComponent<Layering>(ent)) {
            auto& layerComponent = Coordinator::GetInstance()->GetComponent<Layering>(ent);
            if (layerComponent.assignedLayer == mLayerNames[i]) {
              layerComponent.assignedLayer = tempLayerName;
            }
          }
        }
     
        mLayerNames[i] = tempLayerName;
      }
    }

    ImGui::TreePop();
  }

  treeNodeLabel = "Visibility Toggle##";

  if (ImGui::TreeNodeEx(treeNodeLabel.c_str(), flags)) {
    for (int i{}; i < MAX_LAYERS; ++i) {
      if (mLayerNames[i] == "") continue;
      ImGui::Text("%s", (mLayerNames[i] != "") ? mLayerNames[i].c_str() : "Unnamed");
      ImGui::SameLine();
      ImGui::SetCursorPosX(SAME_LINE_SPACING);
      ImGui::Checkbox(("##readonly" + std::to_string(i)).c_str(), reinterpret_cast<bool *>(&mLayerVisibility[i]));
    }

    ImGui::TreePop();
  }

  treeNodeLabel = "Collision Matrix##";

  if (ImGui::TreeNodeEx(treeNodeLabel.c_str(), flags)) {
    for (int i{}; i < MAX_LAYERS; ++i) {
      if (mLayerNames[i] == "") continue;
      ImGui::Text("%s", mLayerNames[i].c_str());
      ImGui::SameLine();
      ImGui::SetCursorPosX(SAME_LINE_SPACING);

      for (int j{}; j < MAX_LAYERS; ++j) {
        if (mLayerNames[j] == "") continue;
        if (i >= j) continue;

        ImGui::Checkbox(
          ("##Checkbox" + std::to_string(i) + "_" + std::to_string(j)).c_str(),
          reinterpret_cast<bool*>(&mCollisionMatrix[i][j])
        );
        ImGui::SameLine(0,3);
        ImGui::Text("%d", j);
        ImGui::SameLine(0,3);
      }
      ImGui::NewLine();
    }

    ImGui::TreePop();
  }

  ImGui::End();
#endif
}

/*  _________________________________________________________________________ */
/*! ReadFromJson

@param filename
The name of the file to read from.

@return none.

Reads the layering system data from a JSON file.
*/
void LayeringSystem::ReadFromJson(std::string const& filename) {
  std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };
  if (!sm->OpenJSON(filename)) {
    // JSON file does not exist, use default values
    mLayerNames.reserve(MAX_LAYERS);
    mLayerVisibility.reserve(MAX_LAYERS);

    for (int i{}; i < MAX_LAYERS; ++i) {
      mLayerNames.emplace_back("");
      mLayerVisibility.emplace_back(true);
    }
    mLayerNames[0] = "Default";
    mCollisionMatrix.resize(MAX_LAYERS, std::vector<int>(MAX_LAYERS, true));

    return;
  }

  if (!sm->At("Layers").IsArray()) return;

  for (auto const& item : sm->At("Layers").GetArray()) {
    const auto arr{ item[NAME_OF_SERIALIZED_LAYER_NAMES].GetArray() };
    mLayerNames.reserve(MAX_LAYERS);
    for (auto const& v : arr) {
      mLayerNames.emplace_back(v.GetString());
    }

    const auto flagArr{ item[NAME_OF_SERIALIZED_VISIBILITY_FLAGS].GetArray() };
    mLayerVisibility.reserve(MAX_LAYERS);
    for (auto const& v : flagArr) {
      mLayerVisibility.emplace_back(v.GetInt());
    }

    mCollisionMatrix.resize(MAX_LAYERS, std::vector<int>(MAX_LAYERS, true));
    for (int i{}; i < MAX_LAYERS; ++i) {
      std::string combinedStr{ std::string(NAME_OF_SERIALIZED_COLLISION_MATRIX) + std::to_string(i) };
      const auto matrixArr{ item[combinedStr.c_str()].GetArray() };
      for (int j{}; j < MAX_LAYERS; ++j) {
        mCollisionMatrix[i][j] = matrixArr[j].GetInt();
      }
    }
  }
}

/*  _________________________________________________________________________ */
/*! SerializeToFile

@param filename
The name of the file to write to.

@return bool
Returns true if the file is successfully written to, false otherwise.

This function is called to write the layering system data to a JSON file.
*/
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

/*  _________________________________________________________________________ */
/*! Serialize

@param obj
The JSON object to write to.

@return none.

Serializes the layering system data to a JSON object.
*/
void LayeringSystem::Serialize(rapidjson::Value& obj) {
  std::shared_ptr<Serializer::SerializationManager> sm{ Serializer::SerializationManager::GetInstance() };

  rapidjson::Value objArr{ rapidjson::kArrayType };
  objArr.SetArray();
  // Push each layer name to the array
  for (auto& layerNames : mLayerNames) {
    sm->PushToArray(objArr, layerNames);
  }

  sm->InsertValue(obj, NAME_OF_SERIALIZED_LAYER_NAMES, objArr);

  rapidjson::Value flagArr{ rapidjson::kArrayType };
  flagArr.SetArray();
  // Push each layer name to the array
  for (int visibilityFlag : mLayerVisibility) {
    sm->PushToArray(flagArr, visibilityFlag);
  }

  sm->InsertValue(obj, NAME_OF_SERIALIZED_VISIBILITY_FLAGS, flagArr);

  for (int i{}; i < MAX_LAYERS; ++i) {
    rapidjson::Value collisionArr{ rapidjson::kArrayType };
    collisionArr.SetArray();

    for (int j{}; j < MAX_LAYERS; ++j) {
      sm->PushToArray(collisionArr, mCollisionMatrix[i][j]);
    }
    sm->InsertValue(obj, NAME_OF_SERIALIZED_COLLISION_MATRIX + std::to_string(i), collisionArr);
  }
}

/*  _________________________________________________________________________ */
/*! IsLayerVisible

@param layerName
The name of the layer to check.

@return bool
'false' if the layer is not visible, 'true' otherwise.

Returns whether the layer is visible or not, by cross checking the 
two vectors.
*/
bool LayeringSystem::IsLayerVisible(std::string const& layerName) {
  int index{};
  for (auto const& name : mLayerNames) {
    if (name == layerName) {
      break;
    }
    ++index;
  }

  return mLayerVisibility[index];
}

/*  _________________________________________________________________________ */
/*! IsCollidable

@param lhsName
The lhs layer name to check.

@param rhsName
The rhs layer name to check.

@return bool
'false' if layers are not collidable, 'true' otherwise.

Returns whether two layers are collidable or not.
*/
bool LayeringSystem::IsCollidable(std::string const& lhsName, std::string const& rhsName) {
  int idxLhs{};
  int idxRhs{};
  int idx{};

  // For now, assume same layer collides with same layer
  if (lhsName == rhsName)
    return true;

  for (auto const& str : mLayerNames) {
    if (lhsName == str) {
      idxLhs = idx;
    }

    if (rhsName == str) {
      idxRhs = idx;
    }

    ++idx;
  }

  // As lower layer takes priority
  if (idxRhs < idxLhs)
    std::swap(idxLhs, idxRhs);
  
  return mCollisionMatrix[idxLhs][idxRhs];
}
