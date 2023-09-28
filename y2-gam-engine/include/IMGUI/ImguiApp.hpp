#pragma once
#include "IMGUI/ImguiApp.hpp"
#include "IMGUI/ImguiComponent.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Math/MathUtils.h"
#include "Components/BoxCollider.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/Animation.hpp"
#include "Components/Sprite.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/Editor.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/EditorControlSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/CollisionSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include <chrono>
#include <random>
#include <Core/Globals.hpp>
#include "Graphics/Renderer.hpp"
#include <Core/FrameRateController.hpp>
#include "Math/Vec3.hpp"
#include <fstream>
#include <iostream>
#include "Graphics/Renderer.hpp"
namespace Image {
    void AppRender(std::set<Entity>const& mEntities);
    void MainMenuWindow();
    void HierarchyWindow(std::set<Entity>const& mEntities);
    void InspectorWindow();
    void PropertyWindow();
    void BufferWindow();
    void LoggingWindow();
}