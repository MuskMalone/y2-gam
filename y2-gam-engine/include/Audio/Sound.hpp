/******************************************************************************/
/*!
\par        Image Engine
\file       Sound.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 21, 2023

\brief      Header file for Audio library that wraps around FMOD.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

namespace Image {

  typedef FMOD::Sound* Sound;
  typedef FMOD::ChannelGroup* SoundGroup;

  struct SoundProperties : public ResProp{
      std::string path;
      bool stream;
  };

  class SoundManager{
  public:
    using SoundAssetPair = std::pair<Sound, SoundProperties>;

  public:
    // Fmod Control
    static bool AudioInit();
    static void AudioUpdate();
    static void AudioExit();
    static SoundGroup AudioCreateGroup();
    static Sound AudioLoadSound(const char* filepath);
    static Sound AudioLoadMusic(const char* filepath);
    static void AudioPlay(Sound const& audio, SoundGroup const& group, int loops);
    static void AudioPlay(Sound const& audio, int loops);
    static void AudioPlay(std::string filename, int loops);
    static void AudioResumeGroup(SoundGroup const& group);
    static void AudioStopChannelFromFilename(std::string filename);
    static void AudioResumeChannelFromFilename(std::string filename);
    static void AudioPauseChannelFromFilename(std::string filename);
    static void AudioStopGroup(SoundGroup const& group);
    static void AudioPauseGroup(SoundGroup const& group);
    static void AudioSetGroupVolume(SoundGroup const& group, float volume);
    static void AudioSetGroupPitch(SoundGroup const& group, float pitch);

    // Sound Assets Management
    static ResourceID LoadAsset(SoundProperties const& props);
    static ResourceID LoadAsset(rapidjson::Value const& obj);
    static void SaveAsset(ResourceID aid, SoundProperties const& props, rapidjson::Value& obj);
    static Sound const& GetAsset(ResourceID);
    static ResourceID GetResourceID(std::string);
    static SoundProperties & GetAssetProperties(ResourceID);
    static ResourceID AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID id);

  public:
    static SoundGroup musicGroup;
    static SoundGroup sfxGroup;

  private:
    static FMOD::System* sSystem;
    static std::vector<std::pair<std::pair<std::string, FMOD::Channel*>, bool>> isAudioPlaying;
    static std::map<ResourceID, std::pair<Sound, SoundProperties>> _mSoundAssets;
    static std::map<std::string, ResourceID> sSoundResourceMap;
    static std::map<Sound, std::string> sGroupMap;
    static FMOD_RESULT F_CALLBACK OnSoundFinished(FMOD_CHANNELCONTROL* channelControl,
      FMOD_CHANNELCONTROL_TYPE controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void* commandData1, void* commandData2);
  };
}