/******************************************************************************/
/*!
\par        Image Engine
\file       Player.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 3, 2024

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
        public bool PlayDeathAnimation = false;

        public bool PlayAppearAnimation = false;
        public float PlayAppearTimer = 0.0f;
        public float MaxAppearTime;

        public bool Dead = false;
        public float RespawnTimer = 0.0f;
        public float MaxRespawnTime;

        public Vector2 spawnPosition = new Vector2(-400, -27);
        public Vector2 colliderPosition = new Vector2(-400, -36);

        private float temp_dt = 0f;
        private bool isPaused = false;
        private bool firstTime = true;

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
                dt = temp_dt;
                PauseGame();
            }

            if (Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
            {
                if (!isPaused)
                {

                    PauseGame();
                    temp_dt = dt;
                    dt = temp_dt;
                    isPaused = true;
                }
                else
                {
                    ResumeGame();
                    isPaused = false;
                }
            }

            if (!isPaused)
            {
                if (!GodMode && !Dead)
                {
                    firstTime = true;

                    if (PlayAppearAnimation)
                    {
                        AnimationState = (int)AnimationCodePlayer.APPEAR;
                        PlayAppearTimer += dt;

                        if (PlayAppearTimer >= MaxAppearTime)
                        {
                            PlayAppearAnimation = false;
                            PlayAppearTimer = 0;
                        }
                    }

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
                        if (!PlayAppearAnimation)
                        {
                            AnimationState = (int)AnimationCodePlayer.IDLE;
                        }

                        if (centreRayCast.tag == "Spikes" || leftRayCast.tag == "Spikes" || rightRayCast.tag == "Spikes" ||
                            centreRayCast.tag == "Enemy" || leftRayCast.tag == "Enemy" || rightRayCast.tag == "Enemy")
                        {
                            Dead = true;
                        }
                    }

                    else
                    {
                        IsGrounded = false;
                        if (!PlayAppearAnimation)
                        {
                            AnimationState = (int)AnimationCodePlayer.JUMP;
                        }
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

                    if (Translation.Y <= -99.0f)
                    {
                        Dead = true;
                    }
                }

                else if (!GodMode && Dead)
                {
                    RespawnTimer += dt;
                    AnimationState = (int)AnimationCodePlayer.DEAD;

                    if (RespawnTimer >= 0.5 && firstTime)
                    {
                        PlayDeathAnimation = true;
                        firstTime = false;
                    }

                    if (RespawnTimer >= MaxRespawnTime)
                    {
                        Respawn();
                        Dead = false;
                        RespawnTimer = 0;
                    }
                }

                else
                {
                    if (!PlayAppearAnimation)
                    {
                        AnimationState = (int)AnimationCodePlayer.IDLE;
                    }

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
        }

        /*  _________________________________________________________________________ */
        /*! OnExit

        @return none

        Called on exit.
        */
        void OnExit()
        {

        }

        void PauseGame()
        {
            SaveScene("Level1");
        }

        void ResumeGame()
        {
            LoadScene("Level1");
        }
        public void MoveLeft(float dt)
        {
            if (!PlayAppearAnimation)
            {
                AnimationState = (int)AnimationCodePlayer.RUN;
            }

            Velocity = new Vector2(-MovementSpeed, Velocity.Y);
            isFacingRight = false;
        }

        public void MoveRight(float dt)
        {
            if (!PlayAppearAnimation)
            {
                AnimationState = (int)AnimationCodePlayer.RUN;
            }

            Velocity = new Vector2(MovementSpeed, Velocity.Y);
            isFacingRight = true;
        }

        public void Jump(float dt)
        {
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
