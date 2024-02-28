/******************************************************************************/
/*!
\par        Image Engine
\file       AnimationSystem.hpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Oct 20, 2023

\brief      Header for the AnimationManager class.

            Manages animation, loading and creating of animations from spritesheets.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"


class SpriteManager {
public:
    static ResourceID LoadTexture(const std::string& path);

    static ResourceID CreateSubTexture(ResourceID textureID, SpriteProperties const&);

    static std::shared_ptr<SubTexture> const& GetSprite(ResourceID spriteID);
    static std::shared_ptr<Texture> GetTexture(ResourceID textureID);

    void static UnloadTexture(ResourceID textureID);
    void static UnloadSprite(ResourceID spriteID);

    //for assetmanager
    //loads subtextures
    static ResourceID LoadAsset(rapidjson::Value const& obj);
    static void UnloadAsset(AssetManager::Asset const& asset);
    static void SaveAsset(AssetID aid, SpriteProperties const& props, rapidjson::Value& obj);
    static std::shared_ptr<SubTexture> const& GetAsset(ResourceID);
    static SpriteProperties& GetAssetProperties(ResourceID);
    static ResourceID AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID);
    static ResourceID GetResourceID(std::string const& filename);

private:

    static std::unordered_map<ResourceID, std::shared_ptr<Texture>> textures;
    static std::unordered_map<ResourceID, std::shared_ptr<SubTexture>> sprites;
    static std::map<std::string, ResourceID> sSpriteResourceMap;
};