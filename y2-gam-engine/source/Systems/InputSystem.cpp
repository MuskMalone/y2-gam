#include "../include/pch.hpp"

#include "Systems/InputSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Components/OrthoCamera.hpp"
#include "Core/Coordinator.hpp"

#include <Core/Globals.hpp>

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
MousePosition InputSystem::GetMousePos() const {
	return mMousePos;
}
MousePosition InputSystem::GetWorldMousePos() const {
	auto const& camera{ ::gCoordinator->GetComponent<OrthoCamera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera()) };

	float screenWidth{ mEditorMousePos.second.first };
	float screenHeight{ mEditorMousePos.second.second };

	float x { (2.0f * mEditorMousePos.first.first) / screenWidth - 1.0f };
	float y{ 1.0f - (2.0f * mEditorMousePos.first.second) / screenHeight };

	glm::vec4 screenPos{ x, y, 0.f, 1.f };
	glm::mat4 inversedMtx { glm::inverse(camera.GetViewProjMtx()) };
	glm::vec4 worldPos { inversedMtx * screenPos };

	//return mMousePos;
	//std::cout << worldPos.x << " " << worldPos.y << std::endl;
	return { worldPos.x, worldPos.y };
}
EditorMousePosition InputSystem::GetEditorMousePos() const {
	//auto const& camera{ ::gCoordinator->GetComponent<OrthoCamera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera()) };
	//camera.
	return mEditorMousePos;
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
	if (release.any()) mButtonsReleased |= release;

	MouseKeyState mspress {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_PRESS)};
	MouseKeyState msclick {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_CLICK)};
	MouseKeyState msrelease {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_RELEASE)};
	mMouseButtonsPressed = mspress;
	if (msclick.any()) mMouseButtonsClicked = msclick;
	if (msrelease.any()) mMouseButtonsReleased |= msrelease;

	MousePosition mp = event.GetParam<MousePosition>(Events::Window::Input::MOUSE_MOVE);
	if (!event.GetFail()) {
		mMousePos = std::move(mp);
		//std::cout << mMousePos.first << " " << mMousePos.second << std::endl;
	}

	EditorMousePosition emp = event.GetParam<EditorMousePosition>(Events::Window::Input::EDITOR_MOUSE_MOVE);
	if (!event.GetFail()) {
		mEditorMousePos = std::move(emp);
		//std::cout << mEditorMousePos.first.first << " " << mEditorMousePos.first.second << std::endl;
		//std::cout << mEditorMousePos.second.first << " " << mEditorMousePos.second.second << std::endl;
		GetWorldMousePos();
	}
}
