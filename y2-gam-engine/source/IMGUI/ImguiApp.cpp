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
            menu, hierarchy, inspector, property, buffer, and logging windows. 

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Imgui/ImguiApp.hpp"
#include "IMGUI/ImguiComponent.hpp"
#include "Math/MathUtils.h"
#include "Components/Collider.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/Animation.hpp"
#include "Components/Sprite.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/Tag.hpp"

#include "Components/Editor.hpp"
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

Entity gSelectedEntity=MAX_ENTITIES;
namespace {
    std::shared_ptr<Coordinator> gCoordinator;
}
namespace Image {
    const float scalingFactor = 1.5f;
    /*  _________________________________________________________________________ */
    /*! AppRender

    @param mEntities
    The set of Entities to loop through.

    @return none.

    This function is responsible for rendering the application's UI and
    handling user input.
    Pressing the 'z' key toggles between dock space and non-dock space.
    Pressing the 'c' key to clear the entities
    */
    void AppRender(std::set<Entity>const& mEntities) {
        //Press z to change dock space and non dock space
        //static bool showDockSpace{ true };
        static bool toDelete{ false };

        /*if (ImGui::IsKeyReleased(ImGuiKey_Z)) {
            showDockSpace = !showDockSpace;
        }*/
        if (ImGui::IsKeyReleased(ImGuiKey_C)) {
            NodeManager::ClearAllNodes();
            toDelete = !toDelete;
        }

            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        /*if (showDockSpace) {
        }*/
        //ImGui::ShowDemoWindow();
        MainMenuWindow();
        HierarchyWindow(mEntities);
        InspectorWindow();
        PropertyWindow();
        BufferWindow();
        PrefabWindow();
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
                if (ImGui::MenuItem("New")) {}
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Open Recent")) {
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As")) {}
                ImGui::Separator();
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
        ::gCoordinator = Coordinator::GetInstance();
        ImGui::Begin("Hierarchy");
        //Create entity and destory first
        if (ImGui::Button("Create Entity")) {
            Entity newEntity = gCoordinator->CreateEntity();
            gSelectedEntity = newEntity;
            ImGuiViewport* vP = ImGui::GetWindowViewport();
            gCoordinator->AddComponent(
                gSelectedEntity,
                Transform{
                    {vP->Pos.x,vP->Pos.y,0},
                    {0,0,0},
                    {5,5,5}
                });
            gCoordinator->AddComponent(
                gSelectedEntity,
                Tag{"Name"});
        }

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
        }
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
            //if (ImGui::CollapsingHeader("Transform")) {

