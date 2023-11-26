/******************************************************************************/
/*!
\par        Image Engine
\file       ImguiApp.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Implementation file for Imgui

            This file contains functions responsible for rendering different 
            parts of the application's GUI. It includes the main rendering function, 
            as well as functions for displaying various windows such as the main 
            menu, hierarchy, inspector, property, buffer, prefab buffer, texture
            performance, contet browser and logging windows. 

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Imgui/ImguiApp.hpp"
#include "IMGUI/ImguiComponent.hpp"
#include "Math/MathUtils.h"
#include "ImGuizmo.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>       
#include "Components/Script.hpp"
#include "Components/UIImage.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/ImguiSystem.hpp"
#include "Logging/LoggingSystem.hpp"
#include <Core/Globals.hpp>
#include <Core/FrameRateController.hpp>
#include "Graphics/Renderer.hpp"
#include "Scripting/NodeManager.hpp"
#include "Scripting/ScriptManager.hpp"

#include <Engine/CommandManager.hpp>
#include <Engine/AssetManager.hpp>
#include <Engine/SceneManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Audio/Sound.hpp>
#include <IMGUI/AssetBrowser.hpp>
#include <IMGUI/PrefabsBrowser.hpp>

ImGuizmo::OPERATION gCurrentGuizmoOperation{ImGuizmo::OPERATION::TRANSLATE};
ImGuizmo::MODE gCurrentGizmoMode{ ImGuizmo::LOCAL };

namespace {
    std::shared_ptr<Coordinator> gCoordinator;
    const int   gPercent = 100;
    const float gScalingFactor = 1.5f;
    bool gSnap = false;
    float gSnapVal = 1.f;
    Entity gSelectedEntity = MAX_ENTITIES;
    Entity gSelectedPrefab = MAX_ENTITIES;
    //std::string gCurrentScene = "";
}
namespace Image {
    /*  _________________________________________________________________________ */
    /*! AppRender

    @param mEntities
    The set of Entities to loop through.

    @return none.

    This function is responsible for rendering the application's UI and
    handling user input.
    Pressing the 'c' key to clear the entities
    */
    void AppRender(std::set<Entity>const& mEntities,float dt) {
        ::gCoordinator = Coordinator::GetInstance();
        ImGui::PushFont(mainfont);
        //static bool toDelete{ false };

        //if (ImGui::IsKeyReleased(ImGuiKey_C)) {
        //    NodeManager::ClearAllNodes();
        //    toDelete = !toDelete;
        //}

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        //ImGui::ShowDemoWindow();
        MainMenuWindow();
        PerformanceWindow();
        HierarchyWindow(mEntities);
        LayerWindow();
        //InspectorWindow();
        //PropertyWindow(gSelectedEntity);
        PrefabPropertyWindow();
        GameObjectPropertyWindow();
        PrefabInspectorWindow();
        GameObjectInspectorWindow();
        PrefabWindow();
        BufferWindow(dt);
        ContentWindow();
        //AssetWindow(mEntities);
        AssetWindow(mEntities);
        AssetPropertiesWindow(mEntities);
        GuizmoWindow();
        LoggingWindow();
        RenderStatsWindow();
        //if (toDelete) {
        //    std::vector<Entity> deleteEntites{};
        //    for (auto& e : mEntities) {
        //        deleteEntites.emplace_back(e);
        //    }
        //    for (auto const& e : deleteEntites) {
        //        //TEMP TO BE DELETED
        //        //TODO 
        //        if (!gCoordinator->HasComponent<Script>(e) && std::to_string(e) != "2" && std::to_string(e) != "3" && std::to_string(e) != "4") {
        //            gCoordinator->DestroyEntity(e);
        //        }
        //    }
        //    gSelectedEntity = MAX_ENTITIES;
        //    toDelete = !toDelete;

        //}
        ImGui::PopFont();
    }
    /*  _________________________________________________________________________ */
    /*! MainMenuWindow

    @param none

    @return none.

    Displaying the menu bar.
    */
    void MainMenuWindow() {
        ImGui::PushFont(mainfont);
        if (ImGui::BeginMainMenuBar())
        {
            //Read files maybe
            if (ImGui::BeginMenu("File"))
            {
                //if (ImGui::MenuItem("New", "Cltr+N")) {

                //}
                //if (ImGui::MenuItem("Open...", "Ctrl+O")) {

                //}
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    SceneManager::GetInstance()->SaveScene();
                }
                //if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {

                //}
                ImGui::EndMenu();
            }

            //EDIT
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {
                    CommandManager::GetInstance()->UndoCommand();
                }
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            //View
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Animation Editer")) {}
                ImGui::EndMenu();
            }
            auto renderSystem = gCoordinator->GetSystem<RenderSystem>();
            if (ImGui::MenuItem("Play")) {
                if (renderSystem->IsEditorMode() && SceneManager::GetInstance()->IsSceneActive()) {
                    SceneManager::GetInstance()->ModifyScene();
                   // std::cout << "Play to toggle to editer play mode" << std::endl;
                    renderSystem->ToggleEditorMode();
                    ImGui::SetWindowFocus("Image Game Engine");
                }
            }
            if (ImGui::MenuItem("Stop")) {
                if (SceneManager::GetInstance()->IsSceneActive()) {
                    if (!renderSystem->IsEditorMode()) {
                        //std::cout << "Stop to toggle to editer mode" << std::endl;
                        renderSystem->ToggleEditorMode();
                        ImGui::SetWindowFocus("Image Game Engine");

                    }
                    SceneManager::GetInstance()->ResetScene();

                }
            }
            ImGui::PopFont();
            ImGui::EndMainMenuBar();
        }
    }
    /*  _________________________________________________________________________ */
    /*! HierarchyWindow

    @param mEntities
     Set of entities to loop through and display on the Hierarchy panel

    @return none.

    This function displays the hierarchy of entities and allows for the creation
    and destruction of entities. The entities have a default ImGui component to
    listen from.
    */
    void HierarchyWindow(std::set<Entity>const& mEntities) {
        ImGui::PushFont(mainfont);
        auto scenemgr{ SceneManager::GetInstance() };
        // Hierarchy Panel
        ImGui::Begin("Hierarchy");
        //Create entity and destory first
        std::string scenestring{"Current Scene: " + ((!scenemgr->IsSceneActive()) ? std::string{"No Scene Selected"} : scenemgr->GetSceneName())};
        ImGui::Text(scenestring.c_str());
        if (ImGui::BeginPopupContextWindow("Hierarchy Context Menu", ImGuiPopupFlags_MouseButtonRight)) {
            if (ImGui::MenuItem("Create Entity")) {

                if (scenemgr->IsSceneActive()) {
                    Entity newEntity = gCoordinator->CreateEntity();
                    gSelectedEntity = newEntity;
                    ImGuiViewport* vP = ImGui::GetWindowViewport();
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        Layering{ "Default" });
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        Transform{
                            {vP->Pos.x,vP->Pos.y,0},
                            {0,0,0},
                            {IMGUI_SCALE,IMGUI_SCALE,IMGUI_SCALE}
                        });
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        Tag{ "Name" });
                    gCoordinator->AddComponent(
                      gSelectedEntity,
                      Sprite{
                          {1,1,1, 1}
                      });
                    CommandManager::GetInstance()->AddCommand("Create", newEntity);

                }
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Create Entity")) {
            if (SceneManager::GetInstance()->IsSceneActive()) {
                Entity newEntity = gCoordinator->CreateEntity();
                gSelectedEntity = newEntity;
                ImGuiViewport* vP = ImGui::GetWindowViewport();
                gCoordinator->AddComponent(
                  gSelectedEntity,
                  Layering{ "Default" });
                gCoordinator->AddComponent(
                    gSelectedEntity,
                    Transform{
                        {vP->Pos.x,vP->Pos.y,0},
                        {0,0,0},
                        {IMGUI_SCALE,IMGUI_SCALE,IMGUI_SCALE}
                    });
                gCoordinator->AddComponent(
                    gSelectedEntity,
                    Tag{ "Name" });
                gCoordinator->AddComponent(
                  gSelectedEntity,
                  Sprite{
                      {1,1,1, 1}
                  });
                CommandManager::GetInstance()->AddCommand("Create", newEntity);

            }

        }

        //Cant destroy player
        // Ernest: Can delete now
        if (gSelectedEntity != MAX_ENTITIES && ImGui::Button("Destroy Entity")) {
            //if (!gCoordinator->HasComponent<Script>(gSelectedEntity)) {
                //put before u destroy the entity
            CommandManager::GetInstance()->AddCommand("Destroy", gSelectedEntity, Serializer::SaveEntities(gSelectedEntity));

            gCoordinator->DestroyEntity(gSelectedEntity);
            Image::ScriptManager::RemoveEntity(gSelectedEntity);
            gSelectedEntity = MAX_ENTITIES;
            //}
                //if (gCoordinator->HasComponent<Script>(gSelectedEntity)) {
                  //ScriptManager::RemoveEntity(gSelectedEntity);
                //}

        }

        for (auto const& entity : mEntities) {
            std::string displayName = std::to_string(entity);
            if (gCoordinator->HasComponent<Tag>(entity)) {
                Tag& tagComponent = gCoordinator->GetComponent<Tag>(entity);
                displayName = tagComponent.tag;
            }

            bool isSelected = (gSelectedEntity == entity);
            if (ImGui::Selectable(displayName.c_str(), isSelected)) {
                gSelectedEntity = entity;
            }

            if (ImGui::BeginDragDropTarget()) {
                //std::cout << "Began drag-drop target." << std::endl;

                //if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Sound AssetBrowser")) {
                //    //std::cout << "Accepted payload." << std::endl;
                //    AssetID droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                //    std::cout << droppedAid << std::endl;
                //}
                AssetID droppedAid{ static_cast<AssetID>(-1) };
                if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Sprite AssetBrowser")) {
                    //std::cout << "Accepted payload." << std::endl;
                    droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                    //std::cout << droppedAid << std::endl;
                    if (gCoordinator->HasComponent<Sprite>(entity)) {
                        auto& sprite = gCoordinator->GetComponent<Sprite>(entity);
                        sprite.spriteAssetID = droppedAid;
                    }
                    else {
                        Sprite s{{1,1,1, 1}};
                        s.spriteAssetID = droppedAid;
                        gCoordinator->AddComponent(
                            entity,
                            s);
                    }
                    SceneManager::GetInstance()->AddAsset(droppedAid);
                }
                if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Animation AssetBrowser")) {
                    //std::cout << "Accepted payload." << std::endl;
                    droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                    //std::cout << droppedAid << std::endl;
                    if (gCoordinator->HasComponent<Animation>(entity)) {
                        auto& anim = gCoordinator->GetComponent<Animation>(entity);
                        //anim.assetID = droppedAid;
                        anim.states.emplace_back(droppedAid);
                    }
                    else {
                        Animation a{
                                0.08f,
                                0,
                                0
                        };
                        //a.assetID = droppedAid;
                        a.states.emplace_back(droppedAid);
                        gCoordinator->AddComponent(
                            entity,
                            a);
                    }
                    SceneManager::GetInstance()->AddAsset(droppedAid);
                }
                ImGui::EndDragDropTarget();
            }
        }
        
        //Cant delete stuff with script
        // Ernest: Can delete now
        auto input = gCoordinator->GetSystem<InputSystem>();
        if (input->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_DELETE)) {
            if (gSelectedEntity != MAX_ENTITIES) {
                //if (!gCoordinator->HasComponent<Script>(gSelectedEntity)) {
                    CommandManager::GetInstance()->AddCommand("Destroy", gSelectedEntity, Serializer::SaveEntities(gSelectedEntity));

                    gCoordinator->DestroyEntity(gSelectedEntity);
                    Image::ScriptManager::RemoveEntity(gSelectedEntity);
                    gSelectedEntity = MAX_ENTITIES;
                //}
                //if (gCoordinator->HasComponent<Script>(gSelectedEntity)) {
                  //ScriptManager::RemoveEntity(gSelectedEntity);
                //}
            }
        }
 
            //if (ImGui::BeginPopupContextItem()) {
            //    if (gSelectedEntity != MAX_ENTITIES && ImGui::MenuItem("Destroy Entity")) {
            //        if (!gCoordinator->HasComponent<Script>(gSelectedEntity)) {
            //            gCoordinator->DestroyEntity(gSelectedEntity);
            //            gSelectedEntity = MAX_ENTITIES;
            //        }
            //    }
            //    // Add more entity interaction options here...
            //    ImGui::EndPopup();
            //}
        ImGui::PopFont();
        ImGui::End();
    }

    /*  _________________________________________________________________________ */
    /*! LayerWindow

    @param none

    @return none.

    This function allows for editing user defined layers.
    */
    void LayerWindow() {
      gCoordinator->GetSystem<LayeringSystem>()->ImguiLayeringWindow();
    }


    void CheckFirstAndLastUseTransform(bool& wasActive, const char* widgetName, Entity e) {
        bool isActive = ImGui::IsItemActive();
        if (isActive && !wasActive) {
            //std::cout << widgetName << " was first used." << std::endl;
            auto& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
            CommandManager::GetInstance()->AddCommand("Transform", gSelectedEntity, Transform{ transform });
        }
        if (!isActive && wasActive) {
            //std::cout << widgetName << " was last used." << std::endl;
        }
        wasActive = isActive;
    }

    /*  _________________________________________________________________________ */
    /*! InspectorWindow

    @param none

    @return none.

    This function displays the components of the selected entity and allows for
    modification to its components.
    */
    void InspectorWindow(Entity selectedEntity) {
        // Inspector Panel
        //ImGui::Begin("Inspector");
        //TransformComponent
        ImGui::PushFont(mainfont);

        if (selectedEntity != MAX_ENTITIES) {
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
            std::string entityidstring{"Entity ID: " + std::to_string(selectedEntity)};
            ImGui::Text(entityidstring.c_str());
            if (gCoordinator->HasComponent<Layering>(selectedEntity)) {
              std::string treeNodeLabel = "Layer##" + std::to_string(selectedEntity);
              
              ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
              if (ImGui::TreeNodeEx(treeNodeLabel.c_str(), flags)) {
                Layering& layer = gCoordinator->GetComponent<Layering>(selectedEntity);

                static int selectedOption = -1;
                std::vector<const char*> tmp;
                tmp.push_back("NONE");
                for (std::string const& name : LayeringSystem::GetLayerNames()) {
                  if (name != "")
                    tmp.push_back(name.c_str());
                }

                for (int i{}; i < tmp.size(); ++i) {
                  if (layer.assignedLayer == tmp[i]) {
                    selectedOption = i;
                    break;
                  }
                }

                static int previousOption = selectedOption;



                ImGui::Combo("Current",
                  &selectedOption,
                  tmp.data(),
                  static_cast<int>(tmp.size()));

                if (selectedOption != previousOption) {
                  previousOption = selectedOption;
                  layer.assignedLayer = std::string(tmp[selectedOption]);
                }
                tmp.clear();
                ImGui::TreePop();
              }
            }
            if (gCoordinator->HasComponent<Transform>(selectedEntity)) {
                std::string treeNodeLabel = "Transform##" + std::to_string(selectedEntity);
              
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
                if (ImGui::TreeNodeEx(treeNodeLabel.c_str(), flags)) {
                    Transform& transform = gCoordinator->GetComponent<Transform>(selectedEntity);

                    static bool wasPosXActive = false;
                    static bool wasPosYActive = false;
                    static bool wasPosZActive = false;
                    static bool wasRotZActive = false;
                    static bool wasScaleXActive = false;
                    static bool wasScaleYActive = false;

                    static bool wasPosXSliderActive = false;
                    static bool wasPosYSliderActive = false;
                    static bool wasPosZSliderActive = false;
                    static bool wasRotZSliderActive = false;
                    static bool wasScaleXSliderActive = false;
                    static bool wasScaleYSliderActive = false;

                    // Position X
                    ImGui::Text("Position");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Pos X", &transform.position.x);
                    CheckFirstAndLastUseTransform(wasPosXActive, "InputFloat Pos X", selectedEntity);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Pos X", &transform.position.x, -ENGINE_SCREEN_WIDTH, ENGINE_SCREEN_WIDTH);
                    CheckFirstAndLastUseTransform(wasPosXSliderActive, "SliderFloat Pos X", selectedEntity);

                    // Position Y
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Pos Y", &transform.position.y);
                    CheckFirstAndLastUseTransform(wasPosYActive, "InputFloat Pos Y", selectedEntity);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Pos Y", &transform.position.y, -ENGINE_SCREEN_HEIGHT, ENGINE_SCREEN_HEIGHT);
                    CheckFirstAndLastUseTransform(wasPosYSliderActive, "SliderFloat Pos Y", selectedEntity);

                    // Position Z
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Pos Z", &transform.position.z);
                    CheckFirstAndLastUseTransform(wasPosZActive, "InputFloat Pos Z", selectedEntity);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Pos Z", &transform.position.z, -ENGINE_SCREEN_HEIGHT, ENGINE_SCREEN_HEIGHT);
                    CheckFirstAndLastUseTransform(wasPosZSliderActive, "SliderFloat Pos Z", selectedEntity);

                    // Rotation
                    ImGui::Text("Rotation");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Rot Z", &transform.rotation.z);
                    CheckFirstAndLastUseTransform(wasRotZActive, "InputFloat Rot Z", selectedEntity);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Rot Z", &transform.rotation.z, -Degree(gPI), Degree(gPI));
                    CheckFirstAndLastUseTransform(wasRotZSliderActive, "SliderFloat Rot Z", selectedEntity);

                    // Scale X
                    ImGui::Text("Scale");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Scale X", &transform.scale.x);
                    CheckFirstAndLastUseTransform(wasScaleXActive, "InputFloat Scale X", selectedEntity);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Scale X", &transform.scale.x, 1, IMGUI_MAX_SCALE);
                    CheckFirstAndLastUseTransform(wasScaleXSliderActive, "SliderFloat Scale X", selectedEntity);

                    // Scale Y
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Scale Y", &transform.scale.y);
                    CheckFirstAndLastUseTransform(wasScaleYActive, "InputFloat Scale Y", selectedEntity);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Scale Y", &transform.scale.y, 1, IMGUI_MAX_SCALE);
                    CheckFirstAndLastUseTransform(wasScaleYSliderActive, "SliderFloat Scale Y", selectedEntity);

                    ImGui::TreePop();
                }
            }
            if (gCoordinator->HasComponent<Sprite>(selectedEntity)) {
                std::string treeNodeLabel = "Sprite##" + std::to_string(selectedEntity);

                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    Sprite& sprite = gCoordinator->GetComponent<Sprite>(selectedEntity);
                    //Color
                    ImGui::Text("Color");
                    ImGui::ColorPicker4("Color Picker", &sprite.color.r);
                    auto am{ AssetManager::GetInstance() };
                    if (sprite.spriteAssetID && sprite.spriteAssetID != static_cast<AssetID>(-1)) {
                        auto texture{ am->GetAsset<SpriteManager>(sprite.spriteAssetID) };

                        ImTextureID texID = reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture->GetTexture()->GetTexHdl()));

                        // Original sprite sheet size
                        ImVec2 originalSize(static_cast<float>(texture->GetTexture()->GetWidth()), static_cast<float>(texture->GetTexture()->GetHeight()));

                        // Calculate aspect ratio
                        float aspectRatio = originalSize.x / originalSize.y;

                        // Adjust size while maintaining aspect ratio
                        ImVec2 newSize;
                        if (aspectRatio > 1.0f) {
                            newSize = ImVec2(200, 200 / aspectRatio);
                        }
                        else {
                            newSize = ImVec2(200 * aspectRatio, 200);
                        }

                        ImGui::Image(texID, newSize, { 0,1 }, { 1,0 });
                    }

                    // Highlight the drop area
                    ImGui::TreePop();
                }
                if (ImGui::BeginDragDropTarget()) {
                    //std::cout << "Began drag-drop target." << std::endl;

                    //if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Sound AssetBrowser")) {
                    //    //std::cout << "Accepted payload." << std::endl;
                    //    AssetID droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                    //    std::cout << droppedAid << std::endl;
                    //}
                    AssetID droppedAid{ static_cast<AssetID>(-1) };
                    if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Sprite AssetBrowser")) {
                        //std::cout << "Accepted payload." << std::endl;
                        droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                        //std::cout << droppedAid << std::endl;
                        auto& sprite = gCoordinator->GetComponent<Sprite>(selectedEntity);
                        sprite.spriteAssetID = droppedAid;
                        //SceneManager::GetInstance()->AddAsset(gCurrentScene, droppedAid);
                    }

                    ImGui::EndDragDropTarget();
                }

            }
            if (gCoordinator->HasComponent<Animation>(selectedEntity)) {
                if (ImGui::TreeNode("Animation")) {
                    Animation& anim = gCoordinator->GetComponent<Animation>(selectedEntity);
                    ImGui::Text("Speed per frame");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Speed", &anim.speed);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Speed", &anim.speed, 0, IMGUI_MAX_SPEED_ANIM);
                    auto am{ AssetManager::GetInstance() };
                    //if (anim.assetID && anim.assetID != static_cast<AssetID>(-1)) {
                    ImGui::Text("States:");
                    if (anim.states.size()>0){
                        for (size_t i{}; i < anim.states.size(); ++i) {
                            ImGui::Text(std::to_string(i).c_str());
                            auto texture{ SpriteManager::GetSprite(am->GetAsset<AnimationManager>(anim.states[i])[1].spriteID)->GetTexture()};
                            ImGui::SameLine();
                            if (anim.states.size() > 1) {
                                if (i != anim.states.size() - 1) {
                                    if (ImGui::Button(("Move Down##" + std::to_string(i)).c_str())) {
                                        std::swap(anim.states[i], anim.states[i + 1]);
                                    }
                                }
                                ImGui::SameLine();
                                if (i != 0) {
                                    if (ImGui::Button(("Move Up##" + std::to_string(i)).c_str())) {
                                        std::swap(anim.states[i], anim.states[i - 1]);
                                    }
                                }
                            }
                            ImGui::SameLine();
                            if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
                                anim.states.erase(anim.states.begin() + i);
                                if (anim.currState >= i && anim.states.size() > 0)
                                    anim.currState--;
                                if (gCoordinator->HasComponent<Sprite>(selectedEntity)) {
                                    Sprite& anima = gCoordinator->GetComponent<Sprite>(selectedEntity);
                                    anima.spriteID = InvalidAsset;
                                }
                            }

                            ImTextureID texID = reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(texture->GetTexHdl()));

                            // Original sprite sheet size
                            ImVec2 originalSize(static_cast<float>(texture->GetWidth()), static_cast<float>(texture->GetHeight()));

                            // Calculate aspect ratio
                            float aspectRatio = originalSize.x / originalSize.y;

                            // Adjust size while maintaining aspect ratio
                            ImVec2 newSize;
                            if (aspectRatio > 1.0f) {
                                newSize = ImVec2(200, 200 / aspectRatio);
                            }
                            else {
                                newSize = ImVec2(200 * aspectRatio, 200);
                            }

                            //std::cout << "x:" << newSize.x << "y:" << newSize.y << std::endl;
                            ImGui::Image(texID, newSize, { 0,1 }, { 1,0 });
                        }

                    }
                    else {
                        ImGui::Text("No Animation");
                        ImVec2 dummySize(150, 150);
                        ImGui::Dummy(dummySize);
                    }
                    ImGui::TreePop();
                }
                if (ImGui::BeginDragDropTarget()) {
                    //std::cout << "Began drag-drop target." << std::endl;

                    //if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Sound AssetBrowser")) {
                    //    //std::cout << "Accepted payload." << std::endl;
                    //    AssetID droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                    //    std::cout << droppedAid << std::endl;
                    //}
                    AssetID droppedAid{ static_cast<AssetID>(-1) };

                    if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Animation AssetBrowser")) {
                        //std::cout << "Accepted payload." << std::endl;
                        droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                        //std::cout << droppedAid << std::endl;
                        auto& anim = gCoordinator->GetComponent<Animation>(selectedEntity);
                        //anim.assetID = droppedAid;
                        anim.states.emplace_back(droppedAid);
                        //SceneManager::GetInstance()->AddAsset(gCurrentScene, droppedAid);
                    }
                    ImGui::EndDragDropTarget();
                }
            }
            if (gCoordinator->HasComponent<Collider>(selectedEntity)) {
                std::string treeNodeLabel = "Collider##" + std::to_string(selectedEntity);

                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    Collider& collider = gCoordinator->GetComponent<Collider>(selectedEntity);
                    ImGui::Text("Type");
                    const char* colliderTypes[]{ "BOX", "CIRCLE" };//box, circle;
                    ImGui::Combo("Collider Type", reinterpret_cast<int*>(&collider.type), colliderTypes, IM_ARRAYSIZE(colliderTypes));
                    //Pos
                    ImGui::Text("Position");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Collider Pos X", &collider.position.x);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Collider Pos X", &collider.position.x, -ENGINE_SCREEN_WIDTH, ENGINE_SCREEN_WIDTH);

                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Collider Pos Y", &collider.position.y);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Collider Pos Y", &collider.position.y, -ENGINE_SCREEN_HEIGHT, ENGINE_SCREEN_HEIGHT);
                    // Rotation
                    ImGui::Text("Rotation");
                    ImGui::SetNextItemWidth(50.f);
                    decltype(collider.rotation) crotDeg{Degree(collider.rotation)};
                    ImGui::InputFloat("##Collider Rot", &crotDeg);
                    collider.rotation = glm::radians(crotDeg);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Collider Rot", &collider.rotation, -Degree(gPI), Degree(gPI)); // change to Degree(gPI) same as glm func in math ultiles
                    // Scale
                    if (collider.type == ColliderType::BOX) {
                        ImGui::Text("Dimension");
                        ImGui::SetNextItemWidth(50.f);
                        ImGui::InputFloat("##Collider Scale X", &collider.dimension.x);
                        ImGui::SameLine();
                        ImGui::SetNextItemWidth(100.f);
                        ImGui::SliderFloat("Collider Scale X", &collider.dimension.x, 1, IMGUI_MAX_SCALE);

                        ImGui::SetNextItemWidth(50.f);
                        ImGui::InputFloat("##Collider Scale Y", &collider.dimension.y);
                        ImGui::SameLine();
                        ImGui::SetNextItemWidth(100.f);
                        ImGui::SliderFloat("Collider Scale Y", &collider.dimension.y, 1, IMGUI_MAX_SCALE);

                    }
                    else {
                        ImGui::Text("Diameter");
                        ImGui::SetNextItemWidth(50.f);
                        ImGui::InputFloat("##Collider Scale X", &collider.dimension.x);
                        ImGui::SameLine();
                        ImGui::SetNextItemWidth(100.f);
                        ImGui::SliderFloat("Collider Scale X", &collider.dimension.x, 1, IMGUI_MAX_SCALE);
                        collider.dimension.y = collider.dimension.x;
                    }
                    ImGui::TreePop();
                }
            }
            if (gCoordinator->HasComponent<RigidBody>(selectedEntity)) {
                std::string treeNodeLabel = "RigidBody##" + std::to_string(selectedEntity);

                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    RigidBody& rigidBody = gCoordinator->GetComponent<RigidBody>(selectedEntity);
                    // Mass
                    ImGui::Text("Mass");
                    ImGui::InputFloat("Mass", &rigidBody.mass);
                    rigidBody.SetMass(rigidBody.mass);
                    ImGui::SameLine();
                    bool isRbMassMax{std::fabs(rigidBody.mass - FLT_MAX) < FLT_EPSILON};
                    ImGui::Checkbox("##readonlymaxmassrgbd", reinterpret_cast<bool*>(&isRbMassMax));
                    if (isRbMassMax) rigidBody.SetMass(FLT_MAX);

                    // Velocity
                    ImGui::Text("Velocity");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Velocity X", &rigidBody.velocity.x);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Velocity X", &rigidBody.velocity.x, 0, IMGUI_MAX_VELOCITY);

                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Velocity Y", &rigidBody.velocity.y);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Velocity Y", &rigidBody.velocity.y, 0, IMGUI_MAX_VELOCITY);

                    //Friction
                    ImGui::Text("Friction");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Friction", &rigidBody.friction);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Friction", &rigidBody.friction, 0, 100);

                    const char* items[] = { "False", "True" };
                    int currentItem = rigidBody.isLockRotation ? 1 : 0; // Convert bool to int for selection

                    // The combo box
                    if (ImGui::Combo("Fixed Rotation", &currentItem, items, IM_ARRAYSIZE(items)))
                    {
                        rigidBody.isLockRotation = currentItem == 1; // Convert int back to bool
                    }

                    ImGui::TreePop();
                }
            }
            if (gCoordinator->HasComponent<Gravity>(selectedEntity)) {
                std::string treeNodeLabel = "Gravity##" + std::to_string(selectedEntity);
                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    Gravity& gravity = gCoordinator->GetComponent<Gravity>(selectedEntity);
                    //Force
                    ImGui::Text("Gravity");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Force X", &gravity.force.x);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Force X", &gravity.force.x, -IMGUI_MAX_GRAVITY, IMGUI_MAX_GRAVITY);

                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Force Y", &gravity.force.y);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Force Y", &gravity.force.y, -IMGUI_MAX_GRAVITY, IMGUI_MAX_GRAVITY);
                    ImGui::TreePop();
                }
            }
            if (gCoordinator->HasComponent<Text>(selectedEntity)) {
              std::string treeNodeLabel = "Text##" + std::to_string(selectedEntity);
              if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                Text& text = gCoordinator->GetComponent<Text>(selectedEntity);
                ImGui::Text("Text to Display");
                ImGui::SameLine();
                ImGui::SetCursorPosX(SAME_LINE_SPACING);
                ImGui::SetNextItemWidth(TEXT_BOX_WIDTH);
                char tempLayerName[256]; // Temp Buffer
                strncpy(tempLayerName, text.text.c_str(), sizeof(tempLayerName) - 1);
                tempLayerName[sizeof(tempLayerName) - 1] = '\0';

                if (ImGui::InputText(("##readonly" + std::to_string(selectedEntity)).c_str(), tempLayerName, sizeof(tempLayerName),
                  ImGuiInputTextFlags_EnterReturnsTrue)) {
                  text.text = tempLayerName;
                }

                ImGui::TreePop();
              }
            }
            if (gCoordinator->HasComponent<Script>(selectedEntity)) {
              std::string treeNodeLabel = "Script##" + std::to_string(selectedEntity);
              if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                  Script& script = gCoordinator->GetComponent<Script>(selectedEntity);
                  ImGui::Text("Assigned Script");
                  ImGui::Text(script.name.c_str());

                  static int selectedOption = -1;

                  // Find the initial index for selectedOption based on the name of the script
                  for (int i{}; i < ScriptManager::GetAssignableScriptNames().size(); ++i) {
                    if (script.name == ScriptManager::GetAssignableScriptNames()[i]) {
                      selectedOption = i;
                      break;
                    }
                  }

                  static int previousOption = selectedOption; // Store the previous option

                  ImGui::Combo("Name", 
                    &selectedOption, 
                    ScriptManager::GetAssignableScriptNames().data(), 
                    static_cast<int>(ScriptManager::GetAssignableScriptNames().size()));
                  
                  if (selectedOption != previousOption) {
                    previousOption = selectedOption;
                    script.name = ScriptManager::GetAssignableScriptNames()[selectedOption];
                    ScriptManager::OnCreateEntity(selectedEntity);
                  }
                                
                  ImGui::TreePop();
              }
            }
            ImGui::PopStyleColor(2);
        }
        ImGui::PopFont();

    }
    void PrefabInspectorWindow() {
        ImGui::Begin("Prefab Inspector");
        InspectorWindow(gSelectedPrefab);
        ImGui::End();
        
    }
    void GameObjectInspectorWindow() {
        ImGui::Begin("Game Object Inspector");
        InspectorWindow(gSelectedEntity);
        ImGui::End();
    }
    /*  _________________________________________________________________________ */
    /*! PropertyWindow

    @param none

    @return none.

    This function displays the properties of the selected entity and allows for
    adding or removing components.
    */
    void PropertyWindow(Entity selectedEntity, bool ignore) {
      ImGui::PushFont(mainfont);
        const char* components[] = { "Transform", "Sprite", "RigidBody", "Collider","Animation","Gravity","Tag", "Script", "UIImage", "Text", "Swappable" };
        static int selectedComponent{ -1 };
        //Entity selectedEntity{  (gSelectedPrefab == MAX_ENTITIES) ? gSelectedEntity : gSelectedPrefab };
        if (selectedEntity != MAX_ENTITIES) {
          ImGui::Text("Entity ID: %d", selectedEntity);
          if (gCoordinator->HasComponent<Tag>(selectedEntity)) {
            static Entity gPreviousEntity = { MAX_ENTITIES };
            static char tag[256] = "";
            if (gPreviousEntity != selectedEntity) {
              memset(tag, 0, sizeof(tag));
              gPreviousEntity = selectedEntity;
            }
            Tag& tagComponent = gCoordinator->GetComponent<Tag>(selectedEntity);
            ImGui::Text("Current Tag: %s", tagComponent.tag.c_str());
            //ImGui::SetNextItemWidth(50.0f);
            if (ImGui::InputText("Tag", tag, IM_ARRAYSIZE(tag), ImGuiInputTextFlags_EnterReturnsTrue)) {
              tagComponent.tag = tag;
              memset(tag, 0, sizeof(tag)); // Clear the input
            }
          }
          else {
            ImGui::Text("No Entity Tag");
          }

          //Combo box click to add components
          ImGui::Combo("Components", &selectedComponent, components, IM_ARRAYSIZE(components));
          if (ImGui::Button("Add")) {
            switch (selectedComponent) {
            case 0: {
              if (!gCoordinator->HasComponent<Transform>(selectedEntity)) {
                ImGuiViewport* vP = ImGui::GetWindowViewport();
                gCoordinator->AddComponent(
                  selectedEntity,
                  Transform{
                      {vP->Pos.x,vP->Pos.y,0},
                      {0,0,0},
                      {IMGUI_SCALE,IMGUI_SCALE,IMGUI_SCALE}
                  }, ignore);
              }
            }
                  break;
            case 1: {
              if (!gCoordinator->HasComponent<Sprite>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  Sprite{
                      {1,1,1, 1}
                  }, ignore);
              }
            }
                  break;
            case 2: {
              if (!gCoordinator->HasComponent<RigidBody>(selectedEntity)) {
                if (gCoordinator->HasComponent<Transform>(selectedEntity)) {
                  Transform& transform = gCoordinator->GetComponent<Transform>(selectedEntity);
                  gCoordinator->AddComponent(
                    selectedEntity,
                    RigidBody{
                        Vec2{transform.position.x,transform.position.y},
                        transform.rotation.z,
                        IMGUI_MASS,
                        Vec2{transform.scale.x,transform.scale.y}
                    }, ignore);
                }
                else {
                  ImGuiViewport* vP = ImGui::GetWindowViewport();
                  gCoordinator->AddComponent(
                    selectedEntity,
                    RigidBody{
                        Vec2{vP->Pos.x,vP->Pos.y},
                        0.f,
                        IMGUI_MASS,
                        Vec2{IMGUI_SCALE,IMGUI_SCALE}
                    }, ignore);
                }
              }
            }
                  break;
            case 3: {
              if (!gCoordinator->HasComponent<Collider>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  Collider{ Vec2{},0,Vec2{} }, ignore);
              }

            }
                  break;
            case 4: {
              if (!gCoordinator->HasComponent<Animation>(selectedEntity)) {
                ::gCoordinator->AddComponent(
                  selectedEntity,
                  Animation{
                      0.08f,
                      0,
                      0
                  }, ignore);
              }
            }
                  break;
            case 5: {
              if (!gCoordinator->HasComponent<Gravity>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  Gravity{ Vec2{0.f,-IMGUI_GRAVITY} }, ignore);
              }
            }
                  break;
            case 6: {
              if (!gCoordinator->HasComponent<Tag>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  Tag{ "Entity " + std::to_string(selectedEntity) }, ignore);
              }
            }
                  break;
            case 7: {

              if (!gCoordinator->HasComponent<Script>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  Script{ "No Script Assigned" }, ignore);
                ScriptManager::OnCreateEntity(selectedEntity);
              }
            }
                  break;
            case 8: {

              if (!gCoordinator->HasComponent<UIImage>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  UIImage{ true }, ignore);
              }
            }
                  break;

            case 9: {

              if (!gCoordinator->HasComponent<Text>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  Text{ "Arial", 0.05f,
                        "Your Text Here", Vec3(
                        1.0,
                        1.0,
                        1.0) }, ignore);
              }
            }
                  break;

            case 10: {

              if (!gCoordinator->HasComponent<Swappable>(selectedEntity)) {
                gCoordinator->AddComponent(
                  selectedEntity,
                  Swappable{}, ignore);
              }
            }
                   break;
            }
          }
          ImGui::SameLine();
          if (ImGui::Button("Remove")) {
            switch (selectedComponent) {
            case 0: {
              // Remove Transform component
              if (gCoordinator->HasComponent<Transform>(selectedEntity)) {
                gCoordinator->RemoveComponent<Transform>(selectedEntity);
              }
            }
                  break;
            case 1: {
              // Remove Sprite component
              if (gCoordinator->HasComponent<Sprite>(selectedEntity)) {
                gCoordinator->RemoveComponent<Sprite>(selectedEntity);
              }
            }
                  break;
            case 2: {
              // Remove RigidBody component
              if (gCoordinator->HasComponent<RigidBody>(selectedEntity)) {
                gCoordinator->RemoveComponent<RigidBody>(selectedEntity);
              }
            }
                  break;
            case 3: {
              // Remove Collider component
              if (gCoordinator->HasComponent<Collider>(selectedEntity)) {
                gCoordinator->RemoveComponent<Collider>(selectedEntity);
              }
            }
                  break;
            case 4: {
              // Remove Animation component
              if (gCoordinator->HasComponent<Animation>(selectedEntity)) {
                gCoordinator->RemoveComponent<Animation>(selectedEntity);
              }
            }
                  break;
            case 5: {
              // Remove Gravity component
              if (gCoordinator->HasComponent<Gravity>(selectedEntity)) {
                gCoordinator->RemoveComponent<Gravity>(selectedEntity);
              }
            }
                  break;
            case 6: {
              // Remove Tag component
              if (gCoordinator->HasComponent<Tag>(selectedEntity)) {
                gCoordinator->RemoveComponent<Tag>(selectedEntity);
              }
            }
                  break;
            case 7: {
              // Remove Script component
              if (gCoordinator->HasComponent<Script>(selectedEntity)) {
                gCoordinator->RemoveComponent<Script>(selectedEntity);
                Image::ScriptManager::RemoveEntity(selectedEntity);
              }
            }
                  break;
            case 8: {
              // Remove UIImage component
              if (gCoordinator->HasComponent<UIImage>(selectedEntity)) {
                gCoordinator->RemoveComponent<UIImage>(selectedEntity);
              }
            }
                  break;

            case 9: {
              // Remove UIImage component
              if (gCoordinator->HasComponent<Text>(selectedEntity)) {
                gCoordinator->RemoveComponent<Text>(selectedEntity);
              }
            }
                  break;
            case 10: {
              if (gCoordinator->HasComponent<Swappable>(selectedEntity)) {
                gCoordinator->RemoveComponent<Swappable>(selectedEntity);
              }
            }
                   break;
            }
          }
          ImGui::Separator();
          ImGui::Text("Tag Component: %s", gCoordinator->HasComponent<Tag>(selectedEntity) ? "True" : "False");
          ImGui::Text("Script Component: %s", gCoordinator->HasComponent<Script>(selectedEntity) ? "True" : "False");
          ImGui::Text("Transform Component: %s", gCoordinator->HasComponent<Transform>(selectedEntity) ? "True" : "False");
          ImGui::Text("Sprite Component: %s", gCoordinator->HasComponent<Sprite>(selectedEntity) ? "True" : "False");
          ImGui::Text("RigidBody Component: %s", gCoordinator->HasComponent<RigidBody>(selectedEntity) ? "True" : "False");
          ImGui::Text("Collsion Component: %s", gCoordinator->HasComponent<Collider>(selectedEntity) ? "True" : "False");
          ImGui::Text("Animation Component: %s", gCoordinator->HasComponent<Animation>(selectedEntity) ? "True" : "False");
          ImGui::Text("Gravity Component: %s", gCoordinator->HasComponent<Gravity>(selectedEntity) ? "True" : "False");
          ImGui::Text("UIImage Component: %s", gCoordinator->HasComponent<UIImage>(selectedEntity) ? "True" : "False");
          ImGui::Text("Text Component: %s", gCoordinator->HasComponent<Text>(selectedEntity) ? "True" : "False");
          ImGui::Text("Swappable Component: %s", gCoordinator->HasComponent<Swappable>(selectedEntity) ? "True" : "False");

          //ImGui::PopFont();
          //ImGui::End();

        }
        ImGui::PopFont();
        //ImGui::End();
    }
    void PrefabPropertyWindow() {
        ImGui::Begin("Prefab Property");
        PropertyWindow(gSelectedPrefab, false);
        ImGui::End();
    }
    void GameObjectPropertyWindow() {
        ImGui::Begin("Property");
        PropertyWindow(gSelectedEntity);
        ImGui::End();
    }
    /*  _________________________________________________________________________ */
    /*! BufferWindow

    @param none

    @return none.

     This function displays the game engine's framebuffer as well as getting the
     entity ID when mouse is hovered ad allows for picking
    */
    void BufferWindow(float dt) {
        auto inputSystem{ Coordinator::GetInstance()->GetSystem<InputSystem>() };
        auto mpos{ inputSystem->GetWorldMousePos() };

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 originalPadding = style.WindowPadding;
        style.WindowPadding = ImVec2(0.0f, 0.0f);

        ImGui::Begin("Image Game Engine");
        if (ImGui::IsWindowAppearing()) gSelectedPrefab = MAX_ENTITIES;
        ImGui::BeginChild("LevelEditor");
        auto const& framebuffer = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer(0);
        unsigned int texHdl = framebuffer->GetColorAttachmentID();
        auto renderSystem = gCoordinator->GetSystem<RenderSystem>();

        ImVec2 contentSize = ImGui::GetContentRegionAvail();

        { //tch: my part
            ImVec2 mousePos = {inputSystem->GetMousePos().first, inputSystem->GetMousePos().second };

            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;

            ImVec2 paddedTopLeft = ImVec2(windowPos.x + windowPadding.x, windowPos.y + windowPadding.y);
            ImVec2 paddedBottomRight = ImVec2(windowPos.x + windowSize.x - windowPadding.x, windowPos.y + windowSize.y - windowPadding.y);

            if (ImGui::IsMouseHoveringRect(paddedTopLeft, paddedBottomRight)) {
                Event event(Events::Window::INPUT);
                event.SetParam(Events::Window::Input::EDITOR_MOUSE_MOVE, EditorMousePosition(MousePosition(
                    static_cast<float>(mousePos.x - paddedTopLeft.x),
                    static_cast<float>((mousePos.y - paddedTopLeft.y))
                ), MousePosition(paddedBottomRight.x - paddedTopLeft.x, paddedBottomRight.y - paddedTopLeft.y))
                );
                gCoordinator->SendEvent(event);
            }
        }

        if ((mViewportDim.x != contentSize.x) || (mViewportDim.y != contentSize.y)) {
            framebuffer->Resize(static_cast<unsigned int>(contentSize.x), static_cast<unsigned int>(contentSize.y));

            mViewportDim = contentSize;
        }

        if (ImGui::IsWindowHovered() && renderSystem->IsEditorMode()) {

            //mouse picking part:
            ImVec2 viewportOffset = ImGui::GetCursorPos(); //tab bar included
            ImVec2 min = ImGui::GetWindowPos();
            min.x += viewportOffset.x;
            min.y += viewportOffset.y;
            ImVec2 max{ min.x + contentSize.x, min.y + contentSize.y };
            ImVec2 mousePos = ImGui::GetMousePos();

            mousePos.x -= min.x;
            mousePos.y -= min.y;
            Vec2 viewportSize = { max.x - min.x, max.y - min.y };
            mousePos.y = viewportSize.y - mousePos.y;

            int mouseX = static_cast<int>(mousePos.x);
            int mouseY = static_cast<int>(mousePos.y);

            if (ImGui::IsMouseClicked(0) ) {
                if (mouseX >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY >= 0 && mouseY < static_cast<int>(viewportSize.y)) {
                    framebuffer->Bind();
                    int pixelData = framebuffer->ReadPixel(1, mouseX, mouseY);
                    framebuffer->Unbind();
                    if (pixelData >= 0 && pixelData <= MAX_ENTITIES) {
                        gSelectedEntity = pixelData;
                    }
                }
            }
            // If dragging an entity and the mouse is moving
            //else if (draggedEntity != -1 && draggedEntity <= MAX_ENTITIES && ImGui::IsMouseDragging(0)) {

            //    //if (!gCoordinator->HasComponent<UIImage>(draggedEntity)) {
            //        ImVec2 currentMousePos = ImGui::GetMousePos();
            //        ImVec2 delta = {
            //            currentMousePos.x - lastMousePos.x,
            //            currentMousePos.y - lastMousePos.y
            //        };

            //        Camera cam;
            //        if (!gCoordinator->HasComponent<UIImage>(draggedEntity))
            //            cam = gCoordinator->GetComponent<Camera>(gCoordinator->GetSystem<RenderSystem>()->GetCamera());
            //        else
            //            cam = gCoordinator->GetComponent<Camera>(gCoordinator->GetSystem<RenderSystem>()->GetUICamera());

            //        glm::mat4 invViewProjMtx{ glm::inverse(cam.GetViewProjMtx()) };
            //        // Unproject the initial drag position
            //        glm::vec4 clipSpaceInitial = glm::vec4(2.0f * (lastMousePos.x / viewportSize.x) - 1.0f, 1.0f - 2.0f * (lastMousePos.y / viewportSize.y), 0.0f, 1.0f);
            //        glm::vec4 worldSpaceInitial = invViewProjMtx * clipSpaceInitial;
            //        worldSpaceInitial /= worldSpaceInitial.w;

            //        // Unproject the current mouse position
            //        glm::vec4 clipSpaceCurrent = glm::vec4(2.0f * (currentMousePos.x / viewportSize.x) - 1.0f, 1.0f - 2.0f * (currentMousePos.y / viewportSize.y), 0.0f, 1.0f);
            //        glm::vec4 worldSpaceCurrent = invViewProjMtx * clipSpaceCurrent;
            //        worldSpaceCurrent /= worldSpaceCurrent.w;

            //        // Calculate the world space delta
            //        glm::vec3 worldDelta = glm::vec3(worldSpaceCurrent - worldSpaceInitial);

            //        Transform& transform = gCoordinator->GetComponent<Transform>(draggedEntity);
            //        transform.position += worldDelta;

            //        if (gCoordinator->HasComponent<Collider>(draggedEntity)) {
            //            Collider& collider = gCoordinator->GetComponent<Collider>(draggedEntity);
            //            collider.position += {worldDelta.x, worldDelta.y};
            //        }

            //        lastMousePos = currentMousePos;
            //    }
            //   
            ////}
            //// If left mouse button is released, stop dragging
            //else if (ImGui::IsMouseReleased(0)) {
            //    draggedEntity = -1;
            //}
        }

        auto& camera = ::gCoordinator->GetComponent<Camera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera());
        auto& cameraUI = ::gCoordinator->GetComponent<Camera>(::gCoordinator->GetSystem<RenderSystem>()->GetUICamera());
        auto frameController = FrameRateController::GetInstance();
        if (ImGui::IsWindowFocused() && renderSystem->IsEditorMode()) {
          //std::cout << inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_W) << std::endl;
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_W)) {
                camera.mPos.y += CAMERA_MOVESPEED * dt;
                camera.SetPosition(camera.mPos);
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_S)) {
                camera.mPos.y -= CAMERA_MOVESPEED * dt;
                camera.SetPosition(camera.mPos);
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_A)) {
                camera.mPos.x -= CAMERA_MOVESPEED * dt;
                camera.SetPosition(camera.mPos);
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_D)) {
                camera.mPos.x += CAMERA_MOVESPEED * dt;
                camera.SetPosition(camera.mPos);
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_Q)) {
                camera.mRot -= CAMERA_ROTSPEED * dt;
                camera.SetRotation(camera.mRot);
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_E)) {
                camera.mRot += CAMERA_ROTSPEED * dt;
                camera.SetRotation(camera.mRot);
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_R)) {
                camera.mZoomLevel += CAMERA_ZOOMSPEED * dt;
                camera.ZoomIn();
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_F)) {
                camera.mZoomLevel -= CAMERA_ZOOMSPEED * dt;
                camera.ZoomOut();
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_X)) {
                ::gCoordinator->GetSystem<RenderSystem>()->ToggleDebugMode();
            }
            
        }
        /*if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_8)) {
            frameController->ScaleDeltaTime(0.5f);
        }*/

        //ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(contentSize.x, contentSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), mViewportDim, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        if (gSelectedEntity != MAX_ENTITIES && renderSystem->IsEditorMode() && gCurrentGuizmoOperation!=-1) {
          ImGuizmo::SetOrthographic(true);
          ImGuizmo::SetDrawlist();
          ImVec2 windowPos = ImGui::GetWindowPos();

          float windowWidth = ImGui::GetWindowWidth();
          float windowHeight = ImGui::GetWindowHeight();
          //std::cout << "window posx :" << windowPos.x << " y: " << windowPos.y << std::endl;
          //std::cout << "window width :" << windowWidth << " height: " << windowHeight << std::endl;
          ImGuizmo::SetRect(windowPos.x, windowPos.y, windowWidth, windowHeight);
          glm::mat4 cameraProj{};
          glm::mat4 cameraView{};//or view mtx
          if (gCoordinator->HasComponent<UIImage>(gSelectedEntity)) {
              cameraProj = cameraUI.GetProjMtx();
              cameraView = cameraUI.GetViewMtx();
          }
          else {
              cameraProj = camera.GetProjMtx();
              cameraView = camera.GetViewMtx();
          }
            if (gCoordinator->HasComponent< Transform>(gSelectedEntity)) {
              Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
              // Create a transformation matrix from position, rotation, and scale
              glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), transform.position);
              glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0, 0, 1));
              glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), transform.scale);
              glm::mat4 transformMatrix = translationMatrix * rotationMatrix * scaleMatrix;
              float snapArr[3] = { gSnapVal,gSnapVal ,gSnapVal };

              ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProj),
                  gCurrentGuizmoOperation, gCurrentGizmoMode,
                  glm::value_ptr(transformMatrix), nullptr,
                  gSnap ? snapArr : nullptr);
              
              static bool wasUsingGuizmo = false;
              bool isUsingGuizmo = ImGuizmo::IsUsing();
              if (isUsingGuizmo && !wasUsingGuizmo) {
                  // This is the frame where ImGuizmo started being used
                  std::cout << "guizmo start\n";
                  if (gCoordinator->HasComponent<Collider>(gSelectedEntity)) {
                      Collider& collider = gCoordinator->GetComponent<Collider>(gSelectedEntity);
                      CommandManager::GetInstance()->AddCommand("Transform", gSelectedEntity, Transform{ transform }, Collider{collider});
                  }
                  else {
                      CommandManager::GetInstance()->AddCommand("Transform", gSelectedEntity, Transform{ transform });
                  }
              }

              if (!isUsingGuizmo && wasUsingGuizmo) {
                  // This is the frame where ImGuizmo stopped being used
                  std::cout << "guizmo end\n";
              }

              if (ImGuizmo::IsUsing()) {
                  glm::vec3 position, rotation, scale;
                  Image::DecomposeTransform(transformMatrix, position, rotation, scale);
                  glm::vec3 deltaPosition = position - transform.position;
                  //float deltaRotationZ = glm::degrees(rotation.z) - transform.rotation.z;
                  glm::vec3 deltaScale = scale - transform.scale;
                  transform.position = position;
                  transform.rotation.z = glm::degrees(rotation.z);
                  transform.scale = scale;
                  // Update collider position if the entity has a collider component
                  if (gCoordinator->HasComponent<Collider>(gSelectedEntity)) {
                      Collider& collider = gCoordinator->GetComponent<Collider>(gSelectedEntity);
                      collider.position.x += deltaPosition.x ;
                      collider.position.y += deltaPosition.y;
                      collider.rotation = rotation.z;
                      collider.dimension.x += deltaScale.x ;
                      collider.dimension.y += deltaScale.y;
                  }
              }
              wasUsingGuizmo = isUsingGuizmo;
          }
        }
        ImGui::EndChild();
        //std::cout << "Mouse 1: " << mpos.first << ", Mouse 2:" << mpos.second << std::endl;

        //tch: for scene to drag drop
        if (ImGui::BeginDragDropTarget()) {
            //std::cout << "Began drag-drop target." << std::endl;

            if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("SceneBrowser")) {

                const wchar_t* payLoadPath = (const wchar_t*)dragDropPayLoad->Data;
                std::filesystem::path basePath {""};
                //std::cout  << (basePath / payLoadPath).stem().string() << std::endl;
                //if (gCurrentScene != "") {
                //    SceneManager::GetInstance()->ExitScene(gCurrentScene);
                //}
                std::string currentScene = (basePath / payLoadPath).stem().string();

                SceneManager::GetInstance()->LoadScene(currentScene);
            }
            if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("PrefabsInstance")) {
                if (SceneManager::GetInstance()->IsSceneActive()) {
                    PrefabsManager::PrefabID droppedPid = *(const PrefabsManager::PrefabID*)dragDropPayLoad->Data;

                    PrefabsManager::GetInstance()->SpawnPrefab(droppedPid, { mpos.first, mpos.second });
                    //std::cout << "Mouse X: " << mpos.first << ", Mouse Y:-----------------------------------------------" << mpos.second << std::endl;

                }


            }
            ImGui::EndDragDropTarget();
        }

        ImGui::End();
    }

    /*  _________________________________________________________________________ */
    /*! PrefabWindow

    @param none.

    @return none.

    This function displays the prefab framebuffer window.
    */
    void PrefabWindow() {
        ImGui::PushFont(mainfont);
        static ImVec2 prefabVp {};
        ImGui::Begin("Prefab Editor");

        auto const& framebuffer = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer(1);
        unsigned int texHdl = framebuffer->GetColorAttachmentID();

        ImVec2 contentSize = ImGui::GetContentRegionAvail();

        if ((prefabVp.x != contentSize.x) || (prefabVp.y != contentSize.y)) {
            framebuffer->Resize(static_cast<unsigned int>(contentSize.x), static_cast<unsigned int>(contentSize.y));
            prefabVp = contentSize;
        }
        Entity selectedPrefab = GetSelectedPrefabEntryEntity();
        gSelectedPrefab = selectedPrefab; // tch: this line gives me heebie jeebies.... im not sure why
        if (selectedPrefab != MAX_ENTITIES)
         gCoordinator->GetSystem<RenderSystem>()->RenderPrefab(selectedPrefab);

        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(ENGINE_SCREEN_WIDTH / gScalingFactor, ENGINE_SCREEN_HEIGHT / gScalingFactor), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::PopFont();
        ImGui::End();
    }

    /*  _________________________________________________________________________ */
    /*! TextureHdlWindow

    @param none.

    @return none.

    This function displays all the assets and allows for dragging of the assets.
    */
    void ContentWindow() {
        ImGui::PushFont(mainfont);
        ImGui::Begin("Content Browser");
        static std::shared_ptr<Texture> directroyIcon = Texture::Create("../Icon/DirectoryIcon.png");
        static std::shared_ptr<Texture> fileIcon = Texture::Create("../Icon/FileIcon.png");
        static std::filesystem::path assetDirectory{ "../assets" };
        static std::filesystem::path currentDirectory{ assetDirectory };
        if (currentDirectory != std::filesystem::path(assetDirectory)) {
            if (ImGui::Button("Back")) {
                currentDirectory = currentDirectory.parent_path();
            }

        }
        static float padding = 15.f;
        static float size = 125.f;
        static float iconSize = padding + size;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = static_cast<int>(panelWidth / iconSize);
        if (columnCount < 1) {
            columnCount = 1;
        }
        ImGui::Columns(columnCount, 0, false);
        for (auto& directoryPath : std::filesystem::directory_iterator(currentDirectory)) {
            auto const& path = directoryPath.path();
            auto relativePath = std::filesystem::relative(path, assetDirectory);
            std::string filenameString = relativePath.filename().string();
            auto fileName = directoryPath.path().filename().string();
            ImGui::PushID(filenameString.c_str());
            std::shared_ptr<Texture> icon = directoryPath.is_directory() ? directroyIcon : fileIcon;
            ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
            ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(icon->GetTexHdl())), { size, size }, { 0, 1 }, { 1, 0 });
            ImGui::PopStyleColor();
            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("Content Asset Browser", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::Text("%s", filenameString.c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                if (directoryPath.is_directory()) {
                    currentDirectory /= path.filename();

                }
            }
            ImGui::TextWrapped(filenameString.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::Columns(1);// go back to default
        ImGui::PopFont();
        ImGui::End();
    }



    /*  _________________________________________________________________________ */
    /*! PerformanceWindow

    @param none

    @return none.

    This function displays the fps, total entity and performance of the 
    various systems
    */
    void PerformanceWindow() {
        ImGui::PushFont(mainfont);
        ImGui::Begin("Performance Viewer");
        auto frameController = FrameRateController::GetInstance();
        static float fpsValues[gPercent] = {};
        static float physicsValues[gPercent] = {};
        static float collisionValues[gPercent] = {};
        static float renderValues[gPercent] = {};
        static int valueIndex{};
        valueIndex = (valueIndex + 1) % gPercent;

        // Display FPS
        float fps = frameController->GetFps();
        ImGui::Text("FPS: %.2f", fps);
        ImGui::Text("FPS Graph");
        fpsValues[valueIndex] = fps;
        ImGui::PlotLines("FPS", fpsValues, IM_ARRAYSIZE(fpsValues), valueIndex, nullptr, 0.0f, fps, ImVec2(0, gPercent));
        ImGui::Separator();

        // Display Entity Count
        int entityRatio = gCoordinator->GetEntityCount() / MAX_ENTITIES;
        ImGui::Text("Entities Count");
        ImGui::ProgressBar(static_cast<float>(entityRatio), ImVec2(-1.0f, 0.0f), std::to_string(gCoordinator->GetEntityCount()).c_str());
        ImGui::Separator();

        // Display Physics Performance
        ImGui::Text("Physics Performance");
        physicsValues[valueIndex] = frameController->GetProfilerValue(ENGINE_PHYSICS_PROFILE) * static_cast<float>(gPercent);
        float physicsPerformance = frameController->GetProfilerValue(ENGINE_PHYSICS_PROFILE);
        ImGui::ProgressBar(physicsPerformance, ImVec2(-1.0f, 0.0f), (std::to_string(physicsPerformance * static_cast<float>(gPercent)) + "%").c_str());
        ImGui::Text("Physics Performance Graph");
        ImGui::PlotLines("Physics", physicsValues, IM_ARRAYSIZE(physicsValues), valueIndex, nullptr, 0.0f, static_cast<float>(gPercent), ImVec2(0, gPercent));
        ImGui::Separator();

        // Display Collision Performance 
        ImGui::Text("Collision Performance");
        collisionValues[valueIndex] = frameController->GetProfilerValue(ENGINE_COLLISION_PROFILE) * static_cast<float>(gPercent);
        float collisionPerformance = frameController->GetProfilerValue(ENGINE_COLLISION_PROFILE);
        ImGui::ProgressBar(collisionPerformance, ImVec2(-1.0f, 0.0f), (std::to_string(collisionPerformance * static_cast<float>(gPercent)) + "%").c_str());
        ImGui::Text("Collision Performance Graph");
        ImGui::PlotLines("Collision", collisionValues, IM_ARRAYSIZE(collisionValues), valueIndex, nullptr, 0.0f, static_cast<float>(gPercent), ImVec2(0, gPercent));
        ImGui::Separator();

        // Display Render Performance
        ImGui::Text("Render Performance");
        renderValues[valueIndex] = frameController->GetProfilerValue(ENGINE_RENDER_PROFILE) * static_cast<float>(gPercent);
        float renderPerformance = frameController->GetProfilerValue(ENGINE_RENDER_PROFILE);
        ImGui::ProgressBar(renderPerformance, ImVec2(-1.0f, 0.0f), (std::to_string(renderPerformance * static_cast<float>(gPercent)) + "%").c_str());
        ImGui::Text("Render Performance Graph");
        ImGui::PlotLines("Render", renderValues, IM_ARRAYSIZE(renderValues), valueIndex, nullptr, 0.0f, static_cast<float>(gPercent), ImVec2(0, gPercent));
        ImGui::Separator();
        ImGui::PopFont();
        ImGui::End();
    }
    /*  _________________________________________________________________________ */
    /*! LoggingWindow

    @param none

    @return none.

    This function displays logs from the LoggingSystem. It will auto scroll to the
    bottom.
    */
    void LoggingWindow() {
        ImGui::PushFont(mainfont);
        ImGui::Begin("Log");
        // Retrieve logs from LoggingSystem
        LoggingSystem& logger = LoggingSystem::GetInstance();
        const auto& logs1 = logger.GetLogsBuffer1();
        const auto& logs2 = logger.GetLogsBuffer2();
        if (ImGui::BeginChild("##log", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar)) {
            for (const auto& log : logs1) {
                ImGui::TextUnformatted(log.c_str());
            }
            for (const auto& log : logs2) {
                ImGui::TextUnformatted(log.c_str());
            }
            if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
        }
        ImGui::PopFont();
        ImGui::EndChild();
        ImGui::End();
    }


    /*  _________________________________________________________________________ */
    /*!
    \brief RenderStatsWindow

    Creates an ImGui window displaying rendering statistics, such as draw calls, 
    quads, vertices, and indices, and then resets the stats for the next frame.
    */
    void RenderStatsWindow() {
        ImGui::PushFont(mainfont);
        ImGui::Begin("Renderer Stats");
        auto stats = Renderer::GetStats();
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVtxCount());
        ImGui::Text("Indices: %d", stats.GetTotalIdxCount());
        ImGui::PopFont();
        ImGui::End();
        Renderer::ResetStats();
    }

    void GuizmoWindow() {
        ImGui::PushFont(mainfont);
        ImGui::Begin("Guizmo");

        if (ImGui::RadioButton("Translate", gCurrentGuizmoOperation == ImGuizmo::TRANSLATE)) {
            gCurrentGuizmoOperation = ImGuizmo::TRANSLATE;

        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", gCurrentGuizmoOperation == ImGuizmo::ROTATE)) {
            gCurrentGuizmoOperation = ImGuizmo::ROTATE;

        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", gCurrentGuizmoOperation == ImGuizmo::SCALE)) {
            gCurrentGuizmoOperation = ImGuizmo::SCALE;
        }
        if (gSelectedEntity != MAX_ENTITIES) {
            if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
                float matrixTranslation[3]{ transform.position.x,transform.position.y,transform.position.z },
                    matrixRotation[3]{ transform.rotation.x,transform.rotation.y,transform.rotation.z },
                    matrixScale[3]{ transform.scale.x,transform.scale.y,transform.scale.z };

                ImGui::InputFloat3("Tr", matrixTranslation);
                ImGui::InputFloat3("Rt", matrixRotation);
                ImGui::InputFloat3("Sc", matrixScale);
            }
        }
        if (gCurrentGuizmoOperation != ImGuizmo::SCALE){
            if (ImGui::RadioButton("Local", gCurrentGizmoMode == ImGuizmo::LOCAL))
                gCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", gCurrentGizmoMode == ImGuizmo::WORLD))
                gCurrentGizmoMode = ImGuizmo::WORLD;
        }
        ImGui::Checkbox("Enable Snapping", &gSnap);
        if (gSnap) {
            if (gCurrentGuizmoOperation == ImGuizmo::TRANSLATE) {
                ImGui::InputFloat("Snap", &gSnapVal);
            }
            else if (gCurrentGuizmoOperation == ImGuizmo::ROTATE) {
                ImGui::InputFloat("Angle Snap", &gSnapVal);
            }
            else if (gCurrentGuizmoOperation == ImGuizmo::SCALE) {
                ImGui::InputFloat("Scale Snap", &gSnapVal);
            }
        }
        ImGui::PopFont();
        ImGui::End();

    }
    
}