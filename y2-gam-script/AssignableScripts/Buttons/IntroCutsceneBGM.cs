using Image;
using System;

namespace Object
{
    public class IntroCutsceneBGM : Entity
    {
        bool isPlaying = false;

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public IntroCutsceneBGM() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public IntroCutsceneBGM(uint entityHandle) : base(entityHandle)
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
            if (GetCurrentScene() == "IntroCutscene" && !isPlaying)
            {
                PlayAudio("openingCutscene.wav", 0);
                isPlaying = true;
            }

            if (Input.IsKeyClicked(KeyCode.KEY_SPACE))
            {
                LoadScene("LevelSelect");
            }
            //else
            //{
            //    StopAudio();

                //}

                //if(firstTime)
                //{
                //StopAudio();
                //firstTime = false;
                //}
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

