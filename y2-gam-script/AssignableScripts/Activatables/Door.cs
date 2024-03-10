/******************************************************************************/
/*!
\par        Image Engine
\file       Door.cs

\author     (@digipen.edu)
\date       Jan 21, 2024

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
        public string buttonTag;
        private bool previousButtonState;
        Button button;
        private float doorColliderYOffset = 3000.0f;

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
            if (buttonTag != null)
            {
                button = GameplayWrapper.FindEntityByName(buttonTag).As<Button>();
            }

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

            if (button.isPressed == true && !previousButtonState)
            {
                PlayAudio("door_open.wav", 0);
                AnimationState = (int)AnimationCodeDoor.OPEN;
                previousButtonState = true;
                Collider = new Vector2(Collider.X, Collider.Y - doorColliderYOffset);
            }

            else if (!button.isPressed && previousButtonState)
            {
                AnimationState = (int)AnimationCodeDoor.CLOSED;
                previousButtonState = false;
                Collider = new Vector2(Collider.X, Collider.Y + doorColliderYOffset);
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
