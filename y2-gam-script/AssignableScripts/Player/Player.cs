/******************************************************************************/
/*!
\par        Image Engine
\file       Player.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Jan 21, 2024

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
        public float JumpSpeed;
        public float MovementSpeed;
        public bool IsGrounded = true;
        public bool SlowdownToggle = true;
        private bool IsKeyPressed = false;
        public bool GodMode = false;
        public bool IsFacingRight;
        public float MaxHorizontalVelocity;

        public Vector2 spawnPosition = new Vector2(-400, -27);
        public Vector2 colliderPosition = new Vector2(-400, -36);
        
        private float temp_dt = 0f;
        private bool isPaused = false;

        // Direction related
        private bool _isFacingRight;
        private bool isFacingRight
        {
            get { return _isFacingRight; }
            set
            {
                if (_isFacingRight != value)
                {
                    _isFacingRight = value;
                    FacingDirectionChanged = true;
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
        void OnCreate()
        {
            // Get the serialized IsFacingRight value
            isFacingRight = IsFacingRight;
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
            IsFacingRight = isFacingRight;

            if (isPaused)
            {
                dt = 0f;
                //PauseGame();
            }

            if (Input.IsKeyClicked(KeyCode.KEY_P))
            {
                if (!isPaused)
                {
                    //PauseGame();
                    temp_dt = dt;
                    dt = 0f;
                    isPaused = true;
                }
                else
                {
                    //ResumeGame();
                    isPaused = false;
                }
            }

            

            if (!GodMode)
            {
                RaycastHit centreRayCast = new RaycastHit();
                RaycastHit leftRayCast = new RaycastHit();
                RaycastHit rightRayCast = new RaycastHit();

                if (PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2) + 2, Collider.Y),
                    new Vector2(Collider.X - (ColliderDimensions.X / 2) + 0.5f, Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out leftRayCast) ||
                        PhysicsWrapper.Raycast(new Vector2(Collider.X + (ColliderDimensions.X / 2) - 2, Collider.Y),
                    new Vector2(Collider.X + (ColliderDimensions.X / 2) - 0.5f, Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out rightRayCast) ||
                        PhysicsWrapper.Raycast(new Vector2(Collider.X, Collider.Y),
                    new Vector2(Collider.X, Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out centreRayCast))
                {
                    IsGrounded = true;
                    AnimationState = (int)AnimationCodePlayer.IDLE;

                    if (centreRayCast.tag == "Spikes" || leftRayCast.tag == "Spikes" || rightRayCast.tag == "Spikes" ||
                        centreRayCast.tag == "Enemy" || leftRayCast.tag == "Enemy" || rightRayCast.tag == "Enemy")
                    {
                        Respawn();
                    }
                }

                else
                {
                    IsGrounded = false;
                    AnimationState = (int)AnimationCodePlayer.JUMP;
                }

                if (FacingDirectionChanged)
                {
                    Scale = new Vector3(-Scale.X, Scale.Y, Scale.Z);
                    FacingDirectionChanged = false; // Reset the flag
                }


                if (Input.IsKeyPressed(KeyCode.KEY_0) && (Input.IsKeyPressed(KeyCode.KEY_9)))
                {
                    GodMode = true;
                    Gravity = new Vector2(0.0f, 0.0f);
                    Mass = 0;
                }

                if (Input.IsKeyPressed(KeyCode.KEY_SPACE))
                {
                    if (!IsKeyPressed)
                    {
                        GameplayWrapper.SlowdownTime(SlowdownToggle);
                        SlowdownToggle = !SlowdownToggle;
                        IsKeyPressed = true;
                    }
                }

                else
                {
                    IsKeyPressed = false;
                }

                if (!isPaused)
                {
                    if (Input.IsKeyPressed(KeyCode.KEY_W))
                    {
                        if (IsGrounded)
                        {
                            Jump(dt);
                        }

                        if (!Input.IsKeyPressed(KeyCode.KEY_A) && !Input.IsKeyPressed(KeyCode.KEY_D))
                        {
                            Velocity = new Vector2(0.0f, Velocity.Y);
                        }
                    }

                    if (Input.IsKeyPressed(KeyCode.KEY_A))
                    {
                        MoveLeft(dt);
                    }

                    else if (Input.IsKeyPressed(KeyCode.KEY_D))
                    {
                        MoveRight(dt);
                    }

                    else if (Input.IsKeyReleased(KeyCode.KEY_A))
                    {
                        Velocity *= 0.2f;
                    }

                    else if (Input.IsKeyReleased(KeyCode.KEY_D))
                    {
                        Velocity *= 0.2f;
                    }

                }
                /*
                if (PhysicsWrapper.Raycast(new Vector2(Collider.X + (ColliderDimensions.X / 2.0f) + 1.0f, Collider.Y), 
                    new Vector2(Collider.X - (ColliderDimensions.X / 2.0f) - 1.0f, Collider.Y), entityID, 
                    out RaycastHit enemyHit) && enemyHit.tag == "Enemy")
                {
                    Respawn();
                }
                */

                if (Translation.Y <= -99.0f)
                {
                    Respawn();
                }
            }

            else
            {
                AnimationState = (int)AnimationCodePlayer.IDLE;
                ColliderDimensions = new Vector2(0f, 0f);

                if (Input.IsKeyPressed(KeyCode.KEY_W))
                {
                    FlyUp(dt);
                }

                if (Input.IsKeyPressed(KeyCode.KEY_A))
                {
                    FlyLeft(dt);
                }

                if (Input.IsKeyPressed(KeyCode.KEY_D))
                {
                    FlyRight(dt);
                }

                if (Input.IsKeyPressed(KeyCode.KEY_S))
                {
                    FlyDown(dt);
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

        //void PauseGame()
        //{
        //    SaveScene("Level1");
        //}

        //void ResumeGame()
        //{
        //    LoadScene("Level1");
        //}
        public void MoveLeft(float dt)
        {
            AnimationState = (int)AnimationCodePlayer.RUN;
            //Velocity -= new Vector2(MovementSpeed, 0.0f) * dt;
            //if (Velocity.X <= -MaxHorizontalVelocity)
            //{
            //    Velocity = new Vector2(-MaxHorizontalVelocity, Velocity.Y);
            //}
            Velocity = new Vector2(-MovementSpeed, Velocity.Y);
            isFacingRight = false;
        }

        public void MoveRight(float dt)
        {
            AnimationState = (int)AnimationCodePlayer.RUN;
            //Velocity += new Vector2(MovementSpeed, 0.0f) * dt;
            //if (Velocity.X >= MaxHorizontalVelocity)
            //{
            //    Velocity = new Vector2(MaxHorizontalVelocity, Velocity.Y);
            //}
            Velocity = new Vector2(MovementSpeed, Velocity.Y);
            isFacingRight = true;
        }

        public void Jump(float dt)
        {
            //Velocity -= new Vector2(0, Velocity.Y);
            //Velocity += new Vector2(0, JumpSpeed) * dt;
            Velocity = new Vector2 (Velocity.X, JumpSpeed);
        }

        public void Respawn()
        {
            Translation = spawnPosition;
            Collider = colliderPosition;
        }
        public void FlyLeft(float dt)
        {
            Translation -= new Vector2(MovementSpeed, 0) * dt;
            isFacingRight = false;
        }

        public void FlyRight(float dt)
        {
            Translation += new Vector2(MovementSpeed, 0) * dt;
            isFacingRight = true;
        }

        public void FlyUp(float dt)
        {
            Translation += new Vector2(0, MovementSpeed) * dt;
        }

        public void FlyDown(float dt)
        {
            Translation += new Vector2(0, -MovementSpeed) * dt;
        }
    }
}
