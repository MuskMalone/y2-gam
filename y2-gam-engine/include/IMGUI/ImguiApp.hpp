/******************************************************************************/
/*!
\par        Image Engine
\file       ImguiApp.hpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Sep 9, 2023

\brief      Header file for Imgui

            This file contains functions responsible for rendering different
            parts of the application's GUI. It includes the main rendering function,
            as well as functions for displaying various windows such as the main
            menu, hierarchy, inspector, property, buffer, and logging windows.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include "IMGUI/ImguiApp.hpp"
#include "IMGUI/ImguiComponent.hpp"
#include "Math/MathUtils.h"
#include "Components/Collider.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/Animation.hpp"
#include "Components/Sprite.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/Editor.hpp"
#include "Components/Script.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/LayeringSystem.hpp"
#include <Core/Globals.hpp>
#include "Graphics/Renderer.hpp"
#include <Core/FrameRateController.hpp>
#include "Graphics/Renderer.hpp"
namespace Image {
    void AppRender(std::set<Entity>const& mEntities,float dt);
    void MainMenuWindow();
    void HierarchyWindow(std::set<Entity>const& mEntities);
    void LayerWindow();
    void InspectorWindow(Entity selectedEntity);
    void PropertyWindow(Entity selectedEntity);
    void PrefabPropertyWindow();
    void GameObjectPropertyWindow();
    void PrefabInspectorWindow();
    void GameObjectInspectorWindow();
    void BufferWindow(float dt);
    void PrefabWindow();
    void ContentWindow();
    void PerformanceWindow();
    void LoggingWindow();
    void RenderStatsWindow();

    static ImVec2 mViewportDim{};
    void GuizmoWindow();
}
