/******************************************************************************/
/*!
\par        Image Engine
\file       TextSystem.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 23, 2023

\brief      The source file for the text system.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
						or disclosure of this file or its contents without the prior
						written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/


#include "../include/pch.hpp"

#include "Systems/TextSystem.hpp"
#include "Systems/RenderSystem.hpp"

#include <Core/Globals.hpp>
#include "Core/Coordinator.hpp"
#include "Graphics/FontRenderer.hpp"
#include "Systems/RenderSystem.hpp"

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

/*  _________________________________________________________________________ */
/*! Init

@return none.

Initializes the font renderer and loads the fonts, for future use.
*/
void TextSystem::Init() {
	Image::FontRenderer::Init();
	Image::FontRenderer::LoadFont("../assets/fonts/arial/arial.ttf", "Arial");
	Image::FontRenderer::SetFontSize("Arial");
	Image::FontRenderer::GenerateBitmap("Arial");

	Image::FontRenderer::LoadFont("../assets/fonts/lato/Lato-Bold.ttf", "Lato");
	Image::FontRenderer::SetFontSize("Lato");
	Image::FontRenderer::GenerateBitmap("Lato");

	Image::FontRenderer::LoadFont("../assets/fonts/getho/GethoLight-7Gal.ttf", "Getho");
	Image::FontRenderer::SetFontSize("Getho");
	Image::FontRenderer::GenerateBitmap("Getho");
}

/*  _________________________________________________________________________ */
/*! Update

@return none.

Updates the text system by rendering the text.
*/
void TextSystem::Update() {
	for (auto const& entity : mEntities) {
		auto const& textToPrint{ Coordinator::GetInstance()->GetComponent<Text>(entity) };
		auto const& trans{ Coordinator::GetInstance()->GetComponent<Transform>(entity) };
		Vec2 screenCoords{ WorldToScreenCoordinates(Vec2(trans.position.x, trans.position.y)) };
		float lengthOfText{ Image::FontRenderer::GetTextWidth(textToPrint.fontName, textToPrint.text, textToPrint.scale) };
		Image::FontRenderer::RenderText(textToPrint.fontName, textToPrint.text, screenCoords.x - (lengthOfText / 2.f), screenCoords.y,
			textToPrint.scale, textToPrint.color);
	}
}

/*  _________________________________________________________________________ */
/*! Exit

@return none.

A wrapper function for the font renderer's exit function.
*/
void TextSystem::Exit() {
	Image::FontRenderer::Exit();
}

/*  _________________________________________________________________________ */
/*! WorldToScreenCoordinates

@param worldCoordinates
The world coordinates to convert to screen coordinates.

@return Vec2
The screen coordinates.

Calculates the screen coordinates from the world coordinates.
*/
Vec2 TextSystem::WorldToScreenCoordinates(Vec2 worldCoordinates) {
	auto& cam{ Coordinator::GetInstance()->GetComponent<Camera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };
	glm::vec4 screenCoordinates{ cam.GetViewMtx() * glm::vec4(worldCoordinates.x, worldCoordinates.y, 0.f, 1.f) };
	return Vec2{ screenCoordinates.x, screenCoordinates.y };
}

