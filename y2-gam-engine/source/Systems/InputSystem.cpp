/******************************************************************************/
/*!
\par        Image Engine
\file       InputSystem.cpp

\author     Team Hemp
\date       Sep 10, 2023

\brief      Input System

			System manages all the input and various functions for getting
			mouse position in various different coordinate systems.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "../include/pch.hpp"

#include "Systems/InputSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Core/Coordinator.hpp"

#include <Core/Globals.hpp>
#include "../WindowManager.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*!
	\brief Init

	Initializes the Input System by acquiring a global Coordinator instance and setting up event listeners for input events.
*/
void InputSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();
	::gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::INPUT, InputSystem::InputListener));
}

/*  _________________________________________________________________________ */
/*!
	\brief CheckKey

	Checks if a specific key is in a given state (e.g., pressed, released, clicked).

	\param state The state of the key to check.
	\param key The key code to check the state of.
	\return Returns true if the key is in the specified state; otherwise, false.
*/
bool InputSystem::CheckKey(InputKeyState state, size_t key) const {
	bool out{ false };
	switch (state) {
	case InputKeyState::KEY_RELEASED:
		out = mButtonsReleased.test(static_cast<std::size_t>(key));
		break;
	case InputKeyState::KEY_PRESSED:
		out= mButtonsPressed.test(static_cast<std::size_t>(key));
		break;
	case InputKeyState::KEY_CLICKED:
		out = mButtonsClicked.test(static_cast<std::size_t>(key));
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

/*  _________________________________________________________________________ */
/*!
	\brief GetMousePos

	Retrieves the current position of the mouse cursor.

	\return Returns a `MousePosition` struct containing the x and y screen coordinates of the mouse cursor.
*/
MousePosition InputSystem::GetMousePos() const {
	return mMousePos;
}


/*  _________________________________________________________________________ */
/*!
	\brief GetWorldMousePos

	Converts the current mouse position to world coordinates.

	\return Returns a `MousePosition` struct containing the x and y world coordinates of the mouse cursor.
*/
MousePosition InputSystem::GetWorldMousePos() const {
	Entity cameraEntity = ::gCoordinator->GetSystem<RenderSystem>()->GetCamera();
	if (!::gCoordinator->HasComponent<Camera>(cameraEntity)) {
		// Handle the error or return a default value
		return MousePosition{ 0.f, 0.f }; // Example default value
	}

	auto const& camera{ ::gCoordinator->GetComponent<Camera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera()) };

	float screenWidth{ mEditorMousePos.second.first };
	float screenHeight{ mEditorMousePos.second.second };

	float x { (2.0f * mEditorMousePos.first.first) / screenWidth - 1.0f };
	float y{ 1.0f - (2.0f * mEditorMousePos.first.second) / screenHeight };

	glm::vec4 screenPos{ x, y, 0.f, 1.f };
	glm::mat4 inversedMtx { glm::inverse(camera.GetViewProjMtx()) };
	glm::vec4 worldPos { inversedMtx * screenPos };

	return { worldPos.x, worldPos.y };
}

/*  _________________________________________________________________________ */
/*!
	\brief GetSceneMousePos

	Transforms the current mouse position to scene coordinates based on the active scene's camera.

	\return Returns a `MousePosition` struct with the mouse position transformed into the scene's coordinate space.
*/
MousePosition InputSystem::GetSceneMousePos() const {
	int windowWidth = WindowManager::GetInstance()->GetWidth();
	int windowHeight = WindowManager::GetInstance()->GetHeight();

	MousePosition mp{ GetMousePos() };

	// Normalize mouse position to NDC
	Vec2 mousePos{
		(2.0f * mp.first / static_cast<float>(windowWidth)) - 1.0f,
		1.0f - (2.0f * mp.second / static_cast<float>(windowHeight))
	};

	// Transform mouse position using the camera
	auto const& camera{ ::gCoordinator->GetComponent<Camera>(::gCoordinator->GetSystem<RenderSystem>()->GetCamera()) };
	glm::mat4 inverseViewProj = glm::inverse(camera.GetViewProjMtx());
	glm::vec4 mouseClipSpace = glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
	glm::vec4 mouseWorldSpace = inverseViewProj * mouseClipSpace;
	mouseWorldSpace /= mouseWorldSpace.w;

	return { mouseWorldSpace.x, mouseWorldSpace.y };
}

/*  _________________________________________________________________________ */
/*!
	\brief GetUIMousePos

	Converts the mouse position to UI coordinates, taking into account the UI camera's view and projection matrices.

	\return Returns a `MousePosition` struct containing the x and y UI coordinates of the mouse cursor.
*/
MousePosition InputSystem::GetUIMousePos() const {
	int windowWidth = WindowManager::GetInstance()->GetWidth();
	int windowHeight = WindowManager::GetInstance()->GetHeight();

	MousePosition mp{ GetMousePos() };

	// Normalize mouse position to NDC
	Vec2 mousePos{
		(2.0f * mp.first / static_cast<float>(windowWidth)) - 1.0f,
		1.0f - (2.0f * mp.second / static_cast<float>(windowHeight))
	};

	// Transform mouse position using the camera
	auto const& camera{ ::gCoordinator->GetComponent<Camera>(::gCoordinator->GetSystem<RenderSystem>()->GetUICamera()) };
	glm::mat4 inverseViewProj = glm::inverse(camera.GetViewProjMtx());
	glm::vec4 mouseClipSpace = glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
	glm::vec4 mouseWorldSpace = inverseViewProj * mouseClipSpace;
	mouseWorldSpace /= mouseWorldSpace.w;

	return { mouseWorldSpace.x, mouseWorldSpace.y };
}

/*  _________________________________________________________________________ */
/*!
	\brief GetEditorMousePos

	Retrieves the mouse position within the editor's viewport.

	\return Returns an `EditorMousePosition` struct, which includes both the mouse position and the editor viewport size.
*/
EditorMousePosition InputSystem::GetEditorMousePos() const {
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

/*  _________________________________________________________________________ */
/*!
	\brief InputListener

	Handles input events by updating the state of keys and mouse buttons based on the event data received.

	\param event The event data containing input state changes.
*/
void InputSystem::InputListener(Event& event)
{
	KeyState press {event.GetParam<KeyState>(Events::Window::Input::KEY_PRESS)};
	if(!event.GetFail()) mButtonsPressed |= press;
	KeyState click {event.GetParam<KeyState>(Events::Window::Input::KEY_CLICK)};
	if (!event.GetFail()) 
		mButtonsClicked |= click;
	KeyState release {event.GetParam<KeyState>(Events::Window::Input::KEY_RELEASE)};
	if (!event.GetFail()) mButtonsReleased |= release;

	MouseKeyState mspress {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_PRESS)};
	if (!event.GetFail())	mMouseButtonsPressed |= mspress;
	MouseKeyState msclick {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_CLICK)};
	if (!event.GetFail()) mMouseButtonsClicked |= msclick;
	MouseKeyState msrelease {event.GetParam<MouseKeyState>(Events::Window::Input::MOUSE_RELEASE)};
	if (!event.GetFail()) mMouseButtonsReleased |= msrelease;

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
