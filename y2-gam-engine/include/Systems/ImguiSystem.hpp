/******************************************************************************/
/*!
\par        Image Engine
\file       ImguiSystem.hpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Header file for Imgui Systems

            This file sets up and manages the ImGui context for rendering GUI
            elements. It provides initialization, update, and destruction for
            the ImGui system, as well as event handling for entities added
            to the ECS.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

#include "IMGUI/ImguiApp.hpp"
#include "Math/MathUtils.h"
#include "Core/System.hpp"
#include "Core/EventTypes.hpp"
static void glfw_error_callback(int error, const char* description);
extern ImFont* mainfont;

class ImGuiSystem : public System
{
public:

    void Init(GLFWwindow* window);
    void Update(float dt);
    void Destroy();

private:
    void ImguiEventListener(Event& event);
    void CameraCreationListener(Event& event);
};
