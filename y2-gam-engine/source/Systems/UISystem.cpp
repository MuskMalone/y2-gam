#include "../include/pch.hpp"

#include "Systems/InputSystem.hpp"

#include <Core/Globals.hpp>
#include "Core/Coordinator.hpp"
#include "Systems/UISystem.hpp"
#include "Systems/RenderSystem.hpp"
//#include "Systems/LayeringSystem.hpp"

#include "Components/UIImage.hpp"
#include "../WindowManager.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*! Init

@return none.

*/
void UISystem::Init() {

}

/*  _________________________________________________________________________ */
/*! Update

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
/*! Exit

@return none.

*/
void UISystem::Exit() {

}

bool UISystem::IsMouseOverButton(Vec2 const& mousePos, Transform const& transform) {
    Vec2 halfSize = Vec2(transform.scale.x / 2, transform.scale.y / 2);
    Vec2 topLeft = Vec2(transform.position.x - halfSize.x, transform.position.y - halfSize.y);
    Vec2 bottomRight = Vec2(transform.position.x + halfSize.x, transform.position.y + halfSize.y);

    return (mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x &&
        mousePos.y >= topLeft.y && mousePos.y <= bottomRight.y);
}