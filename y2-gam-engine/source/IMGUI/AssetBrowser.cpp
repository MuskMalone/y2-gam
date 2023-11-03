#include "../include/pch.hpp"
#include <Engine/AssetManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Audio/Sound.hpp>
#include "IMGUI/AssetBrowser.hpp"

namespace {
    std::pair<AssetID, AssetManager::Asset> gSelectedAsset;
    
}
std::string GetFileExtension(const std::string& filePath) {
    // Find the last dot character
    std::size_t dotPos = filePath.rfind('.');
    if (dotPos != std::string::npos && dotPos != filePath.length() - 1) {
        // Find the last directory separator
        std::size_t sepPos = filePath.find_last_of("/\\");
        if (sepPos == std::string::npos || sepPos < dotPos) {
            // Extract the substring from the dot to the end
            return filePath.substr(dotPos + 1);

 
        }
    }
    return ""; // No extension found
}

template <typename _system>
void AssetContents(std::string const& systemName) {

    static float padding = 15.f;
    static float size = 95.f;
    static float iconSize = padding + size;
    static std::shared_ptr<Texture> fileIcon = Texture::Create("../Icon/FileIcon.png");
    static std::shared_ptr<Texture> addIcon = Texture::Create("../Icon/PlusIcon.png");
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = static_cast<int>(panelWidth / iconSize);
    if (columnCount < 1) {
        columnCount = 1;
    }
    ImGui::Columns(columnCount, 0, false);

    ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(addIcon->GetTexHdl())), { size, size }, { 0, 1 }, { 1, 0 });

    ImGui::NextColumn();
    if (ImGui::BeginDragDropTarget()) {
        std::cout << "Began drag-drop target." << std::endl;

        if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Content Asset Browser")) {
            //std::cout << "Accepted payload." << std::endl;

            const wchar_t* payLoadPath = (const wchar_t*)dragDropPayLoad->Data;
            std::filesystem::path basePath {"../assets"};
            std::string path{(basePath / payLoadPath).string()};
            std::cout << path << std::endl;
            AssetManager::GetInstance()->AddAsset<_system>(path);

        }
        ImGui::EndDragDropTarget();
    }


    for (auto const& asset : AssetManager::GetInstance()->GetAllAssets()) {
        //extremely inefficient for large amounts of assets
        if (asset.second.systemType.find(systemName) != std::string::npos) {
            ImGui::PushID(asset.first); //assetid
            std::shared_ptr<Texture> icon = fileIcon;
            ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
            ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(icon->GetTexHdl())), { size, size }, { 0, 1 }, { 1, 0 });
            ImGui::PopStyleColor();
            AssetID aid{ asset.first };
            if (ImGui::BeginDragDropSource()) {
                ImGui::SetDragDropPayload((systemName + " AssetBrowser").c_str(), &aid, sizeof(aid));
                ImGui::Text("%s", asset.second.path.c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
                //add actions
                ::gSelectedAsset = asset;
            }
            if (ImGui::BeginDragDropTarget()) {
                std::cout << "Began drag-drop target." << std::endl;

                if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Content Asset Browser")) {
                    //std::cout << "Accepted payload." << std::endl;

                    const wchar_t* payLoadPath = (const wchar_t*)dragDropPayLoad->Data;
                    std::filesystem::path basePath {"../assets"};
                    std::string path{(basePath / payLoadPath).string()};
                    std::cout << path << std::endl;
                    AssetManager::GetInstance()->ChangeAssetResource<_system>(aid, path);

                }
                ImGui::EndDragDropTarget();
            }

            ImGui::TextWrapped(asset.second.path.c_str());
            ImGui::NextColumn();
            ImGui::PopID();
            // ImGui::SameLine();
        }
    }

}
/*  _________________________________________________________________________ */
/*! AssetWindow

@param std::set<Entity>const& mEntities
 Set of entities to loop through

@return none.

This function change all the texture related to the asset ID
*/
void AnimationAssetWindow(std::set<Entity>const& mEntities) {
    ImGui::Begin("Animation Assets");
    AssetContents<AnimationManager>("Animation");

    ImGui::End();
}
void SpriteAssetWindow(std::set<Entity>const& mEntities) {
    ImGui::Begin("Sprite Assets");
    AssetContents<SpriteManager>("Sprite");
    ImGui::End();
}
void SoundAssetWindow(std::set<Entity>const& mEntities) {
    ImGui::Begin("Sound Assets");
    AssetContents<SoundManager>("Sound");
    ImGui::End();
}
bool AssetBrowserFindStr(std::string const& string, std::string const& substr) {
    return !(string.find(substr) == std::string::npos);
}
void AssetPropertiesWindow(std::set<Entity> const& mEntities){
    ImGui::Begin("Asset Properties");
    //displays path
    auto am(AssetManager::GetInstance());

    bool isSelected{};
    if (ImGui::Selectable(("Path: " + gSelectedAsset.second.path).c_str(), isSelected));
    if (ImGui::BeginDragDropTarget()) {
        std::cout << "Began drag-drop target." << std::endl;

        if (const ImGuiPayload* dragDropPayLoad = ImGui::AcceptDragDropPayload("Content Asset Browser")) {
            //std::cout << "Accepted payload." << std::endl;

            const wchar_t* payLoadPath = (const wchar_t*)dragDropPayLoad->Data;
            std::filesystem::path basePath {"../assets"};
            std::string path{(basePath / payLoadPath).string()};
            std::cout << path << std::endl;
            
        }
        ImGui::EndDragDropTarget();
    }

    //tch: should use some type reflection lib to fix this...
    if (AssetBrowserFindStr(gSelectedAsset.second.systemType, "Sound")) {
        auto& props{ am->GetAssetProperties<SoundManager>(gSelectedAsset.first) };
        const char* items[] = { "False", "True" };
        int currentItem = props.stream ? 1 : 0; // Convert bool to int for selection

        // The combo box
        if (ImGui::Combo("SoundStream", &currentItem, items, IM_ARRAYSIZE(items)))
        {
            props.stream = currentItem == 1; // Convert int back to bool
        }
    }
    else if (AssetBrowserFindStr(gSelectedAsset.second.systemType, "Sprite")) {
        auto& props{ am->GetAssetProperties<SpriteManager>(gSelectedAsset.first) };
        float width{ 70.f };
        ImGui::Text("Dimensions");
        // The first input float field
        ImGui::SetNextItemWidth(width); // Set the width to 100 pixels

        ImGui::InputFloat("Width", &(props.dim.x));
        ImGui::SameLine(); // This will place the next widget on the same line
        // The second input float field
        ImGui::SetNextItemWidth(width); // Set the width to 100 pixels

        ImGui::InputFloat("Height", &(props.dim.y));

        ImGui::Text("Index");
        // The first input float field
        ImGui::SetNextItemWidth(width); // Set the width to 100 pixels

        ImGui::InputFloat("X", &(props.idx.x));
        ImGui::SameLine(); // This will place the next widget on the same line
        // The second input float field
        ImGui::SetNextItemWidth(width); // Set the width to 100 pixels

        ImGui::InputFloat("Y", &(props.idx.y));
    }
    else if (AssetBrowserFindStr(gSelectedAsset.second.systemType, "Animation")) {
        auto& props{ am->GetAssetProperties<AnimationManager>(gSelectedAsset.first) };
        ImGui::InputInt("Frame Count", &(props.frameCount));
        float width{ 70.f };
        ImGui::Text("Dimensions");
        // The first input float field
        ImGui::SetNextItemWidth(width); // Set the width to 100 pixels

        ImGui::InputFloat("Width", &(props.dim.x));
        ImGui::SameLine(); // This will place the next widget on the same line
        // The second input float field
        ImGui::SetNextItemWidth(width); // Set the width to 100 pixels

        ImGui::InputFloat("Height", &(props.dim.y));
        ImGui::InputFloat("Index Y", &(props.idxCoordy));
    }

    // Calculate the remaining space and set the cursor to the bottom right
    float windowWidth = ImGui::GetWindowSize().x;
    float windowHeight = ImGui::GetWindowSize().y;
    float buttonWidth = windowWidth;  // Width of the button
    float buttonHeight = 30.0f;  // Height of the button
    ImGui::SetCursorPosX(windowWidth - buttonWidth - ImGui::GetStyle().WindowPadding.x); // Subtract the window padding to align it perfectly
    ImGui::SetCursorPosY(windowHeight - buttonHeight - ImGui::GetStyle().WindowPadding.y); // Subtract the window padding to align it perfectly

    // Place the button
    if (ImGui::Button("Save", ImVec2(buttonWidth, buttonHeight))) {
        if (AssetBrowserFindStr(gSelectedAsset.second.systemType, "Sound")) {
            auto& props{ am->GetAssetProperties<SoundManager>(gSelectedAsset.first) };
            am->SaveAsset<SoundManager>(gSelectedAsset.first, props);

        }
        else if (AssetBrowserFindStr(gSelectedAsset.second.systemType, "Sprite")) {
            auto& props{ am->GetAssetProperties<SpriteManager>(gSelectedAsset.first) };
            am->SaveAsset<SpriteManager>(gSelectedAsset.first, props);

        }
        else if (AssetBrowserFindStr(gSelectedAsset.second.systemType, "Animation")) {
            auto& props{ am->GetAssetProperties<AnimationManager>(gSelectedAsset.first) };
            am->SaveAsset<AnimationManager>(gSelectedAsset.first, props);
        }
    }

    ImGui::End();
}
