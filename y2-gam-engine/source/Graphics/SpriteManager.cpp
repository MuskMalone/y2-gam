/******************************************************************************/
/*!
\par        Image Engine
\file       SpriteManager.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       29 Oct 2023

\brief      Implementation file for the SpriteManager class.

            Contains the definitions for the SpriteManager class which manages
            textures and sprites. It provides functionalities for loading,
            retrieving, and unloading textures and sprites.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Graphics/SpriteManager.hpp"

std::unordered_map<ResourceID, std::shared_ptr<Texture>> SpriteManager::textures;
std::unordered_map<ResourceID, std::shared_ptr<SubTexture>> SpriteManager::sprites;

/*  _________________________________________________________________________ */
/*! LoadTexture

@param path
Path to the texture file.

@param id
The unique identifier assigned to the texture.

@return
The ID of the loaded texture or an error value (-1 if failed).

This function loads a texture from the provided path and associates it with
the given ID. If a texture with the given ID already exists or the texture
file cannot be loaded, the function will return an error value.
*/
ResourceID SpriteManager::LoadTexture(const std::string& path) {
    ResourceID id{ _hash(path) };
    if (textures.find(id) != textures.end()) {
        // handle error: ID already used
        return id;
    }

    auto texture = Texture::Create(path);
    textures[id] = texture;
    return id;
}

/*  _________________________________________________________________________ */
/*! CreateSubTexture

@param textureID
The ID of the main texture to create a subtexture from.

@param min
The bottom-left coordinates of the subtexture within the main texture.

@param max
The top-right coordinates of the subtexture within the main texture.

@param id
The unique identifier assigned to the subtexture.

@return
The ID of the created subtexture or an error value (-1 if failed).

This function creates a subtexture from the provided main texture using
the specified coordinates. The subtexture is associated with the provided ID.
If a subtexture with the given ID already exists or the main texture ID
is not found, the function will return an error value.
*/
//struct SpriteProperties{
//    int textureID;
//    glm::vec2& min; 
//    glm::vec2& max;
//
//}
ResourceID SpriteManager::CreateSubTexture(ResourceID textureID, SpriteProperties const& props) {
    if (sprites.find(props.id) != sprites.end()) {
        // handle error: ID already used
        return false;
    }

    if (textures.find(textureID) == textures.end()) {
        // handle error: textureID not found
        return false;
    }

    auto sprite = SubTexture::Create(textures[textureID], props.idx, props.dim);
    sprites[props.id] = sprite;
    return props.id;
}

//ResourceID SpriteManager::CreateSubTexture(ResourceID textureID, const glm::vec2& min, const glm::vec2& max, ResourceID id) {
//    if (sprites.find(id) != sprites.end()) {
//        // handle error: ID already used
//        return false;
//    }
//
//    if (textures.find(textureID) == textures.end()) {
//        // handle error: textureID not found
//        return false;
//    }
//
//    auto sprite = SubTexture::Create(textures[textureID], min, max);
//    sprites[id] = sprite;
//    return id;
//}

/*  _________________________________________________________________________ */
/*! GetSprite

@param spriteID
The ID of the desired sprite (subtexture).

@return
A shared pointer to the desired subtexture.

This function retrieves a shared pointer to the subtexture associated with
the provided sprite ID.
*/
std::shared_ptr<SubTexture> SpriteManager::GetSprite(ResourceID spriteID) {
    return sprites[spriteID];
}

/*  _________________________________________________________________________ */
/*! GetTexture

@param textureID
The ID of the desired texture.

@return
A shared pointer to the desired texture.

This function retrieves a shared pointer to the texture associated with
the provided texture ID.
*/
std::shared_ptr<Texture> SpriteManager::GetTexture(ResourceID textureID) {
    return textures[textureID];
}

/*_________________________________________________________________________* /
/*! UnloadTexture

@param textureID
The ID of the texture to be unloaded.

This function removes the texture associated with the provided texture ID
from the manager's storage, effectively unloading it.
*/
void SpriteManager::UnloadTexture(ResourceID textureID) {
    textures.erase(textureID);
}

/*  _________________________________________________________________________ */
/*! UnloadSprite

@param spriteID
The ID of the sprite (subtexture) to be unloaded.

This function removes the sprite associated with the provided sprite ID
from the manager's storage, effectively unloading it.
*/
void SpriteManager::UnloadSprite(ResourceID spriteID) {
    sprites.erase(spriteID);
}


ResourceID SpriteManager::LoadAsset(rapidjson::Value const& obj) {
    ResourceID texrid{ LoadTexture(obj["path"].GetString()) };
    ResourceID key{ obj["id"].GetUint64() };
    CreateSubTexture(texrid, SpriteProperties{
            key,
            glm::vec2{obj["minX"].GetFloat(), obj["minY"].GetFloat()},
            glm::vec2{obj["minX"].GetFloat(), obj["minY"].GetFloat()}
        });
    return key;
}
void SpriteManager::SaveAsset(ResourceID rid, SpriteProperties const& props, rapidjson::Value& obj) {

}
std::shared_ptr<SubTexture> const& SpriteManager::GetAsset(ResourceID rid) {
    return sprites[rid];
}

//SpriteProperties& SpriteManager::GetAssetProperties(ResourceID rid) {
//    return sprites[rid]->GetProperties();
//}
ResourceID SpriteManager::AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID rid) {
    auto sm{ Serializer::SerializationManager::GetInstance() };

    sm->InsertValue(obj, "minX", 0.0);
    sm->InsertValue(obj, "minY", 0.0);

    sm->InsertValue(obj, "maxX", 0.0);
    sm->InsertValue(obj, "maxY", 0.0);

    return rid;
}
