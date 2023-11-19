/******************************************************************************/
/*!
\par        Image Engine
\file       ImguiSystem.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Implementation file for Imgui Systems

            This file sets up and manages the ImGui context for rendering GUI 
            elements. It provides initialization, update, and destruction for 
            the ImGui system, as well as event handling for entities added 
            to the ECS.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "../include/pch.hpp"

#include "Systems/ImguiSystem.hpp"
#include "IMGUI/ImguiApp.hpp"
#include <Core/Globals.hpp>
#include "../WindowManager.hpp"

#include "ImGuizmo.h"
#define GL_SILENCE_DEPRECATION

namespace {
    std::shared_ptr<Coordinator> gCoordinator{};
}
    /*  _________________________________________________________________________ */
    /*! glfw_error_callback

    @param error
    The error code

    @param description
    The description of the error

    @return none.

    GLFW error call back function
    */
    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    /*  _________________________________________________________________________ */
    /*! Init

    @param window
    The pointer to GLFW window

    @return none.

    This function sets up the ImGui context, style, and renderer backends.
    */
    void ImGuiSystem::Init(GLFWwindow* window){
        gCoordinator = Coordinator::GetInstance();
        ::gCoordinator->AddEventListener(METHOD_LISTENER(Events::System::ENTITY, ImGuiSystem::ImguiEventListener));
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return;

         // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;     
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        return;
    }
    /*  _________________________________________________________________________ */
    /*! Update

    @param none

    @return none.

    This function handles input events, starts the ImGui frame, renders the 
    application GUI, and updates platform windows.
    */
    void ImGuiSystem::Update(float dt){
        ImGuiIO& io = ImGui::GetIO();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
        Image::AppRender(mEntities,dt);

        int width, height;
        glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
        glViewport(0,0, width, height);
        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }
    /*  _________________________________________________________________________ */
    /*! Destroy

    @param none

    @return none.

    This function cleans up the ImGui context and shuts down the renderer backends.
    */
    void ImGuiSystem::Destroy() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    /*  _________________________________________________________________________ */
    /*! ImguiEventListener

    @param event
    Reference to the event object.

    @return none.

    This function listens for entity creation events and marks them for serialization.
    */
    void ImGuiSystem::ImguiEventListener(Event& event){
        //every item added to the ecs will be marked as an entity to serialize
        Entity e{ event.GetParam<Entity>(Events::System::Entity::CREATE) };
        if (event.GetFail()) return;
        gCoordinator->AddComponent<ImguiComponent>(e, {});
    }
