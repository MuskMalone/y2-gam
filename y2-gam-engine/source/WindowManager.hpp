#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bitset>
#include <string>
#include <Core/Globals.hpp>
#include <memory>


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

private:
	static std::shared_ptr<WindowManager> _mSelf;

	GLFWwindow* mWindow;

	std::bitset<ENGINE_KEYS_COUNT> mButtons{};
	std::bitset<ENGINE_KEYS_COUNT> mPrevButtons{};
	std::pair<bool, bool> mKeystateLb{ false, false };
	std::pair<bool, bool> mKeystateRb{ false, false };
	std::pair<bool, bool> mKeystateMb{ false, false };
	template <typename _bitset>
	void SetKey(_bitset & bs, GLenum key, bool state);
};
