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
        public readonly float JumpForce = 4000000.0f;
        public readonly float MovementForce = 100000.0f;
        public bool isGrounded = true;

        // Direction related
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
        /*! Player

        @return *this

        Default constructor for the player entity. Inherits from entity class.
        */
        public Player() : base()
        {

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
            isFacingRight = FacingDirection;
            FacingDirectionChanged = false;
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
                if (PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2), Collider.Y),
                new Vector2(Collider.X - (ColliderDimensions.X / 2), Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out RaycastHit leftRayCast) ||
                    PhysicsWrapper.Raycast(new Vector2(Collider.X + (ColliderDimensions.X / 2), Collider.Y),
                new Vector2(Collider.X + (ColliderDimensions.X / 2), Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out RaycastHit rightRayCast))
                {
                    isGrounded = true;
                    AnimationState = (int)AnimationCodePlayer.IDLE;
                }

                else
                {
                    isGrounded = false;
                    AnimationState = (int)AnimationCodePlayer.JUMP;
                }

                if (FacingDirectionChanged)
                {
                    Scale = new Vector3(-Scale.X, Scale.Y, Scale.Z);
                    FacingDirectionChanged = false; // Reset the flag
                }

                if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_RIGHT))
                {
                    FireCard(Collider);
                }

                if (Input.IsKeyClicked(KeyCode.KEY_SPACE))
                {
                    if (isGrounded)
                    {
                        Jump(dt);
                    }
                }

                else if (Input.IsKeyPressed(KeyCode.KEY_A))
                {
                    MoveLeft(dt);
                }

                else if (Input.IsKeyPressed(KeyCode.KEY_D))
                {
                    MoveRight(dt);
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
            FacingDirection = isFacingRight;
        }

        public void MoveLeft(float dt)
        {
            float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            AnimationState = (int)AnimationCodePlayer.RUN;
            Force -= new Vector2(horizontalMovement, 0.0f) * dt;
            isFacingRight = false;
        }

        public void MoveRight(float dt)
        {
            float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            AnimationState = (int)AnimationCodePlayer.RUN;
            Force += new Vector2(horizontalMovement, 0.0f) * dt;
            isFacingRight = true;
        }

        public void Jump(float dt)
        {
            Force += new Vector2(0, JumpForce) * dt;
        }
    }
}
