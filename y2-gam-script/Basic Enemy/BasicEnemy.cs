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
        float speed = 500.0f;
        bool isFacingRight = true;
        Vector2 forces = new Vector2(0.0f, 0.0f);

        // State Machines
        EnemyBaseState currentState;
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
            Console.WriteLine("Enemy Default Constructor Called!");
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
            currentState = PatrolState;
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
            currentState.UpdateState(this);

            forces *= speed;
            Vector2 acceleration = CalculateAcceleration(forces, Mass);
            Vector2 velocity = Velocity;
            velocity += acceleration * dt;
            Velocity = velocity;
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
            currentState = state;
            state.EnterState(this);
        }

        public void MoveLeft()
        {
            if (isFacingRight)
            {
                // Only update the scale if the direction changes
                Scale = new Vector3(-Scale.X, Scale.Y, Scale.Z);
                isFacingRight = false;
            }

            AnimationState = (int)AnimationCode.RUN;
            forces.X = -1.0f;
        }

        public void MoveRight()
        {
            if (!isFacingRight)
            {
                // Only update the scale if the direction changes
                Scale = new Vector3(-Scale.X, Scale.Y, Scale.Z);
                isFacingRight = true;
            }

            AnimationState = (int)AnimationCode.RUN;
            forces.X = 1.0f;
        }
    }
}
