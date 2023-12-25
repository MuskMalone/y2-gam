/******************************************************************************/
/*!
\par        Image Engine
\file       Door.cs

\author      (@digipen.edu)
\date       Oct 24, 2023

\brief      The main script for destroying open door. Has OnCreate and OnUpdate 
            functions.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;

namespace Object
{
    public class Door : Entity
    {
        Button button = GameplayWrapper.FindEntityByName("Button").As<Button>();

        /*  _________________________________________________________________________ */
        /*! Door

        @return *this

        Default constructor for the open door entity. Inherits from entity class.
        */
        public Door() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Door
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a open door entity.
        */
        public Door(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the open door entity.
        */
        // Don't worry about the 'unused' message, as the one using/referencing it is the C++ code!
        void OnCreate()
        {

        }


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the open door entity.
        */
        void OnUpdate(float dt)
        {
            if (!IsEditorMode())
            {
                if (button.isPressed == false)
                {
                    //button.
                    PlayAudio("door_open.wav", 0);
                    GameplayWrapper.DestroyEntity(entityID);
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
