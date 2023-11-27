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
        public readonly float JumpCooldown = 0.2f;
        public readonly float MovementForce = 70000.0f;
        public readonly float JumpForce = 3500000.0f;
        public readonly float VisionRange = 80.0f;
        public readonly float AttackRange = 70.0f;
        public bool isGrounded = true;

        //For pausing 
        //int temp_AnimationState = 0;
        Vector2 temp_pos;
        Vector2 temp_Force;
        Vector2 temp_velocity;
        float temp_dt = 0f;
        bool isPaused = false;
        // Time related
        public float TimeInState = 0.0f;
        public float JumpTimer = 0.0f;

        // Direction related
        //public bool directionChanged = false;
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

        // State Machines
        // All states inherit from this
        EnemyBaseState currentState;

        // States
        public EnemyDefaultState DefaultState = new EnemyDefaultState();
        public EnemyPatrolState PatrolState = new EnemyPatrolState();
        public EnemyAttackState AttackState = new EnemyAttackState();
        public EnemyChaseState ChaseState = new EnemyChaseState();
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
            isFacingRight = FacingDirection;
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
            if (!IsEditorMode())
            {
                if (isPaused)
                {
                    dt = temp_dt;
                    PauseGame();
                    //AnimationState = temp_AnimationState;
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
                        dt = temp_dt;
                        isPaused = false;
                    }
                    //firstTime = false;
                }
                if (!isPaused)
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

                    //Vector2 enemyHead = new Vector2(Collider.X, Collider.Y + (Scale.X / 2.5f));       

                    //if (PhysicsWrapper.Raycast(Collider, enemyHead, entityID, out RaycastHit anvilHit) && anvilHit.tag == "Anvil")
                    //{   
                    //    GameplayWrapper.DestroyEntity(entityID);
                    //}
                    Vector2 enemyHead;

                    if (isFacingRight)
                    {
                        // Cast the ray from the right side of the head
                        enemyHead = new Vector2(Collider.X, Collider.Y - (Scale.X / 2.5f));
                    }
                    else
                    {
                        // Cast the ray from the left side of the head
                        enemyHead = new Vector2(Collider.X, Collider.Y + (Scale.X / 2.5f));
                    }

                    if (PhysicsWrapper.Raycast(Collider, enemyHead, entityID, out RaycastHit anvilHit) && anvilHit.tag == "Anvil")
                    {
                        PlayAudio("enemy_killed.wav", 0);
                        GameplayWrapper.DestroyEntity(entityID);
                    }

                }
            }
        }
        void OnExit()
        {
            FacingDirection = isFacingRight;
        }

        void PauseGame()
        {
            //pause the game
            temp_Force = Force;
            temp_pos = Translation;
            temp_velocity = Velocity;
            //temp_AnimationState = AnimationState;
            Force = new Vector2(0, 0);
            Translation = new Vector2((float)temp_pos.X, (float)temp_pos.Y);
            Velocity = new Vector2(0, 0);
            //AnimationState = temp_AnimationState;
        }

        void ResumeGame()
        {
            Force = temp_Force;
            Translation = temp_pos;
            Velocity = temp_velocity;
            //AnimationState = temp_AnimationState;
        }
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
            float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            Force -= new Vector2(horizontalMovement, 0.0f) * dt;
            isFacingRight = false;
        }

        public void MoveRight(float dt)
        {
            float horizontalMovement = (isGrounded) ? MovementForce : MovementForce * 0.2f;
            Force += new Vector2(horizontalMovement, 0.0f) * dt;
            isFacingRight = true;
        }

        public void Jump(float dt)
        {
            Force += new Vector2(0, JumpForce) * dt;
        }
    }
}
