#include "../include/pch.hpp"

#include "Systems/TextSystem.hpp"
#include <Core/Globals.hpp>
#include "Core/Coordinator.hpp"
#include "Graphics/FontRenderer.hpp"

#include "Components/Transform.hpp"
#include "Components/Text.hpp" 

namespace {
	std::shared_ptr<Coordinator> gCoordinator;
}

void TextSystem::Init() {
	Image::FontRenderer::Init();
	Image::FontRenderer::LoadFont("../assets/fonts/arial/arial.ttf", "Arial");
	Image::FontRenderer::SetFontSize("Arial", 100);
	Image::FontRenderer::GenerateBitmap("Arial", 100);

	Image::FontRenderer::LoadFont("../assets/fonts/lato/Lato-Bold.ttf", "Lato");
	Image::FontRenderer::SetFontSize("Lato", 100);
	Image::FontRenderer::GenerateBitmap("Lato", 100);

	Image::FontRenderer::LoadFont("../assets/fonts/getho/GethoLight-7Gal.ttf", "Getho");
	Image::FontRenderer::SetFontSize("Getho", 100);
	Image::FontRenderer::GenerateBitmap("Getho", 100);
}

void TextSystem::Update() {
	for (auto const& entity : mEntities) {
		auto const& textToPrint{ Coordinator::GetInstance()->GetComponent<Text>(entity) };
		auto const& trans{ Coordinator::GetInstance()->GetComponent<Transform>(entity) };
		Image::FontRenderer::RenderText(textToPrint.fontName, textToPrint.text, trans.position.x, trans.position.y,
			textToPrint.scale, textToPrint.color);
	}
}

void TextSystem::Exit() {
	Image::FontRenderer::Exit();
}

