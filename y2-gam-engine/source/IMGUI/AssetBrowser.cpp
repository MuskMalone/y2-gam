/**
 * @file AssetBrowser.cpp
 * @author tan cheng hian (t.chenghian@digipen.edu)
 * @brief 
 * @version 0.1
 * @date 2023-11-03
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../include/pch.hpp"
#include <Engine/AssetManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Audio/Sound.hpp>
#include "IMGUI/AssetBrowser.hpp"
#include <Core/Coordinator.hpp>
#include <Systems/InputSystem.hpp>

namespace {
    std::pair<AssetID, AssetManager::Asset> gSelectedAsset;
    
}
/**
 * @brief Fetches the extension of the given file path.
 *
 * @param filePath The full file path from which to extract the file extension.
 * @return The file extension, or an empty string if one was not found.
 */

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
/**
 * @brief Generates the contents for a panel displaying assets of a specific system.
 *
 * @param systemName The name of the system for which to generate asset content.
 */
template <typename _system>
void AssetContents(std::string const& systemName) {
    ImGui::BeginChild(systemName.c_str());
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

    for (auto const& asset : AssetManager::GetInstance()->GetAllAssets()) {
        //extremely inefficient for large amounts of assets
        if (asset.second.systemType.find(systemName) != std::string::npos) {
            ImGui::PushID(static_cast<int>(asset.first)); //assetid
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
    ImGui::EndChild();

    if (ImGui::BeginDragDropTarget()) {

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
}

/*  _________________________________________________________________________ */
/*! AssetWindow

@param std::set<Entity>const& mEntities
 Set of entities to loop through

@return none.

This function change all the texture related to the asset ID
*/
void AnimationAssetWindow(std::set<Entity>const& mEntities) {
    AssetContents<AnimationManager>("Animation");
}
void SpriteAssetWindow(std::set<Entity>const& mEntities) {
    AssetContents<SpriteManager>("Sprite");
}
void SoundAssetWindow(std::set<Entity>const& mEntities) {
    AssetContents<SoundManager>("Sound");
}
void SceneAssetWindow(std::set<Entity> const& mEntities) {
    ImGui::BeginChild("SceneBrowser");
    static std::shared_ptr<Texture> directroyIcon = Texture::Create("../Icon/DirectoryIcon.png");
    static std::shared_ptr<Texture> fileIcon = Texture::Create("../Icon/FileIcon.png");
    static std::filesystem::path assetDirectory{ "Data/scenes" };
    static std::filesystem::path currentDirectory{ assetDirectory };
    if (currentDirectory != std::filesystem::path(assetDirectory)) {
        if (ImGui::Button("Back")) {
            currentDirectory = currentDirectory.parent_path();
        }

    }
    static char namebuffer[2048] = "";
    ImGui::InputText("##unique_id", namebuffer, IM_ARRAYSIZE(namebuffer));
    ImGui::SameLine();
    if (ImGui::Button("New Scene")) {
        std::string sceneName{namebuffer};
        std::ofstream newScene{(assetDirectory / sceneName).string() + ".json"};
        if (newScene) {
            newScene.close();
        }
    }
    static float padding = 15.f;
    static float size = 95.f;
    static float iconSize = padding + size;
    float panelWidth = ImGui::GetContentRegionAvail().x;
    int columnCount = static_cast<int>(panelWidth / iconSize);
    if (columnCount < 1) {
        columnCount = 1;
    }
    ImGui::Columns(columnCount, 0, false);
    for (auto& directoryPath : std::filesystem::directory_iterator(currentDirectory)) {
        auto const& path = directoryPath.path();
        auto relativePath = std::filesystem::relative(path, assetDirectory);
        std::string filenameString = relativePath.filename().string();
        auto fileName = directoryPath.path().filename().string();
        ImGui::PushID(filenameString.c_str());
        std::shared_ptr<Texture> icon = directoryPath.is_directory() ? directroyIcon : fileIcon;
        ImGui::PushStyleColor(ImGuiCol_Button, { 0,0,0,0 });
        ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uintptr_t>(icon->GetTexHdl())), { size, size }, { 0, 1 }, { 1, 0 });
        ImGui::PopStyleColor();
        if (directoryPath.is_regular_file()) {
            if (ImGui::BeginDragDropSource()) {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("SceneBrowser", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::Text("%s", filenameString.c_str());
                ImGui::EndDragDropSource();
            }
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) {
            if (directoryPath.is_directory()) {
                currentDirectory /= path.filename();

            }
        }
        ImGui::TextWrapped(filenameString.c_str());
        ImGui::NextColumn();
        ImGui::PopID();
    }

    ImGui::Columns(1);// go back to default
    ImGui::EndChild();
}
namespace {
    std::vector< std::pair<std::string, std::function<void(std::set<Entity>const&)>>> gAssetwindows{
        {"Animations", AnimationAssetWindow}, 
        { "Sprites", SpriteAssetWindow }, 
        { "Sounds", SoundAssetWindow }, 
        { "Scenes", SceneAssetWindow }
    };
}

void AssetWindow(std::set<Entity>const& mEntities) {
    // Global or static variable
    static int activeChild = 0;

    // Inside your rendering loop
    ImGui::Begin("Asset");
    // Get the available width of the window
    float totalWidth = ImGui::GetContentRegionAvail().x;

    // Create 2 columns
    ImGui::Columns(2, nullptr, false);

    // Set the width of the first column to be 20% of the total width
    ImGui::SetColumnWidth(0, totalWidth * 0.15f);
    for (int i{}; i < gAssetwindows.size(); ++i) {
        // Check if the button is the active one and push the style color
        bool isButtonActive = i == activeChild;
        if (isButtonActive) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.5f, 0.0f, 1.0f)); // Example: Orange color for active button
        }

        // Create the button
        if (ImGui::Button(gAssetwindows[i].first.c_str(), ImVec2(totalWidth * 0.12f, 20))) {
            activeChild = i;
        }

        // Only pop the style color if it was pushed
        if (isButtonActive) {
            ImGui::PopStyleColor();
        }
    }

    ImGui::NextColumn();

    gAssetwindows[activeChild].second(mEntities);
    ImGui::Columns(1);
    ImGui::End();

}
/**
 * @brief Checks if a string is found within another string.
 *
 * @param string The main string to search within.
 * @param substr The substring to look for in the main string.
 * @return true if substr was found in string, false otherwise.
 */

bool AssetBrowserFindStr(std::string const& string, std::string const& substr) {
    return !(string.find(substr) == std::string::npos);
}
void AssetPropertiesWindow(std::set<Entity> const& mEntities){
    ImGui::Begin("Asset Properties");
    //displays path
    auto am(AssetManager::GetInstance());

    bool isSelected{};
    if (ImGui::Selectable(("Path: " + gSelectedAsset.second.path).c_str(), isSelected)) {}
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
