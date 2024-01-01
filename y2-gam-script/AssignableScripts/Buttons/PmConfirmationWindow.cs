using Image;
using System;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Object
{
    public class PmConfirmationWindow : Entity
    {
        //bool firstTime = true;
        //bool isPaused = true;
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public PmConfirmationWindow() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public PmConfirmationWindow(uint entityHandle) : base(entityHandle)
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
            //GameplayWrapper.SpawnPrefab("ConfirmationYes", new Vector2(Translation.X - 150, Translation.Y - 170));
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
            //if(isPaused)

            if(Input.IsKeyClicked(KeyCode.KEY_Y))
            {
                QuitGame();
            }
            //resume game
            else if(Input.IsKeyClicked(KeyCode.KEY_N) || Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
            {
                //Colour = new Vector4(1, 1, 1, 0);
                GameplayWrapper.DestroyEntity(entityID);
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


