using Image;
using System;

namespace Sandbox
{
    public class Player : Entity
    {
        public Player() : base()
        {
            //Console.WriteLine("Player Default Constructor Called!");
        }
        public Player(uint entityHandle) : base(entityHandle)
        {
            //Console.WriteLine("Player Non-Default, Single Parameter Constructor Called!");
            entityID = entityHandle;
        }

        // Don't worry about the 'unused' message, as the one using/referencing it is the C++ code!
        void OnCreate()
        {
            //Console.WriteLine($"Player.OnCreate from ID: {entityID}");
        }

        void OnUpdate(float dt)
        {
            float speed = 500.0f;
            Vector2 forces = new Vector2(0.0f, 0.0f);

            if (Input.IsKeyPressed((KeyCode.KEY_LEFT)))
            {
                AnimationState = (int)AnimationCode.RUN;
                forces.X = -1.0f;
            }

            else if (Input.IsKeyPressed((KeyCode.KEY_RIGHT)))
            {
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

            // For Debugging Purposes
            //Vector3 translation = Translation;
            //Console.WriteLine($"Player.OnUpdate: {translation.X}, {translation.Y}");
        }

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