                if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                    Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);

                    // Position
                    ImGui::Text("Position");
                    ImGui::SliderFloat("Pos X", &transform.position.x, -ENGINE_SCREEN_WIDTH / 4.f, ENGINE_SCREEN_WIDTH / 4.f);
                    ImGui::SliderFloat("Pos Y", &transform.position.y, -ENGINE_SCREEN_HEIGHT / 4.f, ENGINE_SCREEN_HEIGHT / 4.f);

                    // Rotation
                    ImGui::Text("Rotation");
                    ImGui::SliderFloat("Rot Z", &transform.rotation.z, -180, 180); // change to Degree(gPI) same as glm func in math ultiles

                    // Scale
                    ImGui::Text("Scale");
                    ImGui::SliderFloat("Scale X", &transform.scale.x, 1, 50);
                    ImGui::SliderFloat("Scale Y", &transform.scale.y, 1, 50);
                }
            //}
            //if (ImGui::CollapsingHeader("Sprite")) {

                if (gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                    Sprite& sprite = gCoordinator->GetComponent<Sprite>(gSelectedEntity);
                    //Color
                    ImGui::Separator();
                    ImGui::Text("Color");
                    ImGui::ColorPicker4("Color Picker", &sprite.color.r);
                }
            //    else {
            //        ImGui::Text("No Sprite Component");
            //    }
            //}
            //if (ImGui::CollapsingHeader("RigidBody")) {
                if (gCoordinator->HasComponent<Collider>(gSelectedEntity)) {
                    Collider& collider = gCoordinator->GetComponent<Collider>(gSelectedEntity);

                    ImGui::Separator();
                    ImGui::Text("Collider");

                    ImGui::Text("Type");
                    const char* colliderTypes[]{ "BOX", "CIRCLE" };//box, circle;
                    ImGui::Combo("Collider Type", reinterpret_cast<int*>(&collider.type), colliderTypes, IM_ARRAYSIZE(colliderTypes));
                    //Pos
                    ImGui::Text("Position");
                    ImGui::SliderFloat("Collider Pos X", &collider.position.x, -ENGINE_SCREEN_WIDTH / 4.f, ENGINE_SCREEN_WIDTH / 4.f);
                    ImGui::SliderFloat("Collider Pos Y", &collider.position.y, -ENGINE_SCREEN_HEIGHT / 4.f, ENGINE_SCREEN_HEIGHT / 4.f);
                    // Rotation
                    ImGui::Text("Rotation");
                    ImGui::SliderFloat("Collider Rot", &collider.rotation, -180, 180); // change to Degree(gPI) same as glm func in math ultiles
                    // Scale
                    if (collider.type == ColliderType::BOX) {
                        ImGui::Text("Dimension");
                        ImGui::SliderFloat("Collider Scale X", &collider.dimension.x, 1, 50);
                        ImGui::SliderFloat("Collider Scale Y", &collider.dimension.y, 1, 50);

                    }
                    else {
                        ImGui::Text("Diameter");
                        ImGui::SliderFloat("Collider Scale X", &collider.dimension.x, 1, 50);
                        collider.dimension.y = collider.dimension.x;
                    }


                    // Mass

                }
                if (gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                    RigidBody& rigidBody = gCoordinator->GetComponent<RigidBody>(gSelectedEntity);

                    ImGui::Separator();
                    ImGui::Text("RigidBody");
                    //Pos
                    ImGui::Text("Position");
                    ImGui::SliderFloat("Pos X", &rigidBody.position.x, -ENGINE_SCREEN_WIDTH / 4.f, ENGINE_SCREEN_WIDTH / 4.f);
                    ImGui::SliderFloat("Pos Y", &rigidBody.position.y, -ENGINE_SCREEN_HEIGHT / 4.f, ENGINE_SCREEN_HEIGHT / 4.f);
                    // Rotation
                    ImGui::Text("Rotation");
                    ImGui::SliderFloat("Rot Z", &rigidBody.rotation, -180, 180); // change to Degree(gPI) same as glm func in math ultiles
                    // Scale
                    ImGui::Text("Dimension");
                    ImGui::SliderFloat("Scale X", &rigidBody.dimension.x, 1, 50);
                    ImGui::SliderFloat("Scale Y", &rigidBody.dimension.y, 1, 50);
                    // Mass
                    ImGui::Text("Mass");
                    ImGui::InputFloat("Mass", &rigidBody.mass);
                    rigidBody.SetMass(rigidBody.mass);
                }
                /*else {
                    ImGui::Text("No RigidBody Component");
                }
            }*/
            //if (ImGui::CollapsingHeader("Gravity")) {
                if (gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                    Gravity& gravity = gCoordinator->GetComponent<Gravity>(gSelectedEntity);
                    ImGui::Separator();
                    //Force
                    ImGui::Text("Gravity");
                    ImGui::SliderFloat("Force X", &gravity.force.x, -10, 10);
                    ImGui::SliderFloat("Force Y", &gravity.force.y, -10, 10);
                }
                /*else {
                    ImGui::Text("No Gravity Component");
                }
            }*/
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
        const char* components[] = { "Transform", "Sprite", "RigidBody", "Collision","Animation","Gravity","Tag"};
        static int selectedComponent{ -1 };
        if (gSelectedEntity != MAX_ENTITIES) {
            ImGui::Text("Entity ID: %d", gSelectedEntity);
            if (gCoordinator->HasComponent<Tag>(gSelectedEntity)) {
                static Entity gPreviousEntity = {MAX_ENTITIES}; 
                static char tag[256] = "";
                if (gPreviousEntity != gSelectedEntity) {
                    memset(tag, 0, sizeof(tag));
                    gPreviousEntity = gSelectedEntity; 
                }
                Tag& tagComponent = gCoordinator->GetComponent<Tag>(gSelectedEntity);
                ImGui::Text("Current Tag: %s", tagComponent.tag.c_str());
                //ImGui::SetNextItemWidth(50.0f);
                if (ImGui::InputText("Tag", tag, IM_ARRAYSIZE(tag))) {
                    tagComponent.tag = tag;
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
                                {5,5,5}
                            });
                    }
                }
                      break;
                case 1: {
                    if (!gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Sprite{
                                {1,0,0, 1},
                                nullptr
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
                                    50.f,
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
                                    50.f,
                                    Vec2{5.f,5.f}
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
                        //------------TEMPORARY TO BE READ FROM JSON FILES------------------------------------------------------------------/
                        std::vector<AnimationFrame> idleFrames{ {0.f, 0}, {0.f, 1}, { 0.f, 2 }, { 0.f, 3 }, { 0.f, 4 }, { 0.f, 5 }, { 0.f, 6 }, { 0.f, 7} };
                        std::vector<AnimationFrame> runFrames{ {0.f, 8}, {0.f, 9}, { 0.f, 10 }, { 0.f, 11 }, { 0.f, 12 }, { 0.f, 13 }, { 0.f, 14 }, { 0.f, 15 } };
                        std::vector<AnimationFrame> attackFrames{ {0.f, 16}, {0.f, 17}, { 0.f, 18 }, { 0.f, 19 }, { 0.f, 20 }, { 0.f, 21 }, { 0.f, 22 } };
                        std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> map{ {ANIM_STATE::IDLE, idleFrames},
                                                                                         {ANIM_STATE::RUN, runFrames},
                                                                                         {ANIM_STATE::ATTACK, attackFrames} };
                        ::gCoordinator->AddComponent(
                            gSelectedEntity,
                            Animation{
                                0.08f,
                                0,
                                ANIM_STATE::IDLE,
                                map
                            });
                    }
                }
                      break;
                case 5: {
                    if (!gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Gravity{ Vec2{0.f,0.f} });
                    }
                }
                      break;
                case 6: {
                    if (!gCoordinator->HasComponent<Tag>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Tag{"Entity "+std::to_string(gSelectedEntity)});
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

     This function displays the game engine's framebuffer.
    */
    void BufferWindow() {
        static int draggedEntity = -1;   // -1 means no entity is being dragged.
        static ImVec2 lastMousePos;      // Store the last position of the mouse.
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 originalPadding = style.WindowPadding;
        style.WindowPadding = ImVec2(0.0f, 0.0f);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;

        ImGui::Begin("Image Game Engine", nullptr, window_flags);
        auto const& framebuffer = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer(0);
        unsigned int texHdl = framebuffer->GetColorAttachmentID();

        if (ImGui::IsWindowHovered()) {

            //mouse picking part:
            ImVec2 contentSize = ImGui::GetContentRegionAvail();
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
            int fbX = static_cast<int>(mouseX * scalingFactor);
            int fbY = static_cast<int>(mouseY * scalingFactor);

            if (ImGui::IsMouseClicked(0) && draggedEntity == -1) {
                if (mouseX >= 0 && mouseX < static_cast<int>(viewportSize.x) && mouseY >= 0 && mouseY < static_cast<int>(viewportSize.y)) {
                    framebuffer->Bind();
                    int pixelData = framebuffer->ReadPixel(1, fbX, fbY);
                    framebuffer->Unbind();
                    draggedEntity = pixelData;
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

        //tch: hello this is my input part
        if (ImGui::IsWindowHovered()) {
            ImGuiIO& io = ImGui::GetIO();
            ImVec2 mousePos = io.MousePos;

            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;

            ImVec2 paddedTopLeft = ImVec2(windowPos.x + windowPadding.x, windowPos.y + windowPadding.y);
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 paddedBottomRight = ImVec2(windowPos.x + windowSize.x - windowPadding.x, windowPos.y + windowSize.y - windowPadding.y);

            if (ImGui::IsMouseHoveringRect(paddedTopLeft, paddedBottomRight)) {
                Event event(Events::Window::INPUT);
                event.SetParam(Events::Window::Input::EDITOR_MOUSE_MOVE, EditorMousePosition(MousePosition(
                    static_cast<float>(mousePos.x - paddedTopLeft.x), 
                    static_cast<float>((mousePos.y - paddedTopLeft.y) )
                ), MousePosition(paddedBottomRight.x - paddedTopLeft.x, paddedBottomRight.y - paddedTopLeft.y))
                );
                gCoordinator->SendEvent(event);
            }
        }

        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(ENGINE_SCREEN_WIDTH / scalingFactor, ENGINE_SCREEN_HEIGHT / scalingFactor), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
    }

    void PrefabWindow() {
        ImGui::Begin("Prefab Editor");
        auto const& framebuffer = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer(1);
        unsigned int texHdl = framebuffer->GetColorAttachmentID();
        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(ENGINE_SCREEN_WIDTH / scalingFactor, ENGINE_SCREEN_HEIGHT / scalingFactor), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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

    void RenderStatsWindow() {

        ImGui::Begin("Renderer Stats");
        auto stats = Renderer::GetStats();
        ImGui::Text("Draw Calls: %d", stats.drawCalls);
        ImGui::Text("Quads: %d", stats.quadCount);
        ImGui::Text("Vertices: %d", stats.GetTotalVtxCount());
        ImGui::Text("Indics: %d", stats.GetTotalIdxCount());
        ImGui::End();
        Renderer::ResetStats();
    }

}