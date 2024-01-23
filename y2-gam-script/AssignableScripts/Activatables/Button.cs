/******************************************************************************/
/*!
\par        Image Engine
\file       Button.cs

\author     (@digipen.edu)
\date       Jan 21, 2024

\brief      The main script for button activation. Has OnCreate and OnUpdate 
            functions.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;

namespace Object
{

    public class Button : Entity
    {
        public bool isPressed;
        private Vector2 buttonMid;
        private Vector2 buttonLeft;
        private Vector2 buttonRight;

        /*  _________________________________________________________________________ */
        /*! Button

        @return *this

        Default constructor for the button entity. Inherits from entity class.
        */
        public Button() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Button
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a button entity.
        */
        public Button(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the button entity.
        */
        void OnCreate()
        {
            isPressed = false;
            buttonMid = new Vector2(Translation.X, Translation.Y + (Scale.Y / 2.0f) + 1);
            buttonLeft = new Vector2(Translation.X - (Scale.X / 2.0f) + 2.0f, Translation.Y + (Scale.Y / 2.0f) + 1);
            buttonRight = new Vector2(Translation.X + (Scale.X / 2.0f) - 2.0f, Translation.Y + (Scale.Y / 2.0f) + 1);
        }


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the button entity.
        */
        void OnUpdate(float dt)
        {
            if (PhysicsWrapper.Raycast(Collider, buttonMid, entityID, out RaycastHit buttMid) ||
                PhysicsWrapper.Raycast(Collider, buttonLeft, entityID, out RaycastHit buttLeft) ||
                PhysicsWrapper.Raycast(Collider, buttonRight, entityID, out RaycastHit buttRight))
            {
                isPressed = true;
            }

            else
            {
                isPressed = false;
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