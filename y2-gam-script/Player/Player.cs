/******************************************************************************/
/*!
\par        Image Engine
\file       Player.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The main script for a ‘player’ entity. Has OnCreate and OnUpdate 
            functions. Currently the player input is located here.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class Player : Entity
    {
        // Force Based
        public readonly float JumpForce = 30000.0f;
        public readonly float MovementForce = 1200.0f;

        public int Health { get; set; } = 1;

        public bool isGrounded = true;

        // Direction related
        //public bool directionChanged = false;
        private bool _isFacingRight = true;
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
        /*! Player

        @return *this

        Default constructor for the player entity. Inherits from entity class.
        */
        public Player() : base()
        {
            //Console.WriteLine("Player Default Constructor Called!");
        }

        /*  _________________________________________________________________________ */
        /*! Player
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a player entity.
        */
        public Player(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the player entity.
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

        Called on every update for the player entity.
        */
        void OnUpdate(float dt)
        {
            if (!IsEditorMode())
            {
                // Workaround for now
                if (Math.Abs(Velocity.Y) > 1.0f)
                {
                    isGrounded = false;
                }

                else
                {
                    isGrounded = true;
                }

                if (FacingDirectionChanged)
                {
                    Scale = new Vector3(-Scale.X, Scale.Y, Scale.Z);
                    FacingDirectionChanged = false; // Reset the flag
                }

                if (Input.IsKey
                    ed((KeyCode.KEY_SPACE)))
                {
                    Jump();
                }

                else if (Input.IsKeyPressed((KeyCode.KEY_LEFT)))
                {
                    MoveLeft();
                }

                else if (Input.IsKeyPressed((KeyCode.KEY_RIGHT)))
                {
                    MoveRight();
                }

                else
                {
                    AnimationState = (int)AnimationCode.IDLE;
                }
            }
        }

        public void MoveLeft()
        {
            float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            AnimationState = (int)AnimationCode.RUN;
            Force -= new Vector2(horizontalMovement, 0.0f);
            isFacingRight = false;
        }

        public void MoveRight()
        {
            float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            AnimationState = (int)AnimationCode.RUN;
            Force += new Vector2(horizontalMovement, 0.0f);
            isFacingRight = true;
        }

        public void Jump()
        {
            Force += new Vector2(0, JumpForce);
        }

        public void PlayerTakeDamage()
        {
            Health -= 1;
            if(Health <= 0)
            {
                Console.WriteLine("Player died");
            }
        }
    }
}
