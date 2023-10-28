#include "../include/pch.hpp"
#include "Graphics/SpriteManager.hpp"

std::unordered_map<int, std::shared_ptr<Texture>> SpriteManager::textures;
std::unordered_map<int, std::shared_ptr<SubTexture>> SpriteManager::sprites;

int SpriteManager::LoadTexture(const std::string& path, int id) {
    if (textures.find(id) != textures.end()) {
        // handle error: ID already used
        return false;
    }

    auto texture = Texture::Create(path);
    textures[id] = texture;
    return id;
}

int SpriteManager::CreateSubTexture(int textureID, const glm::vec2& min, const glm::vec2& max, int id) {
    if (sprites.find(id) != sprites.end()) {
        // handle error: ID already used
        return false;
    }

    if (textures.find(textureID) == textures.end()) {
        // handle error: textureID not found
        return false;
    }

    auto sprite = SubTexture::Create(textures[textureID], min, max);
    sprites[id] = sprite;
    return id;
}

std::shared_ptr<SubTexture> SpriteManager::GetSprite(int spriteID) {
    return sprites[spriteID];
}

std::shared_ptr<Texture> SpriteManager::GetTexture(int textureID) {
    return textures[textureID];
}

void SpriteManager::UnloadTexture(int textureID) {
    textures.erase(textureID);
}

void SpriteManager::UnloadSprite(int spriteID) {
    sprites.erase(spriteID);
}