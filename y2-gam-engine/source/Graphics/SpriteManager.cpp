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

std::unordered_map<int, std::shared_ptr<Texture>> SpriteManager::textures;
std::unordered_map<int, std::shared_ptr<SubTexture>> SpriteManager::sprites;

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
int SpriteManager::LoadTexture(const std::string& path, int id) {
    if (textures.find(id) != textures.end()) {
        // handle error: ID already used
        return false;
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
//ResourceID SpriteManager::CreateSubTexture(std::string const& path, SpriteProperties const& props) {
//    //if texture cannot be found
//    //add texture
//    //use texture
//}

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

/*  _________________________________________________________________________ */
/*! GetSprite

@param spriteID
The ID of the desired sprite (subtexture).

@return
A shared pointer to the desired subtexture.

This function retrieves a shared pointer to the subtexture associated with
the provided sprite ID.
*/
std::shared_ptr<SubTexture> SpriteManager::GetSprite(int spriteID) {
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
std::shared_ptr<Texture> SpriteManager::GetTexture(int textureID) {
    return textures[textureID];
}

/*_________________________________________________________________________* /
/*! UnloadTexture

@param textureID
The ID of the texture to be unloaded.

This function removes the texture associated with the provided texture ID
from the manager's storage, effectively unloading it.
*/
void SpriteManager::UnloadTexture(int textureID) {
    textures.erase(textureID);
}

/*  _________________________________________________________________________ */
/*! UnloadSprite

@param spriteID
The ID of the sprite (subtexture) to be unloaded.

This function removes the sprite associated with the provided sprite ID
from the manager's storage, effectively unloading it.
*/
void SpriteManager::UnloadSprite(int spriteID) {
    sprites.erase(spriteID);
}