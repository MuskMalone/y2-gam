/******************************************************************************/
/*!
\par        Image Engine
\file       Sound.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Aug 30, 2023

\brief      Header file for Audio library that wraps around FMOD. 
            Remember to download
            and install FMOD core (developer version) before using.

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
    /*  _________________________________________________________________________ */
    /*! AudioInit

    @return bool
    True if successful operation, false if unsuccessful.

    Initializes the audio system.
    Must be called in the engine initilization!
    */
    static bool AudioInit();

    /*  _________________________________________________________________________ */
    /*! AudioUpdate

    @return none.

    The main update for sound.
    Should be called somewhere in the engine's main update loop.
    */
    static void AudioUpdate();

    /*  _________________________________________________________________________ */
    /*! AudioExit

    @return none.

    Releases the resources used.
    Should be called in the engine's release function.
    */
    static void AudioExit();

    /*  _________________________________________________________________________ */
    /*! AudioCreateGroup

    @return SoundGroup
    The FMOD channel group that is created.

    Creates a new channel group, then returns the handle for the channel
    group created.
    */
    static SoundGroup AudioCreateGroup();

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
    static Sound AudioLoadSound(const char* filepath);

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
    static Sound AudioLoadMusic(const char* filepath);

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
    static void AudioPlay(Sound const& audio, SoundGroup const& group, int loops);

    /*  _________________________________________________________________________ */
    /*! AudioResumeGroup

    @param group
    The channel group.

    @return none.

    Resumes the channel.
    */
    static void AudioResumeGroup(SoundGroup const& group);

    /*  _________________________________________________________________________ */
    /*! AudioStopGroup

    @param group
    The channel group.

    @return none.

    Stops the channel from playing. This frees up internal resources for reuse.
    Once stopped, the channel handle becomes invalid.
    */
    static void AudioStopGroup(SoundGroup const& group);

    /*  _________________________________________________________________________ */
    /*! AudioPauseGroup

    @param group
    The channel group.

    @return none.

    Sets the paused state of a channel group to true, which halts playback.
    */
    static void AudioPauseGroup(SoundGroup const& group);

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
    static void AudioSetGroupVolume(SoundGroup const& group, float volume);

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
    static void AudioSetGroupPitch(SoundGroup const& group, float pitch);

    static ResourceID LoadAsset(SoundProperties const& props);
    static ResourceID LoadAsset(rapidjson::Value const& obj);
    static void SaveAsset(ResourceID aid, SoundProperties const& props, rapidjson::Value& obj);
    static Sound const& GetAsset(ResourceID);
    static SoundProperties & GetAssetProperties(ResourceID);
    static ResourceID AddAsset(rapidjson::Value& obj, std::string const& path, ResourceID id);

  private:
    static FMOD::System* sSystem;
    static std::map<ResourceID, std::pair<Sound, SoundProperties>> _mSoundAssets;

  };
}