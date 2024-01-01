/******************************************************************************/
/*!
\par        Image Engine
\file       AnimationManager.cpp

\author     Xavier Choa (k.choa@digipen.edu)
\date       Oct 29, 2023

\brief      File implementation for the AnimationManager class.

			Manages animation, loading and creating of animations from spritesheets.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
			or disclosure of this file or its contents without the prior
			written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "Graphics/AnimationManager.hpp"
#include "Graphics/SpriteManager.hpp"
std::unordered_map<ResourceID, AnimationProperties> AnimationManager::mAnimationFrameLists;


/*  _________________________________________________________________________ */
/*!
\brief Loads an animation from a given path and creates animation frames.

@param path
The file path to the animation resource.

@param rid
The unique identifier for the animation resource.

@param frameCount
The total number of frames in the animation.

@param idxCoordy
The y-coordinate index of the animation frames.

@param dim
The dimensions (width and height) of each animation frame.

@return
Returns the ResourceID of the loaded animation.

This function loads a texture from the specified path, creates animation frames based on the given parameters, and stores the animation properties in the manager.
*/
ResourceID AnimationManager::LoadAnimation(std::string const& path, ResourceID rid, int frameCount, float idxCoordy, glm::vec2 const& dim) {
	//if (mAnimationFrameLists.find(rid) != mAnimationFrameLists.end()) return rid;
	ResourceID texID = SpriteManager::LoadTexture(path);
	AnimationProperties ap{};
	ap.path = path;
	ap.id = rid;
	ap.frameCount = frameCount;
	ap.idxCoordy = idxCoordy;
	ap.dim = dim;
	static uint32_t animframeID = 1;

	for (float i{}; i < frameCount; ++i) {
		ResourceID frameID = SpriteManager::CreateSubTexture(texID, SpriteProperties{ animframeID++, {i, idxCoordy}, {dim.x, dim.y} });
		ap.frames.push_back( AnimationFrame{ 0.f, frameID }); // supposed to be read somewhere... instead of 0.f....
	}

	mAnimationFrameLists[rid] = std::move(ap);
	return rid; 
}

/*  _________________________________________________________________________ */
/*!
\brief Retrieves the list of animation frames for a given animation.

@param rid
The unique identifier for the animation resource.

@return
Returns a reference to the list of AnimationFrames associated with the given ResourceID.

This function fetches the list of animation frames corresponding to the specified animation resource ID.
*/
AnimationFrames& AnimationManager::GetAnimationFrameList(ResourceID rid) {
	return mAnimationFrameLists[rid].frames;
}



// asset manager
/*  _________________________________________________________________________ */
/*!
\brief Loads an animation asset from a JSON object.

@param obj
The JSON object containing the animation asset data.

@return
Returns the ResourceID of the loaded animation asset.

This function reads animation properties from a JSON object and loads the corresponding animation using the LoadAnimation function.
*/
ResourceID AnimationManager::LoadAsset(rapidjson::Value const& obj) {
	ResourceID key{ obj["id"].GetUint64() };
	LoadAnimation(obj["path"].GetString(), key, obj["frameCount"].GetInt(), obj["idxCoordY"].GetFloat(),
		glm::vec2{obj["dimX"].GetFloat(), obj["dimY"].GetFloat()});
	return key;
}

/*  _________________________________________________________________________ */
/*!
\brief Saves the animation properties to a JSON object.

@param aid
The unique identifier for the animation asset.

@param props
The properties of the animation to be saved.

@param obj
The JSON object where the animation properties will be saved.

This function serializes the animation properties and saves them to a specified JSON object.
*/
void AnimationManager::SaveAsset(AssetID aid, AnimationProperties const& props, rapidjson::Value& obj) {
	//frees the previous sprites
	for (auto const& frame : props.frames){
		SpriteManager::UnloadSprite(frame.spriteID);
	}
	auto sm{ Serializer::SerializationManager::GetInstance() };

	sm->ModifyValue(obj, "frameCount", props.frameCount);
	sm->ModifyValue(obj, "idxCoordY", props.idxCoordy);

	sm->ModifyValue(obj, "dimX", props.dim.x);                            
	sm->ModifyValue(obj, "dimY", props.dim.y);

	ResourceID key{ obj["id"].GetUint64() };
	LoadAnimation(obj["path"].GetString(), key, obj["frameCount"].GetInt(), obj["idxCoordY"].GetFloat(),
		glm::vec2{obj["dimX"].GetFloat(), obj["dimY"].GetFloat()});
}

/*  _________________________________________________________________________ */
/*!
\brief Retrieves the animation frames for a given animation asset.

@param rid
The unique identifier for the animation asset.

@return
Returns a constant reference to the list of AnimationFrames associated with the given ResourceID.

This function fetches the list of animation frames corresponding to the specified animation asset ID.
*/

AnimationFrames const& AnimationManager::GetAsset(ResourceID rid) {
	return mAnimationFrameLists[rid].frames;
}

/*  _________________________________________________________________________ */
/*!
\brief Retrieves the properties of a given animation asset.

@param rid
The unique identifier for the animation asset.

@return
Returns a reference to the AnimationProperties associated with the given ResourceID.

This function fetches the properties of the animation asset corresponding to the specified ID.
*/

AnimationProperties& AnimationManager::AnimationManager::GetAssetProperties(ResourceID rid) {
	return mAnimationFrameLists[rid];
}

/*  _________________________________________________________________________ */
/*!
\brief Adds a new animation asset to the manager.

@param obj
The JSON object where the new animation asset data will be inserted.

@param path
The file path to the animation resource.

@param rid
The unique identifier for the new animation asset.

@return
Returns the ResourceID of the added animation asset.

This function inserts default animation properties into a JSON object and returns the ResourceID of the new animation asset.
*/
ResourceID AnimationManager::AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID rid) {
	auto sm{ Serializer::SerializationManager::GetInstance() };

	sm->InsertValue(obj, "frameCount", 0);
	sm->InsertValue(obj, "idxCoordY", 0.0);

	sm->InsertValue(obj, "dimX", 0.0);
	sm->InsertValue(obj, "dimY", 0.0);
	
	return rid;

}

