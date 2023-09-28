#pragma once
#include "IMGUI/ImguiApp.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Math/MathUtils.h"
#include "Core/System.hpp"
#include "Core/EventTypes.hpp"
static void glfw_error_callback(int error, const char* description);
class ImGuiSystem : public System
{
public:

    void Init(GLFWwindow* window);
    void Update(GLFWwindow* window);
    void Destroy();
private:
    void ImguiEventListener(Event& event);
};
