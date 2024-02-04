/******************************************************************************/
/*!
\par        Image Engine
\file       Arms.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 25, 2023

\brief      Script for the arms that throw the card.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;   

namespace Object
{
    public class Arms : Entity
    {
        public float timeAlive = 0.0f;
        public float MAX_TIME_ALIVE;
        public float lengthOfArm;
        private float offsetAwayFromBody = -10.0f;
        public bool alive = false;

        private bool _isFacingRight;
        public bool isFacingRight
        {
            get { return _isFacingRight; }
            set
            {
                if (_isFacingRight != value)
                {
                    _isFacingRight = value;
                    FacingDirectionChanged = true; // Set another flag when isFacingRight changes
                }
            }
        }
        public bool FacingDirectionChanged { get; private set; }

        /*  _________________________________________________________________________ */
        /*! Arms

        @return *this

        Default constructor for the arm entity. Inherits from entity class.
        */
        public Arms() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Arms
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a arm entity.
        */
        public Arms(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the card entity.
        */
        void OnCreate()
        {
            Colour = new Vector4(1, 1, 1, 0);
            Scale = new Vector3(30, 30, 1);
        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the arms entity.
        */
        void OnUpdate(float dt)
        {
            if (Input.IsMousePressed(KeyCode.MOUSE_BUTTON_RIGHT))
            {
                alive = true;

                isFacingRight = MousePos.X > GameplayWrapper.PlayerPos.X;

                if (FacingDirectionChanged)
                {
                    Scale = new Vector3(-Scale.X, Scale.Y, Scale.Z);
                    FacingDirectionChanged = false; // Reset the flag
                    offsetAwayFromBody = -offsetAwayFromBody;
                }

                float angleRad = (float)Math.Atan2(MousePos.Y - GameplayWrapper.PlayerPos.Y,
                MousePos.X - GameplayWrapper.PlayerPos.X);

                Vector2 armPosition = GameplayWrapper.PlayerPos
                    + new Vector2((float)Math.Cos(angleRad), (float)Math.Sin(angleRad)) * lengthOfArm;

                Translation = new Vector2(armPosition.X + offsetAwayFromBody, armPosition.Y);
            }
            
            if (alive)
            {
                Colour = new Vector4(1, 1, 1, 1);
                timeAlive += dt;
            }

            else
            {
                Colour = new Vector4(1, 1, 1, 0);
            }

            if ((timeAlive >= MAX_TIME_ALIVE))
            {
                timeAlive = 0.0f;
                alive = false;
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
