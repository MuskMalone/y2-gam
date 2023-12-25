/******************************************************************************/
/*!
\par        Image Engine
\file       Player.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 26, 2023

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
        public Entity otherEntity;
        public float JumpSpeed;
        public float MovementSpeed;

        private Vector2 spawnPosition = new Vector2(-400, -27);
        private Vector2 colliderPosition = new Vector2(-400, -36);

        public bool isGrounded = true;
        public bool slowdownToggle = true;
        bool godMode = false;

        float temp_dt = 0f;
        bool isPaused = false;

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
                        //resume game
                        ResumeGame();
                        isPaused = false;
                    }
                }

                if (!isPaused)
                {

                    if (!godMode)
                    {
                        FacingDirection = isFacingRight;

                        if (PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2) + 2, Collider.Y),
                            new Vector2(Collider.X - (ColliderDimensions.X / 2) + 2, Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out RaycastHit leftRayCast) ||
                                PhysicsWrapper.Raycast(new Vector2(Collider.X + (ColliderDimensions.X / 2) - 2, Collider.Y),
                            new Vector2(Collider.X + (ColliderDimensions.X / 2) - 2, Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out RaycastHit rightRayCast) ||
                                PhysicsWrapper.Raycast(new Vector2(Collider.X, Collider.Y),
                            new Vector2(Collider.X, Collider.Y - (ColliderDimensions.Y / 2) - 1), entityID, out RaycastHit centreRayCast))
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


                        if (Input.IsKeyPressed(KeyCode.KEY_0) && (Input.IsKeyPressed(KeyCode.KEY_9)))
                        {
                            godMode = true;
                            Gravity = new Vector2(0.0f, 0.0f);
                            Mass = 0;
                        }

                        if (Input.IsKeyClicked(KeyCode.KEY_SPACE))
                        {
                            GameplayWrapper.SlowdownTime(slowdownToggle);
                            slowdownToggle = !slowdownToggle;
                        }

                        if (Input.IsKeyPressed(KeyCode.KEY_W))
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

                        Vector2 playerEnd = new Vector2(Collider.X - (Scale.X / 4.5f), Collider.Y);
                        if (PhysicsWrapper.Raycast(Collider, playerEnd, entityID, out RaycastHit waypointHit) && waypointHit.tag == "Waypoint")
                        {
                            float waypointOffset = 2.0f;
                            float colliderOffset = 9.0f;
                            spawnPosition = Translation;
                            spawnPosition += new Vector2(waypointOffset, waypointOffset);
                            colliderPosition = Translation;
                            colliderPosition += new Vector2(waypointOffset, waypointOffset);
                            colliderPosition -= new Vector2(0, colliderOffset);
                        }

                        if (PhysicsWrapper.Raycast(Collider, playerEnd, entityID, out RaycastHit enemyHit) && enemyHit.tag == "Enemy")
                        {
                            Respawn();
                        }

                        Vector2 playerCollider = new Vector2(Collider.X, Collider.Y);

                        Vector2 spikesTip = new Vector2(Translation.X, Translation.Y - (Scale.Y / 2.0f) - 2.0f);

                        if (PhysicsWrapper.Raycast(playerCollider, spikesTip, entityID, out RaycastHit spikeHit))
                        {
                            if (spikeHit.tag == "Spikes")
                            {
                                Respawn();
                            }
                        }

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
            AnimationState = (int)AnimationCodePlayer.RUN;
            Velocity -= new Vector2(MovementSpeed, 0.0f) * dt;
            isFacingRight = false;
        }

        public void MoveRight(float dt)
        {
            AnimationState = (int)AnimationCodePlayer.RUN;
            Velocity += new Vector2(MovementSpeed, 0.0f) * dt;
            isFacingRight = true;
        }

        public void Jump(float dt)
        {
            Velocity -= new Vector2(0, Velocity.Y);
            Velocity += new Vector2(0, JumpSpeed) * dt;
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
