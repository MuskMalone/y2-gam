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

#include "Components/Script.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Logging/LoggingSystem.hpp"
#include <Core/Globals.hpp>
#include <Core/FrameRateController.hpp>
#include "Graphics/Renderer.hpp"
#include "Scripting/NodeManager.hpp"
#include <filesystem>

#include <Engine/AssetManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Audio/Sound.hpp>
#include <IMGUI/AssetBrowser.hpp>
const int   gPercent      = 100;
const float gScalingFactor = 1.5f;

Entity gSelectedEntity=MAX_ENTITIES;
namespace {
    std::shared_ptr<Coordinator> gCoordinator;
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

        static bool toDelete{ false };

        if (ImGui::IsKeyReleased(ImGuiKey_C)) {
            NodeManager::ClearAllNodes();
            toDelete = !toDelete;
        }

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        //ImGui::ShowDemoWindow();
        MainMenuWindow();
        PerformanceWindow();
        HierarchyWindow(mEntities);
        InspectorWindow();
        PropertyWindow();
        //PrefabWindow();
        BufferWindow(dt);
        ContentWindow();
        //AssetWindow(mEntities);
        AnimationAssetWindow(mEntities);
        SpriteAssetWindow(mEntities);
        SoundAssetWindow(mEntities);
        AssetPropertiesWindow(mEntities);

