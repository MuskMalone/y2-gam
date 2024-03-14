/******************************************************************************/
/*!
\par        Image Engine
\file       BasicEnemy.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 23, 2023

\brief      The main script for a 'basic enemy' entity. Has OnCreate and OnUpdate 
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
    public class BasicEnemy : Entity
    {
        public float JumpCooldown;
        public float MovementSpeed;
        public float JumpForce;
        public float VisionRange;
        public float VisionHeightOffset;
        public float GroundHeightOffset;
        public float AttackRange;
        public bool isGrounded = true;
        public bool IsFacingRight;
        public bool EnemyDeath = false;
        public float EnemyDeathTimer = 0;
        public float HowLongDisplayEnemyDeath;
        private int DeathAudioIncrement = 1;
        private int MAX_DEATH_AUDIO_FILES = 7;
        private string audioFile;

        //PmResumeGame resume = GameplayWrapper.FindEntityByName("PmResumeGame").As<PmResumeGame>();
        public Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();

        //For pausing 
        //int temp_AnimationState = 0;
        //Vector2 temp_pos;
        //Vector2 temp_Force;
        //Vector2 temp_velocity;
        float temp_dt = 0f;
        //private bool isPaused = false;
        //PmResumeGame resume = GameplayWrapper.FindEntityByName("PmResumeGame").As<PmResumeGame>();
        // Time related
        public float TimeInState = 0.0f;
        public float JumpTimer = 0.0f;

        // Direction related
         bool _isFacingRight;
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

        // State Machines
        // All states inherit from this
        EnemyBaseState currentState;

        // States
        public EnemyDefaultState DefaultState = new EnemyDefaultState();
        public EnemyPatrolState PatrolState = new EnemyPatrolState();
        public EnemyAttackState AttackState = new EnemyAttackState();
        //public EnemyChaseState ChaseState = new EnemyChaseState();
        public EnemyIdleState IdleState = new EnemyIdleState();


        /*  _________________________________________________________________________ */
        /*! Basic Enemy
        
        @return *this

        Default constructor for the enemy entity. Inherits from entity class.
        */
        public BasicEnemy() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Basic Enemy
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a enemy entity.
        */
        public BasicEnemy(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the enemy entity.
        */
        // Don't worry about the 'unused' message, as the one using/referencing it is the C++ code!
        void OnCreate()
        {
            isFacingRight = IsFacingRight;
            FacingDirectionChanged = false;

            currentState = DefaultState;
            currentState.EnterState(this);
        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the enemy entity.
        */
        void OnUpdate(float dt)
        {
            IsFacingRight = isFacingRight;

            if (player.isPaused)
            {
                dt = 0f;

            }

            if (!player.isPaused)
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

                TimeInState += dt;
                currentState.UpdateState(this, dt);

                RaycastHit centreRayCast = new RaycastHit();
                RaycastHit leftRayCast = new RaycastHit();
                RaycastHit rightRayCast = new RaycastHit();

                if (PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2) + 2, Collider.Y),
                        new Vector2(Collider.X - (ColliderDimensions.X / 2) + 0.5f, Collider.Y + (ColliderDimensions.Y / 2) + 1), entityID, out leftRayCast) ||
                            PhysicsWrapper.Raycast(new Vector2(Collider.X + (ColliderDimensions.X / 2) - 2, Collider.Y),
                        new Vector2(Collider.X + (ColliderDimensions.X / 2) - 0.5f, Collider.Y + (ColliderDimensions.Y / 2) + 1), entityID, out rightRayCast) ||
                            PhysicsWrapper.Raycast(new Vector2(Collider.X, Collider.Y),
                        new Vector2(Collider.X, Collider.Y + (ColliderDimensions.Y / 2) + 1), entityID, out centreRayCast))
                {
                    if (centreRayCast.tag == "Anvil" || leftRayCast.tag == "Anvil" || rightRayCast.tag == "Anvil")
                    {
                        EnemyDeath = true;
                    }                  
                } 

                if (EnemyDeath)
                {
                    EnemyDeathTimer += dt;
                    AnimationState = (int)AnimationCodeEnemy.DEAD;
                    audioFile = "Jester Death_" + DeathAudioIncrement + ".wav";
                    PlayAudio(audioFile, 0);
                    SetAudioFileVolume(audioFile, 0.5f);
                    DeathAudioIncrement++;

                    if (DeathAudioIncrement > MAX_DEATH_AUDIO_FILES)
                    {
                        DeathAudioIncrement = 1;
                    }
                    if (EnemyDeathTimer >= HowLongDisplayEnemyDeath)
                    {
                        EnemyDeath = false;
                        EnemyDeathTimer = 0;
                        //PlayAudio("enemy_killed.wav", 0);
                        GameplayWrapper.DestroyEntity(entityID);
                    }
                }
            }
        }
        void OnExit()
        {

        }

        //void PauseGame()
        //{
        //    //pause the game
        //    temp_Force = Force;
        //    temp_pos = Translation;
        //    temp_velocity = Velocity;
        //    //temp_AnimationState = AnimationState;
        //    Force = new Vector2(0, 0);
        //    Translation = new Vector2((float)temp_pos.X, (float)temp_pos.Y);
        //    Velocity = new Vector2(0, 0);
        //}

        //void ResumeGame()
        //{
        //    Force = temp_Force;
        //    Translation = temp_pos;
        //    Velocity = temp_velocity;
        //}
        /*  _________________________________________________________________________ */
        /*! SwitchState
        
        @param state
        The state to switch to.

        @return none

        Switches the current state of the enemy entity. Called by the state machine.
        */
        public void SwitchState(EnemyBaseState state)
        {
            TimeInState = 0.0f;
            currentState = state;
            state.EnterState(this);
        }

        public void MoveLeft(float dt)
        {
            //float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            //Velocity -= new Vector2(horizontalMovement, 0.0f) * dt;
            Velocity = new Vector2(-MovementSpeed, Velocity.Y);
            isFacingRight = false;
        }

        public void MoveRight(float dt)
        {
            //float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            //Velocity += new Vector2(horizontalMovement, 0.0f) * dt;
            Velocity = new Vector2(MovementSpeed, Velocity.Y);
            isFacingRight = true;
        }

        public void Jump(float dt)
        {
            Velocity += new Vector2(0, JumpForce) * dt;
        }
    }
}
