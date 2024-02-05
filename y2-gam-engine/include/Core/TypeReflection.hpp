#pragma once
#include <pch.hpp>
#include <Audio/Sound.hpp>
#include <Graphics/AnimationManager.hpp>
#include <Graphics/SpriteManager.hpp>
#include <Engine/AssetManager.hpp>
RTTR_REGISTRATION {
    //singleton registration
    rttr::registration::class_<AssetManager>("AssetManager")
        .method("GetInstance", &AssetManager::GetInstance)
        .method("SoundManagerLoadAsset", &AssetManager::LoadAsset<Image::SoundManager>)
        .method("AnimationManagerLoadAsset", &AssetManager::LoadAsset<AnimationManager>)
        .method("SpriteManagerLoadAsset", &AssetManager::LoadAsset<SpriteManager>);

    //method registration
    rttr::registration::class_<Image::SoundManager>("SoundManager")
        .method("LoadAsset", static_cast<ResourceID(*)(rapidjson::Value const&)>(&Image::SoundManager::LoadAsset))
        .method("SaveAsset", &Image::SoundManager::SaveAsset)
        .method("AddAsset", &Image::SoundManager::AddAsset);
    rttr::registration::class_<AnimationManager>("AnimationManager")
        .method("LoadAsset", static_cast<ResourceID(*)(rapidjson::Value const&)>(&AnimationManager::LoadAsset))
        .method("SaveAsset", &AnimationManager::SaveAsset)
        .method("AddAsset", &AnimationManager::AddAsset);
    rttr::registration::class_<SpriteManager>("SpriteManager")
        .method("LoadAsset", static_cast<ResourceID(*)(rapidjson::Value const&)>(&SpriteManager::LoadAsset))
        .method("SaveAsset", &SpriteManager::SaveAsset)
        .method("AddAsset", &SpriteManager::AddAsset);

    //template function registration

}