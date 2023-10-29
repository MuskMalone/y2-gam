#include "../include/pch.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"


class SpriteManager {
public:
    static ResourceID LoadTexture(const std::string& path);

    static ResourceID CreateSubTexture(ResourceID textureID, SpriteProperties const&);

    static std::shared_ptr<SubTexture> GetSprite(ResourceID spriteID);
    static std::shared_ptr<Texture> GetTexture(ResourceID textureID);

    void static UnloadTexture(ResourceID textureID);
    void static UnloadSprite(ResourceID spriteID);

    //for assetmanager
    //loads subtextures
    static ResourceID LoadAsset(rapidjson::Value const& obj);
    static void SaveAsset(ResourceID aid, SpriteProperties const& props, rapidjson::Value& obj);
    static std::shared_ptr<SubTexture> const& GetAsset(ResourceID);
    static SpriteProperties& GetAssetProperties(ResourceID);
    static ResourceID AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID);

private:

    static std::unordered_map<ResourceID, std::shared_ptr<Texture>> textures;
    static std::unordered_map<ResourceID, std::shared_ptr<SubTexture>> sprites;
};