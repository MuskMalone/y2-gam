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
ResourceID SpriteManager::CreateSubTexture(ResourceID textureID, SpriteProperties const& props) {
    if (sprites.find(props.id) != sprites.end()) {
        // handle error: ID already used
        return props.id;
    }

    if (textures.find(textureID) == textures.end()) {
        // handle error: textureID not found
        return -1;
    }

    auto sprite = SubTexture::Create(textures[textureID], props.idx, props.dim, props);
    sprites[props.id] = sprite;
    return props.id;
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
std::shared_ptr<SubTexture> const& SpriteManager::GetSprite(ResourceID spriteID) {
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

/*  _________________________________________________________________________ */
/*!
\brief Loads a sprite asset from a JSON object.

@param obj
The JSON object containing the sprite asset data.

@return
Returns the ResourceID of the loaded sprite asset.

This function reads sprite properties such as path, index coordinates, and dimensions from a JSON object, loads the texture, and creates a subtexture based on the given properties.
*/
ResourceID SpriteManager::LoadAsset(rapidjson::Value const& obj) {
    ResourceID texrid{ LoadTexture(obj["path"].GetString()) };
    ResourceID key{ obj["id"].GetUint64() };
    CreateSubTexture(texrid, SpriteProperties{
            key,
            glm::vec2{obj["idxX"].GetFloat(), obj["idxY"].GetFloat()},
            glm::vec2{obj["dimX"].GetFloat(), obj["dimY"].GetFloat()}
        });
    return key;
}

/*  _________________________________________________________________________ */
/*!
\brief Saves the sprite properties to a JSON object.

@param rid
The unique identifier for the sprite asset.

@param props
The properties of the sprite to be saved.

@param obj
The JSON object where the sprite properties will be saved.

This function serializes the sprite properties and saves them to a specified JSON object.
*/
void SpriteManager::SaveAsset(ResourceID rid, SpriteProperties const& props, rapidjson::Value& obj) {
    auto sm{ Serializer::SerializationManager::GetInstance() };

    sm->ModifyValue(obj, "idxX", props.idx.x);
    sm->ModifyValue(obj, "idxY", props.idx.y);

    sm->ModifyValue(obj, "dimX", props.dim.x);
    sm->ModifyValue(obj, "dimY", props.dim.y);
}

/*  _________________________________________________________________________ */
/*!
\brief Retrieves the subtexture for a given sprite asset.

@param rid
The unique identifier for the sprite asset.

@return
Returns a constant reference to the SubTexture associated with the given ResourceID.

This function fetches the subtexture corresponding to the specified sprite asset ID.
*/
std::shared_ptr<SubTexture> const& SpriteManager::GetAsset(ResourceID rid) {
    return GetSprite(rid);
}


/*  _________________________________________________________________________ */
/*!
\brief Retrieves the properties of a given sprite asset.

@param rid
The unique identifier for the sprite asset.

@return
Returns a reference to the SpriteProperties associated with the given ResourceID.

This function fetches the properties of the sprite asset corresponding to the specified ID.
*/
SpriteProperties& SpriteManager::GetAssetProperties(ResourceID rid) {
    return sprites[rid]->GetProperties();
}

/*  _________________________________________________________________________ */
/*!
\brief Adds a new sprite asset to the manager.

@param obj
The JSON object where the new sprite asset data will be inserted.

@param path
The file path to the sprite resource.

@param rid
The unique identifier for the new sprite asset.

@return
Returns the ResourceID of the added sprite asset.

This function inserts default sprite properties into a JSON object and returns the ResourceID of the new sprite asset.
*/
ResourceID SpriteManager::AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID rid) {
    auto sm{ Serializer::SerializationManager::GetInstance() };

    sm->InsertValue(obj, "idxX", 0.0);
    sm->InsertValue(obj, "idxY", 0.0);

    sm->InsertValue(obj, "dimX", 0.0);
    sm->InsertValue(obj, "dimY", 0.0);

    return rid;
}
