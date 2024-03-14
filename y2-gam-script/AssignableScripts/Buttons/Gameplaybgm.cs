using Image;
using System;

namespace Object
{
    public class Gameplaybgm : Entity
    {
        bool firstTime = true;
        private ReverbCode reverbSetting;
        String currentScene;

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public Gameplaybgm() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public Gameplaybgm(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation.
        */
        void OnCreate()
        {
            currentScene = GetCurrentScene();

            if (currentScene == "Level1")
            {
                reverbSetting = ReverbCode.OFF;
            }

            else if (currentScene == "Level1Transition")
            {
                reverbSetting = ReverbCode.OFF;
            }

            else if (currentScene == "Level2")
            {
                reverbSetting = ReverbCode.CAVE;
            }

            else
            {
                reverbSetting = ReverbCode.OFF;
            }
        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update.
        */
        void OnUpdate(float dt)
        {
            if (firstTime)
            {
                if (currentScene == "Level1")
                {
                    PlayAudio("Poltergeist'sMansionTheme_BGM_Loop.wav", -1, (int)reverbSetting);
                    SetAudioFileVolume("Poltergeist'sMansionTheme_BGM_Loop.wav", 0.7f);
                }

                else if (currentScene == "Level2")
                {
                    PlayAudio("Level2_BGM.wav", -1, (int)reverbSetting);
                }

                else if (currentScene == "HowToPlay")
                {
                    PlayAudio("PM_Menu_Music_Loop.wav", -1, (int)reverbSetting);
                }

                firstTime = false;
            }
        }

        /*  _________________________________________________________________________ */
        /*! OnExit

        @return none

        Called on exit.
        */
        void OnExit()
        {
            StopAudio();
        }
    }
}

