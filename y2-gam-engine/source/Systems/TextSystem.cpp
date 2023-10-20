#include "../include/pch.hpp"

#include "Systems/TextSystem.hpp"
#include "Systems/RenderSystem.hpp"

#include <Core/Globals.hpp>
#include "Core/Coordinator.hpp"
#include "Graphics/FontRenderer.hpp"

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
		Vec2 screenCoords{ WorldToScreenCoordinates(Vec2(trans.position.x, trans.position.y)) };
		Image::FontRenderer::RenderText(textToPrint.fontName, textToPrint.text, screenCoords.x, screenCoords.y,
			textToPrint.scale, textToPrint.color);
	}
}

void TextSystem::Exit() {
	Image::FontRenderer::Exit();
}

Vec2 TextSystem::WorldToScreenCoordinates(Vec2 worldCoordinates) {
	auto& cam{ Coordinator::GetInstance()->GetComponent<OrthoCamera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };
	glm::vec4 screenCoordinates{ cam.GetViewMtx() * glm::vec4(worldCoordinates.x, worldCoordinates.y, 0.f, 1.f) };
	return Vec2{ screenCoordinates.x, screenCoordinates.y };
}

