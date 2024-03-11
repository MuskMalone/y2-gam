using Image;
using System;

namespace Object
{
    public class MainMenu : Entity
    {
        bool isPlaying = false;

        public String StartButtonTag;
        EnterLevelOne StartButton;

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public MainMenu() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public MainMenu(uint entityHandle) : base(entityHandle)
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
            if (StartButtonTag != null)
            {
                StartButton = GameplayWrapper.FindEntityByName(StartButtonTag).As<EnterLevelOne>();
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
            if (GetCurrentScene() == "MainMenu" && !isPlaying)
            {
                PlayAudio("PM_Menu_Music_Loop.wav", -1);
                isPlaying = true;
            }

            if (StartButton.hovered)
            {
                AnimationState = 1;
            }
            else
            {
                AnimationState = 0;
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
