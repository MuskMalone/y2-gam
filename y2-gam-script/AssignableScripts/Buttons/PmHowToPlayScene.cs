using Image;
using System;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Object
{
    public class PmHowToPlayScene : Entity
    {
        //bool firstTime = true;
        //bool isPaused = false;
        PmHowToPlay htp = GameplayWrapper.FindEntityByName("PmHowToPlay").As<PmHowToPlay>();
        //PauseMenu pm = GameplayWrapper.FindEntityByName("PauseMenu").As<PauseMenu>();
        //PmResumeGame resume = GameplayWrapper.FindEntityByName("PmResumeGame").As<PmResumeGame>();
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public PmHowToPlayScene() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public PmHowToPlayScene(uint entityHandle) : base(entityHandle)
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
            //if (Input.IsKeyClicked(KeyCode.KEY_Q))
            //{
            if (htp.htpClicked == true)
            {
                Colour = new Vector4(1, 1, 1, 1);

            }
            else
            {
                Colour = new Vector4(1, 0, 1, 0);
            }
            //}
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


