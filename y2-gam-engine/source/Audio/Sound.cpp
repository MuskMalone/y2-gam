/******************************************************************************/
/*!
\par        Image Engine
\file       Sound.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 26, 2024

\brief      Source file for Audio library that wraps around FMOD. 

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "../include/pch.hpp"
#include "Audio/Sound.hpp"
#include <Windows.h>

namespace Image {
  // Static Initialization
  FMOD::System* SoundManager::sSystem{ nullptr };
  std::vector<AudioInformation> SoundManager::AudioPlaying;
  std::map<ResourceID, std::pair<Sound, SoundProperties>> SoundManager::_mSoundAssets;
  std::map<std::string, ResourceID> SoundManager::sSoundResourceMap;
  std::map<Sound, std::string> SoundManager::sGroupMap;
  SoundGroup SoundManager::musicGroup;
  SoundGroup SoundManager::sfxGroup;
  
  namespace {
    HWND gameWindowHandle;
    HWND foregroundWindow;
  }

  /*  _________________________________________________________________________ */
  /*! AudioInit

  @return bool
  True if successful operation, false if unsuccessful.

  Initializes the audio system.
  Must be called in the engine initilization!
  */
  bool SoundManager::AudioInit() {
    FMOD_RESULT result;
    sSystem = nullptr;
    _mSoundAssets.clear();
    result = FMOD::System_Create(&sSystem);

    if (result != FMOD_OK) {
#ifndef _INSTALLER
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
#endif
      return false;
    }
#ifndef _INSTALLER
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful FMOD System Creation", __FUNCTION__);
    }
#endif

    result = sSystem->init(512, FMOD_INIT_NORMAL, 0);

    if (result != FMOD_OK) {
#ifndef _INSTALLER
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
#endif
      return false;
    }
#ifndef _INSTALLER
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful FMOD System Initialization", __FUNCTION__);
    }
