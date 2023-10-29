#include "../include/pch.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/SubTexture.hpp"

class SpriteManager {
public:
    static int LoadTexture(const std::string& path, int id);
    static int CreateSubTexture(int textureID, const glm::vec2& min, const glm::vec2& max, int id);

    static std::shared_ptr<SubTexture> GetSprite(int spriteID);
    static std::shared_ptr<Texture> GetTexture(int textureID);

    void static UnloadTexture(int textureID);
    void static UnloadSprite(int spriteID);

    //for assetmanager
    //static ResourceID LoadAsset(SoundProperties const& props);
    //static ResourceID LoadAsset(rapidjson::Value const& obj);
    //static void SaveAsset(ResourceID aid, SoundProperties const& props, rapidjson::Value& obj);
    //static Sound const& GetAsset(ResourceID);
    //static SoundProperties& GetAssetProperties(ResourceID);
    //static ResourceID AddAsset(rapidjson::Value& obj, std::string const& path);

private:
    static std::unordered_map<int, std::shared_ptr<Texture>> textures;
    static std::unordered_map<int, std::shared_ptr<SubTexture>> sprites;
};