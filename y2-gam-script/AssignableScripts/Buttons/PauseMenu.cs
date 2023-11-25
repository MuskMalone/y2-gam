using Image;
using System;

namespace Object
{
    public class PauseMenu : Entity
    {
        bool firstTime = true;
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public PauseMenu() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public PauseMenu(uint entityHandle) : base(entityHandle)
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
            Colour = new Vector4(1, 1, 1, 1);
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
            if (!IsEditorMode())
            {
                //SetSprite("start_glow");
                if (UIHover)
                {
                    Colour = new Vector4(1, 1, 1, 1);
                    //SetSprite("start_glow");
                }
                else
                {
                    Colour = new Vector4(1, 1, 1, 0);
                }

                if (UIClicked && firstTime)
                {
                    LoadScene("Level1");
                    firstTime = false;
                }
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