#endif

    musicGroup = Image::SoundManager::AudioCreateGroup();
    sfxGroup = Image::SoundManager::AudioCreateGroup();

    Image::SoundManager::AudioSetGroupVolume(musicGroup, 0.1f);
    Image::SoundManager::AudioSetGroupVolume(sfxGroup, 0.2f);

    return true;
  }

  /*  _________________________________________________________________________ */
  /*! AudioUpdate

  @return none.

  The main update for sound.
  Should be called somewhere in the engine's main update loop.
  */
  void SoundManager::AudioUpdate() {
    FMOD_RESULT result{};

    FMOD::ChannelGroup* masterGroup;
    sSystem->getMasterChannelGroup(&masterGroup);

    ::gameWindowHandle = FindWindowA(NULL, WINDOW_TITLE);
    ::foregroundWindow = GetForegroundWindow();

    if (::gameWindowHandle == ::foregroundWindow) {
      masterGroup->setPaused(false);
    }

    else {
      masterGroup->setPaused(true);
    }
    
    result = sSystem->update();

    // Calculate the volume needed for positional audio
    for (AudioInformation& ai : AudioPlaying) {
      if (ai.isPositional) {
        // Do Something
      }
    }
    
#ifndef _INSTALLER
    if (result != FMOD_OK) {
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioExit

  @return none.

  Releases the resources used.
  Should be called in the engine's release function.
  */
  void SoundManager::AudioExit() {
    FMOD_RESULT result;
    result = sSystem->release();

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful FMOD System Shutdown", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioCreateGroup

  @return SoundGroup
  The FMOD channel group that is created.

  Creates a new channel group, then returns the handle for the channel
  group created.
  */
  SoundGroup SoundManager::AudioCreateGroup() {
    FMOD_RESULT result;
    SoundGroup sg = nullptr;

    result = sSystem->createChannelGroup(NULL, &sg);
#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful FMOD Channel Group Creation", __FUNCTION__);
    }
#endif
    return sg;
  }

  /*  _________________________________________________________________________ */
  /*! AudioLoadSound

  @param filepath
  Filepath to the sound file.

  @return Sound
  The FMOD sound that is created.

  Loads the audio of a music file from a specific file path, then
  returns a handle to the sound that can be played.

  Preferably used with shorter song files, e.g. sound effects of a gun.
  */
  Sound SoundManager::AudioLoadSound(const char* filepath) {
    FMOD_RESULT result;
    Sound ret;

    result = sSystem->createSound(filepath, FMOD_LOOP_NORMAL, NULL, &ret);
#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      std::string str(filepath);
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successfully loaded sound file" + str, __FUNCTION__);
    }
#endif

    return ret;
  }

  /*  _________________________________________________________________________ */
  /*! AudioLoadMusic

  @param filepath
  Filepath to the sound file.

  @return Sound
  The FMOD sound that is created.

  Creates an FMOD sound with the stream flag active, which will help performance
  when playing longer sound tracks.

  Preferably used with longer song files, e.g. background music of a game.
  */
  Sound SoundManager::AudioLoadMusic(const char* filepath) {
    FMOD_RESULT result;
    Sound ret;

    result = sSystem->createSound(filepath, FMOD_CREATESTREAM | FMOD_LOOP_NORMAL, NULL, &ret);
#ifndef _INSTALLER
    if (result != FMOD_OK) {
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      std::string str(filepath);
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successfully loaded music file" + str, __FUNCTION__);
    }
#endif

    return ret;
  }

  /*  _________________________________________________________________________ */
  /*! AudioPlay

  @param audio
  The sound to play.

  @param group
  The group of the sound.

  @param loops
  The number of times the sound is to be looped.
  -1 means it will be looped indefinetely.
  0 means it will not be looped.

  @return none

  Plays the sound for a specific sound and group. Can specify the number of times
  it is looped as well.
  */
  void SoundManager::AudioPlay(Sound const& audio, SoundGroup const& group, int loops) {
    FMOD_RESULT result;
    result = audio->setLoopCount(loops);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful set loop count to " + std::to_string(loops), __FUNCTION__);
    }
#endif

    result = sSystem->playSound(audio, group, false, NULL);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Now playing", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioPlay

  @param audio
  The sound to play.

  @param loops
  The number of times the sound is to be looped.
  -1 means it will be looped indefinetely.
  0 means it will not be looped.

  @return none

  Plays the sound for a specific sound and group. Can specify the number of times
  it is looped as well.

  Simplified version of AudioPlay that auto detects the group the audio should be in.
  */
  void SoundManager::AudioPlay(Sound const& audio, int loops) {
    FMOD_RESULT result;
    result = audio->setLoopCount(loops);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful set loop count to " + std::to_string(loops), __FUNCTION__);
    }
#endif
    auto am{ AssetManager::GetInstance() };
    SoundGroup& group{ sGroupMap[audio] == "music" ? musicGroup : sfxGroup };
    result = sSystem->playSound(audio, group, false, NULL);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Now playing", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioPlay

  @param audio
  The sound to play.

  @param loops
  The number of times the sound is to be looped.
  -1 means it will be looped indefinetely.
  0 means it will not be looped.

  @return none

  Plays the sound for a specific sound and group. Can specify the number of times
  it is looped as well.

  Extremely simplified version of AudioPlay that auto detects the group the audio 
  should be in from just the filename.
  */
  void SoundManager::AudioPlay(std::string filename, int loops) {
    // Prevent stacking of audio that is already playing
    for (AudioInformation& pair : AudioPlaying) {
      if (pair.audioName == filename && pair.isPlaying) {
        return;
      }
    }

    Sound const& audio{ GetAsset(GetResourceID(filename)) };
    FMOD_RESULT result;
    result = audio->setLoopCount(loops);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful set loop count to " + std::to_string(loops), __FUNCTION__);
    }
#endif

    auto am{ AssetManager::GetInstance() };
    SoundGroup& group{ sGroupMap[audio] == "music" ? musicGroup : sfxGroup };

    bool found = false;
    for (AudioInformation& ai : AudioPlaying) {
      if (ai.audioName == filename) {
        found = true;
        ai.isPlaying = true;
        ai.isPositional = false;
        ai.position = Vec2(-1.f, -1.f);
        result = sSystem->playSound(audio, group, false, &ai.audioChannel);
        ai.audioChannel->setCallback(OnSoundFinished);
        break;
      }
    }

    if (!found) {
      FMOD::Channel* channel{ nullptr };
      result = sSystem->playSound(audio, group, false, &channel);

      // Register the callback for when the sound finishes playing
      result = channel->setCallback(OnSoundFinished);
      AudioInformation ai{};
      ai.audioName = filename;
      ai.audioChannel = channel;
      ai.isPlaying = true;
      ai.isPositional = false;
      ai.position = Vec2(-1.f, -1.f);
      AudioPlaying.push_back(ai);
    }

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Now playing", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioPlayPositional

  @param audio
  The sound to play.

  @param loops
  The number of times the sound is to be looped.
  -1 means it will be looped indefinetely.
  0 means it will not be looped.

  @param position
  The position (in game coordinates) that the audio will be played from.

  @return none

  Plays the sound for a specific sound and group. Can specify the number of times
  it is looped as well.

  AudioPlayPositional is strictly for positional audio, in which the closer an object
  is to the centre of the camera, the louder it will be. Use it as an alternative
  to AudioPlay.
  */
  void SoundManager::AudioPlayPositional(std::string filename, int loops, Vec2 position) {
    // Prevent stacking of audio that is already playing
    for (AudioInformation& pair : AudioPlaying) {
      if (pair.audioName == filename && pair.isPlaying) {
        return;
      }
    }

    Sound const& audio{ GetAsset(GetResourceID(filename)) };
    FMOD_RESULT result;
    result = audio->setLoopCount(loops);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful set loop count to " + std::to_string(loops), __FUNCTION__);
    }
#endif

    auto am{ AssetManager::GetInstance() };
    SoundGroup& group{ sGroupMap[audio] == "music" ? musicGroup : sfxGroup };

    bool found = false;
    for (AudioInformation& ai : AudioPlaying) {
      if (ai.audioName == filename) {
        found = true;
        ai.isPlaying = true;
        result = sSystem->playSound(audio, group, false, &ai.audioChannel);
        ai.audioChannel->setCallback(OnSoundFinished);
        ai.isPositional = true;
        ai.position = position;
        break;
      }
    }

    if (!found) {
      FMOD::Channel* channel{ nullptr };
      result = sSystem->playSound(audio, group, false, &channel);

      // Register the callback for when the sound finishes playing
      result = channel->setCallback(OnSoundFinished);
      AudioInformation ai{};
      ai.audioName = filename;
      ai.audioChannel = channel;
      ai.isPlaying = true;
      ai.isPositional = true;
      ai.position = position;
      AudioPlaying.push_back(ai);
    }

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Now playing", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioResumeGroup

  @param group
  The channel group.

  @return none.

  Resumes the channel.
  */
  void SoundManager::AudioResumeGroup(SoundGroup const& group) {
    FMOD_RESULT result;
    result = group->setPaused(false);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Resume Group", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioStopChannelFromFilename

  @param filename
  The filename.

  @return none.

  Stops the audio that is playing from a given filename. The channel will
  not be able to used after stopping.
  */
  void SoundManager::AudioStopChannelFromFilename(std::string filename) {
    for (auto& itr : AudioPlaying) {
      if (itr.audioName == filename) {
        itr.audioChannel->setVolume(0.0f);
        itr.audioChannel->stop();
        itr.isPlaying = false;
        break;
      }
    }
  }

  /*  _________________________________________________________________________ */
  /*! AudioResumeChannelFromFilename

  @param filename
  The filename.

  @return none.

  Resumes channel.
  */
  void SoundManager::AudioResumeChannelFromFilename(std::string filename) {
    for (auto& itr : AudioPlaying) {
      if (itr.audioName == filename) {
        bool isPaused{};
        itr.audioChannel->getPaused(&isPaused);
        if (isPaused) {
          itr.audioChannel->setPaused(false);
        }
        break;
      }
    }
  }

  /*  _________________________________________________________________________ */
  /*! AudioPauseChannelFromFilename

  @param filename
  The filename.

  @return none.

  Pauses channel.
  */
  void SoundManager::AudioPauseChannelFromFilename(std::string filename) {
    for (auto& itr : AudioPlaying) {
      if (itr.audioName == filename) {
        bool isPaused{};
        itr.audioChannel->getPaused(&isPaused);
        if (!isPaused) {
          itr.audioChannel->setPaused(true);
        }
        break;
      }
    }
  }

  /*  _________________________________________________________________________ */
  /*! AudioPauseAll

  @return none.

  Pauses all audio.
  */
  void SoundManager::AudioPauseAll() {
    AudioPauseGroup(sfxGroup);
    AudioPauseGroup(musicGroup);
  }

  /*  _________________________________________________________________________ */
  /*! AudioResumeAll
   
  @return none.

  Resumes all audio.
  */
  void SoundManager::AudioResumeAll() {
    AudioResumeGroup(sfxGroup);
    AudioResumeGroup(musicGroup);
  }

  /*  _________________________________________________________________________ */
  /*! AudioStopGroup

  @param group
  The channel group.

  @return none.

  Stops the channel from playing. This frees up internal resources for reuse.
  Once stopped, the channel handle becomes invalid.
  */
  void SoundManager::AudioStopGroup(SoundGroup const& group) {
    FMOD_RESULT result;
    result = group->stop();

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Group Stopped", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioPauseGroup

  @param group
  The channel group.

  @return none.

  Sets the paused state of a channel group to true, which halts playback.
  */
  void SoundManager::AudioPauseGroup(SoundGroup const& group) {
    FMOD_RESULT result;
    result = group->setPaused(true);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Paused Group", __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioSetGroupVolume

  @param group
  The channel group.

  @param volume
  The volume to set to.
  The range is from 0 to 1, where 0 represents 0% and 1 represents 100%.

  @return none.

  Sets the volume of the channel group.
  */
  void SoundManager::AudioSetGroupVolume(SoundGroup const& group, float volume) {
    FMOD_RESULT result;
    result = group->setVolume(volume);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Set group volume to " + std::to_string(volume), __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! AudioSetGroupPitch

  @param group
  The channel group.

  @param pitch
  The pitch to set to.
  The range is 0 to 2, where 0.5 represents half pitch (one octave down), 1.0
  represents unmodified pitch and 2.0 represents double pitch (one octave up).

  @return none.

  Sets the pitch of the channel group.
  */
  void SoundManager::AudioSetGroupPitch(SoundGroup const& group, float pitch) {
    FMOD_RESULT result;
    result = group->setPitch(pitch);

#ifndef _INSTALLER
    if (result != FMOD_OK) {
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str, __FUNCTION__);
    }
    else {
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Set group pitch to " + std::to_string(pitch), __FUNCTION__);
    }
#endif
  }

  /*  _________________________________________________________________________ */
  /*! LoadAsset

  @param props

  @return ResourceID

  Loads the sound asset.
  */
  ResourceID SoundManager::LoadAssetSoundProperties(SoundProperties const& props) {
    ResourceID key{ props.id };
    if (_mSoundAssets.find(key) != _mSoundAssets.end()) return key;
    _mSoundAssets[key] = ((props.stream) ? (SoundAssetPair{ AudioLoadMusic(props.path.c_str()), SoundProperties{ props.id, props.path, true } })
        : (SoundAssetPair{AudioLoadSound(props.path.c_str()), SoundProperties{ props.id, props.path, false }}));

    // Populate filename to resource ID map
    std::string path{ props.path };
    std::size_t pos{ path.find_last_of("\\") };

    std::string sub{ path.substr(pos + 1) };
    sSoundResourceMap[sub] = key;

    // Add to groups
    path = path.substr(0, pos);
    pos = path.find_last_of("\\");
    sub = path.substr(pos + 1);

    sGroupMap[_mSoundAssets[key].first] = sub;

    return key;
  }

  /*  _________________________________________________________________________ */
  /*! LoadAsset

  @param obj
  The rapidjson object.

  @return ResourceID

  Loads the sound asset on app open.
  */
  ResourceID SoundManager::LoadAsset(rapidjson::Value const& obj) {
    std::cout << "Load Asset for sound" << std::endl;
    return LoadAssetSoundProperties(SoundProperties{ obj["id"].GetUint64(), obj["path"].GetString(), obj["stream"].GetBool()});
  }


  void SoundManager::UnloadAsset(AssetManager::Asset const& asset) {

  }

  /*  _________________________________________________________________________ */
  /*! SaveAsset

  @param aid

  @param props

  @param obj
  The rapidjson object.

  @return none.

  Serializes asset.
  */
  void SoundManager::SaveAsset(AssetID aid, SoundProperties const& props, rapidjson::Value &obj) {
    _mSoundAssets[aid].second.stream = props.stream;
    Serializer::SerializationManager::GetInstance()->ModifyValue(obj, "stream", props.stream);
  }

  /*  _________________________________________________________________________ */
  /*! GetAsset

  @param aid

  @return Sound

  Gets the sound from the asset map.
  */
  Sound const& SoundManager::GetAsset(ResourceID aid) {
    return _mSoundAssets[aid].first;
  }

  /*  _________________________________________________________________________ */
  /*! GetResourceID

  @param filename (not the full path)

  @return ResourceID

  Gets the ResourceID.
  */
  ResourceID SoundManager::GetResourceID(std::string filename) {
    return sSoundResourceMap[filename];
  }

  /*  _________________________________________________________________________ */
  /*! GetAssetProperties

  @param aid

  @return SoundProperties

  Gets the sound property from the asset map.
  */
  SoundProperties& SoundManager::GetAssetProperties(ResourceID aid) {
    return _mSoundAssets[aid].second;
  }

  /*  _________________________________________________________________________ */
  /*! AddAsset

  @param obj

  @param id

  @return ResourceID

  Adds asset.
  */
  ResourceID SoundManager::AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID id) {
    auto sm{ Serializer::SerializationManager::GetInstance() };

    sm->InsertValue(obj, "stream", false);
    return id;
  }

  FMOD_RESULT F_CALLBACK SoundManager::OnSoundFinished(FMOD_CHANNELCONTROL* channelControl, FMOD_CHANNELCONTROL_TYPE controlType,
    FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void* commandData1, void* commandData2) {
    if (callbackType == FMOD_CHANNELCONTROL_CALLBACK_END) {
      for (AudioInformation& pair : AudioPlaying) {
        if (pair.audioChannel == reinterpret_cast<FMOD::Channel*>(channelControl)) {
          pair.isPlaying = false;
          break;
        }
      }
    }

    return FMOD_OK;
  }
}