        LoggingWindow();
        RenderStatsWindow();
        if (toDelete) {
            std::vector<Entity> deleteEntites{};
            for (auto& e : mEntities) {
                deleteEntites.emplace_back(e);
            }
            for (auto const& e : deleteEntites) {
                //TEMP TO BE DELETED
                //TODO 
                if (!gCoordinator->HasComponent<Script>(e) && std::to_string(e) != "2" && std::to_string(e) != "3" && std::to_string(e) != "4") {
                    gCoordinator->DestroyEntity(e);
                }
            }
            gSelectedEntity = MAX_ENTITIES;
            toDelete = !toDelete;

        }
    }
    /*  _________________________________________________________________________ */
    /*! MainMenuWindow

    @param none

    @return none.

    Displaying the menu bar.
    */
    void MainMenuWindow() {
        if (ImGui::BeginMainMenuBar())
        {
            //Read files maybe
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Cltr+N")) {

                }
                if (ImGui::MenuItem("Open...", "Ctrl+O")) {

                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {

                }
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {

                }
                ImGui::EndMenu();
            }

            //EDIT
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
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
                if (renderSystem->IsEditorMode()) {
                   // std::cout << "Play to toggle to editer play mode" << std::endl;
                    renderSystem->ToggleEditorMode();
                    ImGui::SetWindowFocus("Image Game Engine");
                }
            }
            if (ImGui::MenuItem("Stop")) {

                if (!renderSystem->IsEditorMode()) {
                    //std::cout << "Stop to toggle to editer mode" << std::endl;
                    renderSystem->ToggleEditorMode();
                    ImGui::SetWindowFocus("Image Game Engine");

                }
            }
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
        // Hierarchy Panel
        ImGui::Begin("Hierarchy");
        //Create entity and destory first
        if (ImGui::BeginPopupContextWindow("Hierarchy Context Menu", ImGuiPopupFlags_MouseButtonRight)) {
            if (ImGui::MenuItem("Create Entity")) {
                Entity newEntity = gCoordinator->CreateEntity();
                gSelectedEntity = newEntity;
                ImGuiViewport* vP = ImGui::GetWindowViewport();
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
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Create Entity")) {
            Entity newEntity = gCoordinator->CreateEntity();
            gSelectedEntity = newEntity;
            ImGuiViewport* vP = ImGui::GetWindowViewport();
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
        }

        //Cant destroy player
        if (gSelectedEntity != MAX_ENTITIES && ImGui::Button("Destroy Entity")) {
            if (!gCoordinator->HasComponent<Script>(gSelectedEntity)) {
                gCoordinator->DestroyEntity(gSelectedEntity);
                gSelectedEntity = MAX_ENTITIES;
            }
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
                if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Sprite AssetBrowser")) {
                    //std::cout << "Accepted payload." << std::endl;
                    AssetID droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                    //std::cout << droppedAid << std::endl;
                    if (gCoordinator->HasComponent<Sprite>(entity)) {
                        auto& sprite = gCoordinator->GetComponent<Sprite>(entity);
                        sprite.spriteAssetID = droppedAid;
                    }
                    else {
                        Sprite s{{1,1,1, 1}};
                        s.spriteAssetID = droppedAid;
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            s);
                    }

                }
                if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Animation AssetBrowser")) {
                    //std::cout << "Accepted payload." << std::endl;
                    AssetID droppedAid = *(const AssetID*)dragDropPayLoad->Data;
                    //std::cout << droppedAid << std::endl;
                    if (gCoordinator->HasComponent<Animation>(entity)) {
                        auto& anim = gCoordinator->GetComponent<Animation>(entity);
                        anim.assetID = droppedAid;
                    }
                    else {
                        Animation a{
                                0.08f,
                                0,
                                ANIM_STATE::IDLE
                        };
                        a.assetID = droppedAid;
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            a);
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
        
        //Cant delete stuff with spcript
        auto input = gCoordinator->GetSystem<InputSystem>();
        if (input->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_DELETE)) {
            if (gSelectedEntity != MAX_ENTITIES) {
                if (!gCoordinator->HasComponent<Script>(gSelectedEntity)) {
                    gCoordinator->DestroyEntity(gSelectedEntity);
                    gSelectedEntity = MAX_ENTITIES;
                }
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
        ImGui::End();
    }
    /*  _________________________________________________________________________ */
    /*! InspectorWindow

    @param none

    @return none.

    This function displays the components of the selected entity and allows for
    modification to its components.
    */
    void InspectorWindow() {
        // Inspector Panel
        ImGui::Begin("Inspector");
        //TransformComponent
        if (gSelectedEntity != MAX_ENTITIES) {
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Red
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Green
            if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                std::string treeNodeLabel = "Transform##" + std::to_string(gSelectedEntity);
              
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
                if (ImGui::TreeNodeEx(treeNodeLabel.c_str(), flags)) {
                    Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
                    // Position
                    ImGui::Text("Position");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Pos X", &transform.position.x);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Pos X", &transform.position.x, -ENGINE_SCREEN_WIDTH, ENGINE_SCREEN_WIDTH);

                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Pos Y", &transform.position.y);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Pos Y", &transform.position.y, -ENGINE_SCREEN_HEIGHT, ENGINE_SCREEN_HEIGHT);

                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Pos Z", &transform.position.z);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Pos Z", &transform.position.z, -ENGINE_SCREEN_HEIGHT, ENGINE_SCREEN_HEIGHT);

                    // Rotation
                    ImGui::Text("Rotation");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Rot Z", &transform.rotation.z);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Rot Z", &transform.rotation.z, -Degree(gPI), Degree(gPI)); // change to Degree(gPI) same as glm func in math ultiles

                    // Scale
                    ImGui::Text("Scale");
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Scale X", &transform.scale.x);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Scale X", &transform.scale.x, 1, IMGUI_MAX_SCALE);
                    ImGui::SetNextItemWidth(50.f);
                    ImGui::InputFloat("##Scale Y", &transform.scale.y);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(100.f);
                    ImGui::SliderFloat("Scale Y", &transform.scale.y, 1, IMGUI_MAX_SCALE);
                    ImGui::TreePop();
                }
            }
            if (gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                std::string treeNodeLabel = "Sprite##" + std::to_string(gSelectedEntity);

                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    Sprite& sprite = gCoordinator->GetComponent<Sprite>(gSelectedEntity);
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
            }
            if (gCoordinator->HasComponent<Animation>(gSelectedEntity)) {
                if (ImGui::TreeNode("Animation")) {
                    Animation& anim = gCoordinator->GetComponent<Animation>(gSelectedEntity);
                    auto am{ AssetManager::GetInstance() };
                    if (anim.assetID && anim.assetID != static_cast<AssetID>(-1)) {
                        auto texture{ SpriteManager::GetSprite(am->GetAsset<AnimationManager>(anim.assetID)[1].spriteID)->GetTexture()};

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
                    else {
                        ImGui::Text("No Animation");
                        ImVec2 dummySize(150, 150);
                        ImGui::Dummy(dummySize);
                    }
                    ImGui::TreePop();
                }
            }
            if (gCoordinator->HasComponent<Collider>(gSelectedEntity)) {
                std::string treeNodeLabel = "Collider##" + std::to_string(gSelectedEntity);

                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    Collider& collider = gCoordinator->GetComponent<Collider>(gSelectedEntity);
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
                    ImGui::InputFloat("##Collider Rot", &collider.rotation);
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
            if (gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                std::string treeNodeLabel = "RigidBody##" + std::to_string(gSelectedEntity);

                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    RigidBody& rigidBody = gCoordinator->GetComponent<RigidBody>(gSelectedEntity);
                    // Mass
                    ImGui::Text("Mass");
                    ImGui::InputFloat("Mass", &rigidBody.mass);
                    rigidBody.SetMass(rigidBody.mass);

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
                    if (ImGui::Combo("SoundStream", &currentItem, items, IM_ARRAYSIZE(items)))
                    {
                        rigidBody.isLockRotation = currentItem == 1; // Convert int back to bool
                    }

                    ImGui::TreePop();
                }
            }
            if (gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                std::string treeNodeLabel = "Gravity##" + std::to_string(gSelectedEntity);
                if (ImGui::TreeNode(treeNodeLabel.c_str())) {
                    Gravity& gravity = gCoordinator->GetComponent<Gravity>(gSelectedEntity);
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
            ImGui::PopStyleColor(2);
        }
        ImGui::End();
    }

    /*  _________________________________________________________________________ */
    /*! PropertyWindow

    @param none

    @return none.

    This function displays the properties of the selected entity and allows for
    adding or removing components.
    */
    void PropertyWindow() {
        ImGui::Begin("Property");
        const char* components[] = { "Transform", "Sprite", "RigidBody", "Collision","Animation","Gravity","Tag" };
        static int selectedComponent{ -1 };
        if (gSelectedEntity != MAX_ENTITIES) {
            ImGui::Text("Entity ID: %d", gSelectedEntity);
            if (gCoordinator->HasComponent<Tag>(gSelectedEntity)) {
                static Entity gPreviousEntity = { MAX_ENTITIES };
                static char tag[256] = "";
                if (gPreviousEntity != gSelectedEntity) {
                    memset(tag, 0, sizeof(tag));
                    gPreviousEntity = gSelectedEntity;
                }
                Tag& tagComponent = gCoordinator->GetComponent<Tag>(gSelectedEntity);
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
                    if (!gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                        ImGuiViewport* vP = ImGui::GetWindowViewport();
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Transform{
                                {vP->Pos.x,vP->Pos.y,0},
                                {0,0,0},
                                {IMGUI_SCALE,IMGUI_SCALE,IMGUI_SCALE}
                            });
                    }
                }
                      break;
                case 1: {
                    if (!gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Sprite{
                                {1,1,1, 1}
                            });
                    }
                }
                      break;
                case 2: {
                    if (!gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                        if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                            Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
                            gCoordinator->AddComponent(
                                gSelectedEntity,
                                RigidBody{
                                    Vec2{transform.position.x,transform.position.y},
                                    transform.rotation.z,
                                    IMGUI_MASS,
                                    Vec2{transform.scale.x,transform.scale.y}
                                });
                        }
                        else {
                            ImGuiViewport* vP = ImGui::GetWindowViewport();
                            gCoordinator->AddComponent(
                                gSelectedEntity,
                                RigidBody{
                                    Vec2{vP->Pos.x,vP->Pos.y},
                                    0.f,
                                    IMGUI_MASS,
                                    Vec2{IMGUI_SCALE,IMGUI_SCALE}
                                });
                        }
                    }
                }
                      break;
                case 3: {
                    if (!gCoordinator->HasComponent<Collider>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Collider{ Vec2{},0,Vec2{} });
                    }

                }
                      break;
                case 4: {
                    if (!gCoordinator->HasComponent<Animation>(gSelectedEntity)) {
                        ::gCoordinator->AddComponent(
                            gSelectedEntity,
                            Animation{
                                0.08f,
                                0,
                                ANIM_STATE::IDLE
                            });
                    }
                }
                      break;
                case 5: {
                    if (!gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Gravity{ Vec2{0.f,-IMGUI_GRAVITY} });
                    }
                }
                      break;
                case 6: {
                    if (!gCoordinator->HasComponent<Tag>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Tag{ "Entity " + std::to_string(gSelectedEntity) });
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
                    if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Transform>(gSelectedEntity);
                    }
                }
                      break;
                case 1: {
                    // Remove Sprite component
                    if (gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Sprite>(gSelectedEntity);
                    }
                }
                      break;
                case 2: {
                    // Remove RigidBody component
                    if (gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<RigidBody>(gSelectedEntity);
                    }
                }
                      break;
                case 3: {
                    // Remove Collider component
                    if (gCoordinator->HasComponent<Collider>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Collider>(gSelectedEntity);
                    }
                }
                      break;
                case 4: {
                    // Remove Animation component
                    if (gCoordinator->HasComponent<Animation>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Animation>(gSelectedEntity);
                    }
                }
                      break;
                case 5: {
                    // Remove Gravity component
                    if (gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Gravity>(gSelectedEntity);
                    }
                }
                      break;
                case 6: {
                    // Remove Tag component
                    if (gCoordinator->HasComponent<Tag>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Tag>(gSelectedEntity);
                    }
                }
                      break;
                }
            }
            ImGui::Separator();
            ImGui::Text("Tag Component: %s", gCoordinator->HasComponent<Tag>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Transform Component: %s", gCoordinator->HasComponent<Transform>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Sprite Component: %s", gCoordinator->HasComponent<Sprite>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("RigidBody Component: %s", gCoordinator->HasComponent<RigidBody>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Collsion Component: %s", gCoordinator->HasComponent<Collider>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Animation Component: %s", gCoordinator->HasComponent<Animation>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Gravity Component: %s", gCoordinator->HasComponent<Gravity>(gSelectedEntity) ? "True" : "False");

        }
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
        static int draggedEntity = -1;   // -1 means no entity is being dragged.
        static ImVec2 lastMousePos;      // Store the last position of the mouse.
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 originalPadding = style.WindowPadding;
        style.WindowPadding = ImVec2(0.0f, 0.0f);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

        ImGui::Begin("Image Game Engine", nullptr, window_flags);
        auto const& framebuffer = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer(0);
        unsigned int texHdl = framebuffer->GetColorAttachmentID();
        auto renderSystem = gCoordinator->GetSystem<RenderSystem>();

            ImVec2 contentSize = ImGui::GetContentRegionAvail();
        if (ImGui::IsWindowHovered()&&renderSystem->IsEditorMode()) {

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
            int fbX = static_cast<int>(mouseX * gScalingFactor);
            int fbY = static_cast<int>(mouseY * gScalingFactor);

            
            if (ImGui::IsMouseClicked(0) && draggedEntity == -1) {
                if (mouseX >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY >= 0 && mouseY < static_cast<int>(viewportSize.y)) {
                    framebuffer->Bind();
                    int pixelData = framebuffer->ReadPixel(1, fbX, fbY);
                    framebuffer->Unbind();
                    draggedEntity = pixelData;
                    if (pixelData >= 0 && pixelData <= MAX_ENTITIES) {
                        gSelectedEntity = pixelData;
                    }
                    lastMousePos = ImGui::GetMousePos();
                }
            }
            // If dragging an entity and the mouse is moving
            else if (draggedEntity != -1 && draggedEntity <= MAX_ENTITIES && ImGui::IsMouseDragging(0)) {

                ImVec2 currentMousePos = ImGui::GetMousePos();
                ImVec2 delta = {
                    currentMousePos.x - lastMousePos.x,
                    currentMousePos.y - lastMousePos.y
                };
                Camera const& cam{ gCoordinator->GetComponent<Camera>(gCoordinator->GetSystem<RenderSystem>()->GetCamera()) };

                glm::mat4 invViewProjMtx{ glm::inverse(cam.GetViewProjMtx()) };
                // Unproject the initial drag position
                glm::vec4 clipSpaceInitial = glm::vec4(2.0f * (lastMousePos.x / viewportSize.x) - 1.0f, 1.0f - 2.0f * (lastMousePos.y / viewportSize.y), 0.0f, 1.0f);
                glm::vec4 worldSpaceInitial = invViewProjMtx * clipSpaceInitial;
                worldSpaceInitial /= worldSpaceInitial.w;

                // Unproject the current mouse position
                glm::vec4 clipSpaceCurrent = glm::vec4(2.0f * (currentMousePos.x / viewportSize.x) - 1.0f, 1.0f - 2.0f * (currentMousePos.y / viewportSize.y), 0.0f, 1.0f);
                glm::vec4 worldSpaceCurrent = invViewProjMtx * clipSpaceCurrent;
                worldSpaceCurrent /= worldSpaceCurrent.w;

                // Calculate the world space delta
                glm::vec3 worldDelta = glm::vec3(worldSpaceCurrent - worldSpaceInitial);

                Transform& transform = gCoordinator->GetComponent<Transform>(draggedEntity);
                transform.position += worldDelta;

                if (gCoordinator->HasComponent<Collider>(draggedEntity)) {
                    Collider& collider = gCoordinator->GetComponent<Collider>(draggedEntity);
                    collider.position += {worldDelta.x, worldDelta.y};
                }

                lastMousePos = currentMousePos;
            }
            // If left mouse button is released, stop dragging
            else if (ImGui::IsMouseReleased(0)) {
                draggedEntity = -1;
            }
        }

        auto& camera = ::gCoordinator->GetComponent<Camera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera());
        auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
        if (ImGui::IsWindowFocused() && renderSystem->IsEditorMode()) {
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
                camera.mZoom += CAMERA_ZOOMSPEED * dt;
                camera.ZoomIn();
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_PRESSED, GLFW_KEY_F)) {
                camera.mZoom -= CAMERA_ZOOMSPEED * dt;
                camera.ZoomOut();
            }
            if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_X)) {
                ::gCoordinator->GetSystem<RenderSystem>()->ToggleDebugMode();
            }

        }
        //tch: hello this is my input part
        if (ImGui::IsWindowHovered()) {
            ImGuiIO& io = ImGui::GetIO();
            ImVec2 mousePos = io.MousePos;

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

        //ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(contentSize.x, contentSize.y), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(ENGINE_SCREEN_WIDTH / gScalingFactor, ENGINE_SCREEN_HEIGHT / gScalingFactor), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
    }

    /*  _________________________________________________________________________ */
    /*! PrefabWindow

    @param none.

    @return none.

    This function displays the prefab framebuffer window.
    */
    void PrefabWindow() {
        ImGui::Begin("Prefab Editor");
        auto const& framebuffer = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer(1);
        unsigned int texHdl = framebuffer->GetColorAttachmentID();

        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(ENGINE_SCREEN_WIDTH / gScalingFactor, ENGINE_SCREEN_HEIGHT / gScalingFactor), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
    }

    /*  _________________________________________________________________________ */
    /*! TextureHdlWindow

    @param none.

    @return none.

    This function displays all the assets and allows for dragging of the assets.
    */
    void ContentWindow() {
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

        ImGui::Begin("Renderer Stats");
        auto stats = Renderer::GetStats();
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVtxCount());
        ImGui::Text("Indices: %d", stats.GetTotalIdxCount());
        ImGui::End();
        Renderer::ResetStats();
    }

}