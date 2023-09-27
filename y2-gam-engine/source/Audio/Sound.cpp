/******************************************************************************/
/*!
\par        Hemp Engine
\file       Sound.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Aug 30, 2023

\brief      Source file for Audio library that wraps around FMOD. 
            Remember to download,
            and install FMOD core (developer version), then setup paths for
            library and linker before using.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "../include/pch.h"
#include "Audio/Sound.hpp"

#include "Logging/LoggingSystem.hpp"
#include "Logging/backward.hpp"

namespace Image {

  FMOD::System* SoundManager::sSystem{ nullptr };

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

    result = FMOD::System_Create(&sSystem);              // Create the main system object.

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
      return false;
    }
    else {
      //std::cout << "Successful FMOD System Creation" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful FMOD System Creation" + '\n', __FUNCTION__);
    }

    result = sSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
      return false;
    }
    else {
      //std::cout << "Successful FMOD System Initialization" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful FMOD System Initialization" + '\n', __FUNCTION__);
    }

    return true;
  }

  /*  _________________________________________________________________________ */
  /*! AudioUpdate

  @return none.

  The main update for sound.
  Should be called somewhere in the engine's main update loop.
  */
  void SoundManager::AudioUpdate() {
    FMOD_RESULT result;
    result = sSystem->update();

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
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

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      //std::cout << "Successful FMOD System Shutdown" << "\n";
      LoggingSystem::GetInstance().Log(LogLevel::INFO_LEVEL, "Successful FMOD System Shutdown" + '\n', __FUNCTION__);
    }
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
    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Successful FMOD Channel Group Creation" << "\n";
    }
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
    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Successfully loaded sound file " << filepath << "\n";
    }

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
    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Successfully loaded music file " << filepath << "\n";
    }

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

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Successfully set loop count to " << loops << "\n";
    }

    result = sSystem->playSound(audio, group, false, NULL);

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Now playing" << "\n";
    }
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

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Resume Group" << "\n";
    }
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

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Group Stopped" << "\n";
    }
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

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
        std::string str(FMOD_ErrorString(result));
        LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Paused Group" << "\n";
    }
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

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Set group volume to " << volume << "\n";
    }
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

    if (result != FMOD_OK) {
      //std::cout << "FMOD error! " << FMOD_ErrorString(result) << "\n";
      std::string str(FMOD_ErrorString(result));
      LoggingSystem::GetInstance().Log(LogLevel::ERROR_LEVEL, "FMOD error! " + str + '\n', __FUNCTION__);
    }
    else {
      std::cout << "Set group pitch to " << pitch << "\n";
    }
  }
}