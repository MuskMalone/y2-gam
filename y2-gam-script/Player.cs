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

namespace Object
{
    public class Player : Entity
    {
        bool isFacingRight = true;

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
            float speed = 500.0f;
            Vector2 forces = new Vector2(0.0f, 0.0f);

            if (Input.IsKeyClicked((KeyCode.KEY_SPACE)))
            {
                Vector2 jumpForce = new Vector2(0.0f, 3000.0f);
                Force = jumpForce * Mass;
            }

            else if (Input.IsKeyPressed((KeyCode.KEY_LEFT)))
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

            else if (Input.IsKeyPressed((KeyCode.KEY_RIGHT)))
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

            else
            {
                AnimationState = (int)AnimationCode.IDLE;
            }

            forces *= speed;
            Vector2 acceleration = CalculateAcceleration(forces, Mass);
            Vector2 velocity = Velocity;
            velocity += acceleration * dt;
            Velocity = velocity;

            if(!PhysicsWrapper.Raycast(new Vector3(0, 0, 0), new Vector3(0, 0, 0), out RaycastHit ray, 10))
            {

            }
        }

        /*  _________________________________________________________________________ */
        /*! CalculateAcceleration
        
        @param force
        The force.

        @param mass
        The mass.

        @return Vector2

        Given force and mass, calculates acceleration using f=ma.
        */
        Vector2 CalculateAcceleration(Vector2 force, float mass)
        {
            if (mass == 0.0f)
            {
                return new Vector2(0.0f, 0.0f);
            }

            Vector2 acceleration = force / mass;
            return acceleration;
        }
    }
}
