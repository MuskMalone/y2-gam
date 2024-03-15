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
        public bool IsFacingRight;
        public bool SlowdownToggle = true;
        public bool GodMode = false;
        public float MaxHorizontalVelocity;
        public bool PlayDeathAnimation = false;
        public float PlayDeathAnimHowLongAfter;

        public bool PlayAppearAnimation = false;
        public float PlayAppearTimer = 0.0f;
        public float MaxAppearTime;

        public bool Dead = false;
        public float RespawnTimer = 0.0f;
        public float MaxRespawnTime;

        public Vector2 spawnPosition;
        public Vector2 colliderPosition;

        public String keyName;
        public bool KeyCollected = false;
        private float footstepTimer = 0.0f;
        private float footstepInterval = 0.39f;
        string[] footstepSounds;

        private Vector2 playerHead;
        public bool isPaused = false;
        private bool firstTime = true;
        private int DeathAudioIncrement = 1;
        private int PlayerDeathAudioIncrement = 1;
        private int JumpAudioIncrement = 1;
        private int MAX_DEATH_AUDIO_FILES = 6;
        private int MAX_JUMP_AUDIO_FILES = 12;
        private int MAX_PLAYERDEATH_AUDIO_FILES = 8;
        private string FootTrack;
        private ReverbCode reverbSetting;

        Card card;

        bool resetAnimationState = true;
        private bool onInit = true;
        public float VisionRange;
        public float OriginalFriction;

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

        // Grounded related
        private bool _isGrounded;
        private bool IsGrounded
        {
            get { return _isGrounded; }
            set
            {
                if (_isGrounded != value)
                {
                    _isGrounded = value;
                    GroundedStatusChanged = true;
                }
            }
        }
        public bool GroundedStatusChanged { get; private set; }


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
            isPaused = false;
            //GameplayWrapper.SlowdownTime(false);
            if (GetCurrentScene() == "Level1")
            {
                spawnPosition = new Vector2(-400, -27);
                colliderPosition = new Vector2(-400, -36);
                reverbSetting = ReverbCode.OFF;
                //FootTrack = "PlayerRunningFloor.wav";
            }
            else if (GetCurrentScene()== "Level1Transition")
            {
                spawnPosition = new Vector2(-592.5f, -114.5f);
                colliderPosition = new Vector2(-591.5f, -123.5f);
                reverbSetting = ReverbCode.OFF;
                //FootTrack = "PlayerRunningFloor.wav";
            }
            else if (GetCurrentScene() == "Level2")
            {
                spawnPosition = new Vector2(184.5f, 165.5f);
                colliderPosition = new Vector2(183.0f, 156.0f);
                reverbSetting = ReverbCode.CAVE;
                //FootTrack = "FOOTSTEPS-OUTDOOR_GEN-HDF-12206.wav";
            }

            GraphicsWrapper.EmitterSetAlpha(entityID, 0, -1);
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
            //GraphicsWrapper.EmitterSetAlpha(entityID, 1, 2);
            if (onInit)
            {
                card = GameplayWrapper.FindEntityByName("Card").As<Card>();
                onInit = false;
            }

            IsFacingRight = isFacingRight;
 

            if (isPaused)
            {
                dt = 0f;
                //PauseGame();
            }

            if (Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
            {
                if (!isPaused)
                {
                    PauseGame();
                    //isPaused = true;
                }
                else
                {
                    ResumeGame();
                    //isPaused = false;
                }
            }

            if (!isPaused)
            {
                if (!GodMode && !Dead)
                {
                    firstTime = true;

                    if (PlayAppearAnimation)
                    {
                        if (resetAnimationState == true)
                        {
                            GameplayWrapper.ResetAnimationState(entityID);
                            resetAnimationState = false;
                        }
                        
                        AnimationState = (int)AnimationCodePlayer.APPEAR;
                        PlayAppearTimer += dt;

                        if (PlayAppearTimer >= MaxAppearTime)
                        {
                            PlayAppearAnimation = false;
                            resetAnimationState = true;
                            PlayAppearTimer = 0;
                        }
                    }

                    float visionOffset = IsFacingRight ? VisionRange : -VisionRange;

                    RaycastHit topFacingRayCast = new RaycastHit();
                    RaycastHit midFacingRayCast = new RaycastHit();
                    RaycastHit botFacingRayCast = new RaycastHit();
                    if (PhysicsWrapper.Raycast(new Vector2(Collider.X, Collider.Y + (ColliderDimensions.Y / 2.0f)), new Vector2(Collider.X + visionOffset, Collider.Y + (ColliderDimensions.Y / 2.0f)), entityID,
                        out topFacingRayCast) ||
                        PhysicsWrapper.Raycast(new Vector2(Collider.X, Collider.Y), new Vector2(Collider.X + visionOffset, Collider.Y), entityID,
                        out midFacingRayCast) ||
                        PhysicsWrapper.Raycast(new Vector2(Collider.X, Collider.Y - (ColliderDimensions.Y / 2.0f) + 2.0f), new Vector2(Collider.X + visionOffset, Collider.Y - (ColliderDimensions.Y / 2.0f) + 2.0f), entityID,
                        out botFacingRayCast)
                        ) {
                        Friction = 0.0f;
                    }
                    else
                    {
                        Friction = OriginalFriction;
                    }

                    RaycastHit centreRayCast = new RaycastHit();
                    RaycastHit leftRayCast = new RaycastHit();
                    RaycastHit rightRayCast = new RaycastHit();

                    if (PhysicsWrapper.IsCollidedWithLayer(entityID, "Spikes"))
                    {
                        Dead = true;
                    }

                    if (PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2) + 2, Collider.Y),
                        new Vector2(Collider.X - (ColliderDimensions.X / 2) + 0.5f, Collider.Y - (ColliderDimensions.Y / 2) - 3), entityID, out leftRayCast) ||
                            PhysicsWrapper.Raycast(new Vector2(Collider.X + (ColliderDimensions.X / 2) - 2, Collider.Y),
                        new Vector2(Collider.X + (ColliderDimensions.X / 2) - 0.5f, Collider.Y - (ColliderDimensions.Y / 2) - 3), entityID, out rightRayCast) ||
                            PhysicsWrapper.Raycast(new Vector2(Collider.X, Collider.Y),
                        new Vector2(Collider.X, Collider.Y - (ColliderDimensions.Y / 2) - 3), entityID, out centreRayCast))
                    {
                        IsGrounded = true;
                        if (!PlayAppearAnimation)
                        {
                            AnimationState = (int)AnimationCodePlayer.IDLE;
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

                    if (GroundedStatusChanged)
                    {
                        if (IsGrounded)
                        {
                            PlayAudio("player_hit_the_ground.wav", 0, (int)reverbSetting);
                        }

                        GroundedStatusChanged = false;
                    }


                    if (Input.IsKeyPressed(KeyCode.KEY_0) && (Input.IsKeyPressed(KeyCode.KEY_9)))
                    {
                        GodMode = true;
                        Gravity = new Vector2(0.0f, 0.0f);
                        Mass = 0;
                    }

                    if (Input.IsKeyClicked(KeyCode.KEY_L))
                    {
                        Dead = true;
                    }

                    if (Input.IsKeyClicked(KeyCode.KEY_LEFT_SHIFT))
                    {
                        GameplayWrapper.SlowdownTime(SlowdownToggle);
                        SlowdownToggle = !SlowdownToggle;
                    }

                    if (Input.IsKeyClicked(KeyCode.KEY_W) || Input.IsKeyClicked(KeyCode.KEY_SPACE))
                    {
                        if (IsGrounded)
                        {

                            //PlayAudio("Robin Jump_" + JumpAudioIncrement + ".wav", 0);
                            Jump(dt);

                        }


                        //if (!Input.IsKeyPressed(KeyCode.KEY_A) && !Input.IsKeyPressed(KeyCode.KEY_D))
                        //{
                            //Velocity = new Vector2(0.0f, Velocity.Y);
                        //}
                    }

                    if (Input.IsKeyReleased(KeyCode.KEY_A) || Input.IsKeyReleased(KeyCode.KEY_D))
                    {
                        //Console.WriteLine("A was released");
                        //PauseAudioWithFilename("PlayerRunningScaffolding.wav");
                        //PauseAudioWithFilename(FootTrack);
                        GraphicsWrapper.EmitterSetAlpha(entityID, 0, -1);
                        Velocity *= 0.2f;
                    }

                    if (Input.IsKeyPressed(KeyCode.KEY_A))
                    {
                        MoveLeft(dt);
                        if (IsGrounded)
                        {
                            GraphicsWrapper.EmitterSetPosition(entityID, new Vector2(GameplayWrapper.PlayerPos.X + (float)5, GameplayWrapper.PlayerPos.Y - (float)15.0), -1);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 4);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 5);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 6);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 7);

                        }
                        else GraphicsWrapper.EmitterSetAlpha(entityID, 0, -1);

                        if (IsGrounded && (centreRayCast.layer == "Platform" ||
                            leftRayCast.layer == "Platform" ||
                            rightRayCast.layer == "Platform"))
                        {
                            //PlayAudio(FootTrack, 0);
                            //ResumeAudioWithFilename(FootTrack);
                            footstepTimer += dt;

                            if (footstepTimer >= footstepInterval)
                            {
                                PlayFloorFootstep();
                                footstepTimer = 0.0f;
                            }
                        }

                        else if (IsGrounded && (centreRayCast.layer == "Scaffolding" ||
                            leftRayCast.layer == "Scaffolding" ||
                            rightRayCast.layer == "Scaffolding"))
                        {
                            //PlayAudio("PlayerRunningScaffolding.wav", 0);
                            //ResumeAudioWithFilename("PlayerRunningScaffolding.wav");
                            footstepTimer += dt;

                            if (footstepTimer >= footstepInterval)
                            {
                                PlayScaffoldingFootstep();
                                footstepTimer = 0.0f;
                            }
                        }
                    }

                    else if (Input.IsKeyPressed(KeyCode.KEY_D))
                    {
                        MoveRight(dt);
                        if (IsGrounded) {
                            GraphicsWrapper.EmitterSetPosition(entityID, new Vector2(GameplayWrapper.PlayerPos.X - (float)5, GameplayWrapper.PlayerPos.Y - (float)15.0), -1);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 0);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 1);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 2);
                            GraphicsWrapper.EmitterSetAlpha(entityID, 1, 3);
                        }
                        else GraphicsWrapper.EmitterSetAlpha(entityID, 0, -1);

                        //Console.WriteLine("D was Pressed");
                        if (IsGrounded && (centreRayCast.layer == "Platform" ||
                            leftRayCast.layer == "Platform" ||
                            rightRayCast.layer == "Platform"))
                        {

                            //PlayAudio(FootTrack, 0);
                            //ResumeAudioWithFilename(FootTrack);
                            footstepTimer += dt;

                            if (footstepTimer >= footstepInterval)
                            {
                                PlayFloorFootstep();
                                footstepTimer = 0.0f;
                            }
                        }

                        else if (IsGrounded && (centreRayCast.layer == "Scaffolding" ||
                            leftRayCast.layer == "Scaffolding" ||
                            rightRayCast.layer == "Scaffolding"))
                        {
                            //PlayAudio("PlayerRunningScaffolding.wav", 0);
                            //ResumeAudioWithFilename("PlayerRunningScaffolding.wav");
                            footstepTimer += dt;

                            if (footstepTimer >= footstepInterval)
                            {
                                PlayScaffoldingFootstep();
                                footstepTimer = 0.0f;
                            }
                        }
                    }

                    //if (!IsGrounded)
                    //{
                    //    //PauseAudioWithFilename("PlayerRunningScaffolding.wav");
                    //    //PauseAudioWithFilename(FootTrack);
                    //}

                    playerHead = new Vector2(Collider.X, Collider.Y + (Scale.Y / 2.0f) + 1);
                    if (PhysicsWrapper.Raycast(Collider, playerHead, entityID, out RaycastHit anvilHit) && anvilHit.tag == "Anvil")
                    {
                        Dead = true;
                    }
                }

                else if (!GodMode && Dead)
                {
                    RespawnTimer += dt;
                    AnimationState = (int)AnimationCodePlayer.DEAD;

                    if (firstTime)
                    {
                        SlowdownToggle = true;
                        Friction = OriginalFriction;
                        
                        PlayAudio("PlayerDeath_FX_0" + DeathAudioIncrement + ".wav", 0, (int)reverbSetting);
                        PlayAudio("Robin Death_" + PlayerDeathAudioIncrement + ".wav", 0, (int)reverbSetting);
                    }

                    if (RespawnTimer >= PlayDeathAnimHowLongAfter && firstTime)
                    {
                        PlayDeathAnimation = true;
                        firstTime = false;
                        card.timeAlive = card.MAX_TIME_ALIVE;
                        card.ResetCardPos();
                        card.ResetCardUI();
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
            //StopAudioWithFilename("PlayerRunningScaffolding.wav");
            //StopAudioWithFilename("PlayerRunningFloor.wav");
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
            PlayAudio("Robin Jump_" + JumpAudioIncrement + ".wav", 0, (int)reverbSetting);
            JumpAudioIncrement++;

            if (JumpAudioIncrement > MAX_JUMP_AUDIO_FILES)
            {
                JumpAudioIncrement = 1;
            }
        }

        public void Respawn()
        {
            Translation = spawnPosition;
            Collider = colliderPosition;

            DeathAudioIncrement++;
            PlayerDeathAudioIncrement++;

            if (DeathAudioIncrement > MAX_DEATH_AUDIO_FILES)
            {
                DeathAudioIncrement = 1;
            }
            if (PlayerDeathAudioIncrement > MAX_PLAYERDEATH_AUDIO_FILES)
            {
                PlayerDeathAudioIncrement = 1;
            }
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

        public void PauseGame()
        {
            InternalCalls.EngineCore_SetPrevSceneVar(GetCurrentScene());
            isPaused = true;
        }

        public void ResumeGame()
        {
            isPaused = false;
        }

        private void PlayFloorFootstep()
        {

            Console.WriteLine("Movement played");
            if (GetCurrentScene() == "Level1" || GetCurrentScene() == "Level1Transition")
            {
                footstepSounds = new string[]
                {
                    "PlayerRunningFloor_001.wav", "PlayerRunningFloor_002.wav", "PlayerRunningFloor_003.wav","PlayerRunningFloor_004.wav","PlayerRunningFloor_005.wav",
                    "PlayerRunningFloor_006.wav","PlayerRunningFloor_007.wav","PlayerRunningFloor_008.wav","PlayerRunningFloor_009.wav","PlayerRunningFloor_010.wav",
                    "PlayerRunningFloor_011.wav","PlayerRunningFloor_012.wav","PlayerRunningFloor_013.wav","PlayerRunningFloor_014.wav", "PlayerRunningFloor_015.wav",
                    "PlayerRunningFloor_016.wav","PlayerRunningFloor_017.wav","PlayerRunningFloor_018.wav","PlayerRunningFloor_019.wav","PlayerRunningFloor_020.wav",
                };
            }
            else if(GetCurrentScene() == "Level2")
            {
                footstepSounds = new string[]
                {
                    "PlayerRunningConcrete_1.wav", "PlayerRunningConcrete_2.wav", "PlayerRunningConcrete_3.wav","PlayerRunningConcrete_4.wav","PlayerRunningConcrete_5.wav",
                    "PlayerRunningConcrete_6.wav","PlayerRunningConcrete_7.wav","PlayerRunningConcrete_8.wav"
                };
            }


            Random random = new Random();
            int randomIndex = random.Next(0, footstepSounds.Length);
            string footstepSound = footstepSounds[randomIndex];

            Console.WriteLine($"{footstepSound}");

            PlayAudio(footstepSound, 0, (int)reverbSetting);
            //SetAudioFileVolume(footstepSound, 0.8f);
            ResumeAudioWithFilename(footstepSound);
        }

        private void PlayScaffoldingFootstep()
        {
            Console.WriteLine("Scaffolding played");
            footstepSounds = new string[]
            {
                "PlayerRunningScaffolding_001.wav", "PlayerRunningScaffolding_002.wav", "PlayerRunningScaffolding_003.wav","PlayerRunningScaffolding_004.wav","PlayerRunningScaffolding_005.wav",
                "PlayerRunningScaffolding_006.wav","PlayerRunningScaffolding_007.wav","PlayerRunningScaffolding_008.wav","PlayerRunningScaffolding_009.wav","PlayerRunningScaffolding_010.wav",
                "PlayerRunningScaffolding_011.wav","PlayerRunningScaffolding_012.wav","PlayerRunningScaffolding_013.wav","PlayerRunningScaffolding_014.wav", "PlayerRunningScaffolding_015.wav",
                "PlayerRunningScaffolding_016.wav","PlayerRunningScaffolding_017.wav","PlayerRunningScaffolding_018.wav","PlayerRunningScaffolding_019.wav","PlayerRunningScaffolding_020.wav",
            };

            Random random = new Random();
            int randomIndex = random.Next(0, footstepSounds.Length);
            string footstepSound = footstepSounds[randomIndex];

            Console.WriteLine($"{footstepSound}");

            PlayAudio(footstepSound, 0, (int)reverbSetting);
            ResumeAudioWithFilename(footstepSound);
        }

    }


}
