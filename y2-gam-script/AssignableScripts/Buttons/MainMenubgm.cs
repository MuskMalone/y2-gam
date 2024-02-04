using Image;
using System;

namespace Object
{
    public class MainMenubgm : Entity
    {
        bool isPlaying = false;
        
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public MainMenubgm() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public MainMenubgm (uint entityHandle) : base(entityHandle)
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
            if (GetCurrentScene() == "MainMenu" && !isPlaying)
            {
                PlayAudio("PM_Menu_Music_Loop.wav", -1);
                isPlaying = true;
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

