#include "Systems/InputSystem.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Core/Coordinator.hpp"
#include <Core/Globals.hpp>
#include <random>
#include <chrono>

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void InputSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();
	::gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::INPUT, InputSystem::InputListener));
}
bool InputSystem::CheckKey(InputKeyState state, size_t key) const {
	bool out{ false };
	switch (state) {
	case InputKeyState::KEY_PRESSED:
		out= mButtonsPressed.test(static_cast<std::size_t>(key));
		break;
	case InputKeyState::KEY_CLICKED:
		out = mButtonsClicked.test(static_cast<std::size_t>(key));
		break;
	case InputKeyState::KEY_RELEASED:
		out = mButtonsReleased.test(static_cast<std::size_t>(key));
		break;
	case InputKeyState::MOUSE_PRESSED:
		out = mMouseButtonsPressed.test(static_cast<std::size_t>(key));
		break;
	case InputKeyState::MOUSE_CLICKED:
		out = mMouseButtonsClicked.test(static_cast<std::size_t>(key));
		break;
	case InputKeyState::MOUSE_RELEASED:
		out = mMouseButtonsReleased.test(static_cast<std::size_t>(key));
		break;
	}
	return out;
}
void InputSystem::Update()
{
	mButtonsPressed.reset();
	mButtonsClicked.reset();
	mButtonsReleased.reset();

	mMouseButtonsPressed.reset();
	mMouseButtonsClicked.reset();
	mMouseButtonsReleased.reset();
}

void InputSystem::InputListener(Event& event)
{
	KeyState press {event.GetParam<KeyState>(Events::Window::Input::KEY_PRESS)};
	KeyState click {event.GetParam<KeyState>(Events::Window::Input::KEY_CLICK)};
	KeyState release {event.GetParam<KeyState>(Events::Window::Input::KEY_RELEASE)};
	mButtonsPressed = press;
	if (click.any()) mButtonsClicked = click;
	if (release.any()) mButtonsReleased = release;

	MouseKeyState mspress {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_PRESS)};
	MouseKeyState msclick {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_CLICK)};
	MouseKeyState msrelease {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_RELEASE)};
	mMouseButtonsPressed = mspress;
	if (msclick.any()) mMouseButtonsClicked = msclick;
	if (msrelease.any()) mMouseButtonsReleased = msrelease;

	mMousePos = event.GetParam<MousePosition>(Events::Window::Input::MOUSE_MOVE);
}
