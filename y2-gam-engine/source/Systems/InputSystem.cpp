/******************************************************************************/
/*!
\par        Image Engine
\file       InputSystem.cpp

\author     
\date       August 8, 2023

\brief      The source file for the InputSystem.

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
\brief Init Function

Initializes the InputSystem. This function sets up the InputSystem by registering it with the global Coordinator instance
and adding an event listener for input events.

@return none.
*/
void InputSystem::Init()
{
	::gCoordinator = Coordinator::GetInstance();
	::gCoordinator->AddEventListener(METHOD_LISTENER(Events::Window::INPUT, InputSystem::InputListener));
}

/*  _________________________________________________________________________ */
/*!
\brief CheckKey Function

Checks the state of a specific key or mouse button. This function allows querying various states like pressed, clicked, or released
for both keyboard keys and mouse buttons.

\param state The state to check for (e.g., KEY_PRESSED, MOUSE_CLICKED).
\param key The key or button code to check.

@return Boolean indicating whether the specified key is in the given state.
*/
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

/*  _________________________________________________________________________ */
/*!
\brief GetMousePos Function

Gets the current position of the mouse cursor in screen coordinates.

@return MousePosition structure containing the x and y coordinates of the mouse cursor.
*/
MousePosition InputSystem::GetMousePos() const {
	return mMousePos;
}

/*  _________________________________________________________________________ */
/*!
\brief GetWorldMousePos Function

Calculates the mouse position in world coordinates, using the editor camera's view and projection matrices.

@return MousePosition structure containing the x and y world coordinates of the mouse cursor.
*/
MousePosition InputSystem::GetWorldMousePos() const {
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
\brief GetSceneMousePos Function

Calculates the mouse position in the scene's world coordinates, using the scene camera's view and projection matrices.

@return MousePosition structure containing the x and y world coordinates of the mouse cursor in the scene.
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
\brief GetEditorMousePos Function

Gets the current position and size of the editor window's mouse cursor.

@return EditorMousePosition structure containing the position and size data of the mouse cursor in the editor.
*/
EditorMousePosition InputSystem::GetEditorMousePos() const {
	return mEditorMousePos;
}

/*  _________________________________________________________________________ */
/*!
\brief Update Function

Resets the states of all buttons and mouse buttons. This function is typically called at the beginning of a new frame
to reset the state of the input system.

@return none.
*/
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
\brief InputListener Function

Event listener for input events. This function updates the state of the InputSystem based on the received input events.
It processes keyboard and mouse events, updating the relevant states for keys and mouse buttons.

\param event The event containing input data.

@return none.
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
