#include "../include/pch.hpp"
#include <Engine/PrefabsManager.hpp>
#include <Core/Coordinator.hpp>
#include <Systems/InputSystem.hpp>
namespace {
    bool gSelected = false;
    PrefabsManager::PrefabID gSelectedPrefab{static_cast<PrefabsManager::PrefabID>(-1)};
}
Entity const& GetSelectedPrefabEntryEntity() {
    if (gSelectedPrefab == static_cast<PrefabsManager::PrefabID>(-1)) {
        return MAX_ENTITIES;
    }
    return PrefabsManager::GetInstance()->GetEntityFactory().at(gSelectedPrefab).entity;
}
void PrefabsAssetWindow(std::set<Entity> const&) {
    auto inputSystem{ Coordinator::GetInstance()->GetSystem<InputSystem>() };
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
    static char namebuffer[2048] = "";
    ImGui::InputText("##unique_id", namebuffer, IM_ARRAYSIZE(namebuffer));
    ImGui::SameLine();
    if (ImGui::Button("New Prefab")) {
        std::string prefabName{namebuffer};
        if (prefabName.empty()) prefabName = "NewPrefab";
        PrefabsManager::GetInstance()->AddPrefab(prefabName);
    }
    ImGui::Columns(columnCount, 0, false);

    for (auto const& prefab : PrefabsManager::GetInstance()->GetEntityFactory()) {
        ImGui::PushID(static_cast<int>(prefab.first)); //assetid
        std::shared_ptr<Texture> icon = fileIcon;
        if (prefab.first == ::gSelectedPrefab) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f)); // Highlight color (Blue)
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Normal color (Transparent)
        }
        ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(icon->GetTexHdl())), { size, size }, { 0, 1 }, { 1, 0 });

        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("PrefabsInstance", &gSelectedPrefab, sizeof(PrefabsManager::PrefabID));
            ImGui::Text("%s", ("Add " + prefab.second.name).c_str());
            ImGui::EndDragDropSource();
        }

        ImGui::PopStyleColor();
        //PrefabsManager::PrefabID pid{ prefab.first };

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
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_DELETE)) {
        PrefabsManager::GetInstance()->DeletePrefab(gSelectedPrefab);
        gSelectedPrefab = static_cast<PrefabsManager::PrefabID>(-1);
    }
}