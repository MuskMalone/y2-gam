#include "../include/pch.hpp"
#include <Engine/PrefabsManager.hpp>

namespace {
    bool gSelected = false;
    PrefabsManager::PrefabID gSelectedPrefab{static_cast<PrefabsManager::PrefabID>(-1)};
}
Entity const& GetSelectedPrefabEntryEntity() {
    if (gSelectedPrefab == static_cast<PrefabsManager::PrefabID>(-1)) {
        return PrefabsManager::GetInstance()->GetEntityFactory().begin()->second.entity;
    }
    return PrefabsManager::GetInstance()->GetEntityFactory().at(gSelectedPrefab).entity;
}
void PrefabsAssetWindow(std::set<Entity> const&) {
    ImGui::BeginChild("Prefabs");
    static float padding = 15.f;
    static float size = 95.f;
    static float iconSize = padding + size;
    static std::shared_ptr<Texture> fileIcon = Texture::Create("../Icon/PrefabsIcon.png");
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = static_cast<int>(panelWidth / iconSize);
    if (columnCount < 1) {
        columnCount = 1;
    }

    ImGui::Columns(columnCount, 0, false);

    for (auto const& prefab : PrefabsManager::GetInstance()->GetEntityFactory()) {
        ImGui::PushID(static_cast<int>(prefab.first)); //assetid
        std::shared_ptr<Texture> icon = fileIcon;
        ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
        ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(icon->GetTexHdl())), { size, size }, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleColor();
        PrefabsManager::PrefabID pid{ prefab.first };

        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            //add actions
            ::gSelectedPrefab = prefab.first;
        }


        ImGui::TextWrapped(prefab.second.name.c_str());
        ImGui::NextColumn();
        ImGui::PopID();
        // ImGui::SameLine();
    }
    ImGui::EndChild();
}