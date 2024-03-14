using Image;
using System;

namespace Object
{
    public class Quit : Entity
    {
        bool firstTime = true;
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public Quit() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public Quit(uint entityHandle) : base(entityHandle)
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
            if (UIClicked && firstTime)
            {
                
                //StopAudio();
                QuitGame();
                PlayAudio("menu_click.wav", 0);
                firstTime = false;
            }

            if (UIHover)
            {
                Colour = new Vector4(1, 1, 1, 1);
            }
            else
            {
                Colour = new Vector4(1, 0, 1, 0);
            }
        }

        /*  _________________________________________________________________________ */
        /*! OnExit

        @return none

        Called on exit.
        */
        void OnExit()
        {

        }
    }
}

