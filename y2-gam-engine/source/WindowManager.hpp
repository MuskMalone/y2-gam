/******************************************************************************/
/*!
\par        Image Engine
\file       WindowManager.hpp

\author     
\date       August 10, 2023

\brief      Header for the WindowManager class.

			Manages the window and initialization as well as fullscreen toggling.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <string>
#include <Core/Types.hpp>
#include <memory>
#include <Core/Event.hpp>


class WindowManager
{
public:
	static std::shared_ptr<WindowManager> GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<WindowManager>();
		return _mSelf;
	}
	void Init(
		std::string const& windowTitle, unsigned int windowWidth, unsigned int windowHeight,
		unsigned int windowPositionX, unsigned int windowPositionY);

	void Update();

	void ProcessEvents();

	void Shutdown();

	void UpdateWindowTitle(std::string str);

	bool ShouldClose();

	static void ErrorCb(int error, char const* description);
	static void FbSizeCb(GLFWwindow* pwin, int width, int height);
	// I/O callbacks ...
	static void KeyCb(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	static void MouseButtonCb(GLFWwindow* pwin, int button, int action, int mod);
	static void MouseScrollCb(GLFWwindow* pwin, double xoffset, double yoffset);
	static void MousePosCb(GLFWwindow* pwin, double xpos, double ypos);

	void FullscreenListener(Event const& event);
	GLFWwindow* GetContext();

	    int GetWidth() const {
        if (mIsFullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            return mode ? mode->width : 0;
        } else {
			int width, height;
			glfwGetWindowSize(mWindow, &width, &height);
			return width;
        }
    }

    int GetHeight() const {
        if (mIsFullscreen) {
            const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
            return mode ? mode->height : 0;
        } else {
			int width, height;
			glfwGetWindowSize(mWindow, &width, &height);
			return height;
        }
    }

private:
	static std::shared_ptr<WindowManager> _mSelf;

	GLFWwindow* mWindow{};

	int mWindowedWidth{ENGINE_SCREEN_WIDTH}, mWindowedHeight{ENGINE_SCREEN_HEIGHT};
	//int mWindowedWidth{}, mWindowedHeight{};
	int mWindowedPosX{}, mWindowedPosY{};

#ifndef _INSTALLER
	bool mIsFullscreen{ false };
#else 
	bool mIsFullscreen{ true };
#endif

	KeyState mButtons{};
	KeyState mPrevButtons{};
	MouseKeyState mMouseButtons{};
	MouseKeyState mPrevMouseButtons{};
	//std::pair<bool, bool> mKeystateLb{ false, false };
	//std::pair<bool, bool> mKeystateRb{ false, false };
	//std::pair<bool, bool> mKeystateMb{ false, false };
	template <typename _bitset>
	void SetKey(_bitset & bs, GLenum key, bool state);
};
