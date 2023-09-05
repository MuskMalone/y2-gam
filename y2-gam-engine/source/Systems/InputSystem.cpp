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
	::gCoordinator = Coordinator::GetCoordinator();
	::gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::INPUT, InputSystem::InputListener));
}
bool InputSystem::CheckKey(KeyState state, size_t key) const {
	bool out{ false };
	switch (state) {
	case KeyState::KEY_PRESSED:
		out= mButtonsPressed.test(static_cast<std::size_t>(key));
		break;
	case KeyState::KEY_CLICKED:
		out = mButtonsClicked.test(static_cast<std::size_t>(key));
		break;
	case KeyState::KEY_RELEASED:
		out = mButtonsReleased.test(static_cast<std::size_t>(key));
		break;
	case KeyState::MOUSE_PRESSED:
		out = mMouseButtonsPressed.test(static_cast<std::size_t>(key));
		break;
	case KeyState::MOUSE_CLICKED:
		out = mMouseButtonsClicked.test(static_cast<std::size_t>(key));
		break;
	case KeyState::MOUSE_RELEASED:
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
	std::bitset<ENGINE_KEYS_COUNT> press {event.GetParam<std::bitset<ENGINE_KEYS_COUNT>>(Events::Window::Input::KEY_PRESS)};
	std::bitset<ENGINE_KEYS_COUNT> click {event.GetParam<std::bitset<ENGINE_KEYS_COUNT>>(Events::Window::Input::KEY_CLICK)};
	std::bitset<ENGINE_KEYS_COUNT> release {event.GetParam<std::bitset<ENGINE_KEYS_COUNT>>(Events::Window::Input::KEY_RELEASE)};
	mButtonsPressed = press;
	if (click.any()) mButtonsClicked = click;
	if (release.any()) mButtonsReleased = release;

	std::bitset<ENGINE_MOUSEKEYS_COUNT> mspress {event.GetParam<std::bitset<ENGINE_MOUSEKEYS_COUNT>>(Events::Window::Input::MOUSE_PRESS)};
	std::bitset<ENGINE_MOUSEKEYS_COUNT> msclick {event.GetParam<std::bitset<ENGINE_MOUSEKEYS_COUNT>>(Events::Window::Input::MOUSE_CLICK)};
	std::bitset<ENGINE_MOUSEKEYS_COUNT> msrelease {event.GetParam<std::bitset<ENGINE_MOUSEKEYS_COUNT>>(Events::Window::Input::MOUSE_RELEASE)};
	mMouseButtonsPressed = mspress;
	if (msclick.any()) mMouseButtonsClicked = msclick;
	if (msrelease.any()) mMouseButtonsReleased = msrelease;
}
