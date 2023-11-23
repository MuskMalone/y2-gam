#include "../include/pch.hpp"

#include "Systems/InputSystem.hpp"

#include <Core/Globals.hpp>
#include "Core/Coordinator.hpp"
#include "Systems/UISystem.hpp"
#include "Systems/RenderSystem.hpp"

#include "Components/UIImage.hpp"
#include "../WindowManager.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*!
\brief Init Function

Initializes the UISystem. This function is designed to set up any necessary components or state
required by the UISystem. Currently, the function body is empty, indicating no initialization logic is present.

@return none.
*/
void UISystem::Init() {

}

/*  _________________________________________________________________________ */
/*!
\brief Update Function

Performs the update logic for the UISystem. It involves processing user input,
updating the state of UI elements, and handling interactions like mouse hover and clicks on UI buttons.
The function calculates the mouse position in world space and checks if the mouse is over any UI button,
updating their hover and click status accordingly.

@return none.
*/
void UISystem::Update() {
    auto const& inputSystem = Coordinator::GetInstance()->GetSystem<InputSystem>();
    auto const& renderSystem = Coordinator::GetInstance()->GetSystem<RenderSystem>();
    auto const& windowManager = WindowManager::GetInstance();

    int windowWidth = windowManager->GetWidth();
    int windowHeight = windowManager->GetHeight();

    MousePosition mp{ inputSystem->GetMousePos() };

    // Normalize mouse position to NDC
    Vec2 mousePos{
        (2.0f * mp.first / static_cast<float>(windowWidth)) - 1.0f,
        1.0f - (2.0f * mp.second / static_cast<float>(windowHeight))
    };


    // Transform mouse position using the UI camera
    Camera const& uiCamera = Coordinator::GetInstance()->GetComponent<Camera>(renderSystem->GetUICamera());
    glm::mat4 inverseViewProj = glm::inverse(uiCamera.GetViewProjMtx());
    glm::vec4 mouseClipSpace = glm::vec4(mousePos.x, mousePos.y, 0.0f, 1.0f);
    glm::vec4 mouseWorldSpace = inverseViewProj * mouseClipSpace;
    mouseWorldSpace /= mouseWorldSpace.w;
    mousePos = Vec2(mouseWorldSpace.x, mouseWorldSpace.y);

    bool isLeftMouseClicked = inputSystem->CheckKey(InputSystem::InputKeyState::MOUSE_CLICKED, static_cast<size_t>(MouseButtons::LB));

    for (auto const& entity : mEntities) {
        if (! Coordinator::GetInstance()->HasComponent<UIImage>(entity)) continue;
        auto& button = Coordinator::GetInstance()->GetComponent<UIImage>(entity);
        if (button.enabled) {
            auto& transform = Coordinator::GetInstance()->GetComponent<Transform>(entity);
            if (IsMouseOverButton(mousePos, transform)) {
                button.isHover = true;
                if (isLeftMouseClicked) {
                    //std::cout << "Button clicked" << std::endl;
                    button.isClicked = true;
                }
                
                // You can also handle hover state here, if needed
                //std::cout << "Hovering" << std::endl;
            }
            else {
              button.isHover = false;
              button.isClicked = false;
            }       
        }
    }
}

/*  _________________________________________________________________________ */
/*!
\brief Exit Function

Handles the cleanup or shutdown procedures for the UISystem.
This function is intended for deallocating resources or performing any necessary cleanup operations
when the UISystem is no longer needed. Currently, the function body is empty, indicating no exit logic is present.

@return none.
*/
void UISystem::Exit() {

}

/*  _________________________________________________________________________ */
/*!
\brief IsMouseOverButton Function

Determines if the mouse is hovering over a given UI button.
The function calculates the bounding box of the button and checks if the mouse position falls within this area.

\param mousePos A Vec2 structure representing the current mouse position.
\param transform A Transform structure representing the transform of the UI button.

@return A boolean value indicating whether the mouse is over the button.
*/
bool UISystem::IsMouseOverButton(Vec2 const& mousePos, Transform const& transform) {
    Vec2 halfSize = Vec2(transform.scale.x / 2, transform.scale.y / 2);
    Vec2 topLeft = Vec2(transform.position.x - halfSize.x, transform.position.y - halfSize.y);
    Vec2 bottomRight = Vec2(transform.position.x + halfSize.x, transform.position.y + halfSize.y);

    return (mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x &&
        mousePos.y >= topLeft.y && mousePos.y <= bottomRight.y);
}