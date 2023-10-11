#include "../include/pch.hpp"

#include "WindowManager.hpp"

#include "Core/Coordinator.hpp"
#include "Core/Globals.hpp"
#include "Logging/LoggingSystem.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}
std::shared_ptr<WindowManager> WindowManager::_mSelf = 0;

// TODO: Return error to caller
void WindowManager::Init(
	std::string const& windowTitle, unsigned int windowWidth, unsigned int windowHeight, unsigned int windowPositionX,
	unsigned int windowPositionY)
{
	UNREFERENCED_PARAMETER(windowPositionX);
	UNREFERENCED_PARAMETER(windowPositionY);
	glfwInit();
	gCoordinator = Coordinator::GetInstance();

	mWindow = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	// Create OpenGL Context
	glfwMakeContextCurrent(mWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glViewport(0, 0, windowWidth, windowHeight);

	// Configure OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glfwSetErrorCallback(WindowManager::ErrorCb);

	glfwSetFramebufferSizeCallback(mWindow, WindowManager::FbSizeCb);
	glfwSetKeyCallback(mWindow, WindowManager::KeyCb);
	glfwSetMouseButtonCallback(mWindow, WindowManager::MouseButtonCb);
	glfwSetCursorPosCallback(mWindow, WindowManager::MousePosCb);
	glfwSetScrollCallback(mWindow, WindowManager::MouseScrollCb);

	// this is the default setting ...
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void WindowManager::Update()
{

	glfwSwapBuffers(mWindow);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOLINT (hicpp-signed-bitwise)
}

void WindowManager::Shutdown()
{
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void WindowManager::ProcessEvents()
{
	mPrevButtons = mButtons;
	mPrevMouseButtons = mMouseButtons;

	glfwPollEvents();

	////hack to send event for pressed key
	Event event(Events::Window::INPUT);
	event.SetParam(Events::Window::Input::KEY_PRESS,mButtons & mPrevButtons);
	event.SetParam(Events::Window::Input::MOUSE_PRESS, mMouseButtons & mPrevMouseButtons);
	gCoordinator->SendEvent(event);

}
void WindowManager::UpdateWindowTitle(std::string str) {
	glfwSetWindowTitle(mWindow, str.c_str());
}

bool WindowManager::ShouldClose() {
	return glfwWindowShouldClose(mWindow);
}

void WindowManager::KeyCb(GLFWwindow* pwin, int key, int scancode, int action, int mod) {
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(scancode);

	KeyState const& prevButtons {GetInstance()->mPrevButtons};
	KeyState& currButtons {GetInstance()->mButtons};
	Event event(Events::Window::INPUT);
    if (GLFW_PRESS == action) {
        if (GLFW_KEY_ESCAPE == key) {
            glfwSetWindowShouldClose(pwin, GLFW_TRUE);
        }
		GetInstance()->SetKey(currButtons, key, true);

		if (!prevButtons.test(static_cast<std::size_t>(key)) && currButtons.test(static_cast<std::size_t>(key))) {
			event.SetParam(Events::Window::Input::KEY_CLICK, GetInstance()->mButtons);
#ifdef _DEBUG
			std::cout << "key click\n";
#endif
			gCoordinator->SendEvent(event);
		}
	}
    else if (GLFW_RELEASE == action) {
        //key_flags[key] = GL_FALSE;
		GetInstance()->SetKey(currButtons, key, false);
		auto bsB{ GetInstance()->mButtons };
		event.SetParam(Events::Window::Input::KEY_RELEASE, bsB.flip());
#ifdef _DEBUG
		std::cout << "key released\n";
		
#endif
		gCoordinator->SendEvent(event);
	}
}

void WindowManager::MouseButtonCb(GLFWwindow* pwin, int button, int action, int mod) {
    UNREFERENCED_PARAMETER(mod);
    UNREFERENCED_PARAMETER(pwin);
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
#ifdef _DEBUG
        std::cout << "Left mouse button ";
#endif
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
#ifdef _DEBUG
        std::cout << "Right mouse button ";
#endif
        break;
    }
	MouseKeyState const& prevButtons {GetInstance()->mPrevMouseButtons};
	MouseKeyState& currButtons {GetInstance()->mMouseButtons};

	Event event(Events::Window::INPUT);
	//MouseKeyState& mousebtn;
    switch (action) {
    case GLFW_PRESS:
#ifdef _DEBUG
        std::cout << "pressed!!!" << std::endl;
#endif
		if (GLFW_MOUSE_BUTTON_LEFT == button) {
			currButtons.set(static_cast<size_t>(MouseButtons::LB), true);
			if (!prevButtons.test(static_cast<std::size_t>(MouseButtons::LB)) && currButtons.test(static_cast<std::size_t>(MouseButtons::LB))) {
				event.SetParam(Events::Window::Input::MOUSE_CLICK, currButtons);
				gCoordinator->SendEvent(event);
			}
		}
		if (GLFW_MOUSE_BUTTON_RIGHT == button) {
			currButtons.set(static_cast<size_t>(MouseButtons::RB), true);
			if (!prevButtons.test(static_cast<std::size_t>(MouseButtons::RB)) && currButtons.test(static_cast<std::size_t>(MouseButtons::RB))) {
				event.SetParam(Events::Window::Input::MOUSE_CLICK, currButtons);
				gCoordinator->SendEvent(event);
			}
		}
		if (GLFW_MOUSE_BUTTON_MIDDLE == button) {
			currButtons.set(static_cast<size_t>(MouseButtons::MB), true);
			if (!prevButtons.test(static_cast<std::size_t>(MouseButtons::MB)) && currButtons.test(static_cast<std::size_t>(MouseButtons::MB))) {
				event.SetParam(Events::Window::Input::MOUSE_CLICK, currButtons);
				gCoordinator->SendEvent(event);
			}
		}
        break;
    case GLFW_RELEASE:
#ifdef _DEBUG
        std::cout << "released!!!" << std::endl;
#endif
		if (GLFW_MOUSE_BUTTON_LEFT == button) {
			currButtons.reset(static_cast<size_t>(MouseButtons::LB));
		}
		if (GLFW_MOUSE_BUTTON_RIGHT == button) {
			currButtons.reset(static_cast<size_t>(MouseButtons::RB));
		}
		if (GLFW_MOUSE_BUTTON_MIDDLE == button) {
			currButtons.reset(static_cast<size_t>(MouseButtons::MB));
		}
		auto bsM{ currButtons };
		event.SetParam(Events::Window::Input::MOUSE_RELEASE, bsM.flip());
		gCoordinator->SendEvent(event);
        break;
    }
}

void WindowManager::MousePosCb(GLFWwindow* pwin, double xpos, double ypos) {
    UNREFERENCED_PARAMETER(ypos);
    UNREFERENCED_PARAMETER(xpos);
    UNREFERENCED_PARAMETER(pwin);
	Event event(Events::Window::INPUT);
	event.SetParam(Events::Window::Input::MOUSE_MOVE, MousePosition(static_cast<float>(xpos), static_cast<float>(ypos)));
	gCoordinator->SendEvent(event);
#ifdef _DEBUG
    std::cout << "Mouse cursor position: (" << xpos << ", " << ypos << ")" << std::endl;
#endif
}

GLFWwindow* WindowManager::GetContext(){
	return mWindow;
}

void WindowManager::MouseScrollCb(GLFWwindow* pwin, double xoffset, double yoffset) {
    UNREFERENCED_PARAMETER(pwin);
    UNREFERENCED_PARAMETER(xoffset);
    UNREFERENCED_PARAMETER(yoffset);
#ifdef _DEBUG
    std::cout << "Mouse scroll wheel offset: ("
        << xoffset << ", " << yoffset << ")" << std::endl;
#endif
}

void WindowManager::ErrorCb(int error, char const* description) {
    UNREFERENCED_PARAMETER(error);
    UNREFERENCED_PARAMETER(description);
#ifdef _DEBUG
    std::cerr << "GLFW error: " << description << std::endl;
#endif
}

void WindowManager::FbSizeCb(GLFWwindow* pwin, int width1, int height1) {
    UNREFERENCED_PARAMETER(pwin);
    UNREFERENCED_PARAMETER(width1);
    UNREFERENCED_PARAMETER(height1);
#ifdef _DEBUG
    std::cout << "fbsize_cb getting called!!!" << std::endl;
#endif
    // use the entire framebuffer as drawing region
    
	
	
	(0, 0, width1, height1);
    // later, if working in 3D, we'll have to set the projection matrix here ...
}

template <typename _bitset>
void WindowManager::SetKey(_bitset & bs, GLenum key, bool state) {
	(state) ? bs.set(static_cast<std::size_t>(key)) : 
		bs.reset(static_cast<std::size_t>(key));
}