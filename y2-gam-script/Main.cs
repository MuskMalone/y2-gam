/******************************************************************************/
/*!
\par        Image Engine
\file       Main.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The main entity class is located here and has the getter setters 
            for all the required rigid body variables.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;

namespace Image
{
    public class Entity
    {
        protected uint entityID;

        /*  _________________________________________________________________________ */
        /*! Entity
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for entity
        */
        public Entity(uint entityHandle)
        {     
            entityID = entityHandle;
            //Console.WriteLine("Entity Non-Default, Single Parameter Constructor Called!");
        }

        /*  _________________________________________________________________________ */
        /*! Entity

        @return *this

        Default constructor for entity.
        */
        public Entity()
        {
            entityID = 0;
            //Console.WriteLine("Entity Parameterless Default Constructor Called!");
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
        public Vector2 CalculateAcceleration(Vector2 force, float mass)
        {
            if (mass == 0.0f)
            {
                return new Vector2(0.0f, 0.0f);
            }

            Vector2 acceleration = force / mass;
            return acceleration;
        }

        /*  _________________________________________________________________________ */
        /*! AnimationState

        Getter setter for AnimationState.
        */
        public int AnimationState
        {
            get
            {
                InternalCalls.AnimationComponent_GetAnimationState(entityID, out int animationState);
                return AnimationState;
            }
            set
            {
                InternalCalls.AnimationComponent_SetAnimationState(entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Scale

        Getter setter for Scale.
        */
        public Vector3 Scale
        {
            get
            {
                InternalCalls.GraphicsComponent_GetScale(entityID, out Vector3 scale);
                return scale;
            }
            set
            {
                InternalCalls.GraphicsComponent_SetScale(entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Translation

        Getter setter for Translation.
        */
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(entityID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Force

        Getter setter for Force.
        */
        public Vector2 Force
        {
            get
            {
                InternalCalls.ForceComponent_GetForce(entityID, out Vector2 force);
                return force;
            }
            set
            {
                InternalCalls.ForceComponent_SetForce(entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Mass

        Getter setter for Mass.
        */
        public float Mass
        {
            get
            {
                InternalCalls.ForceComponent_GetMass(entityID, out float mass);
                return mass;
            }
            set
            {
                InternalCalls.ForceComponent_SetMass(entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Velocity

        Getter setter for Velocity.
        */
        public Vector2 Velocity
        {
            get
            {
                InternalCalls.ForceComponent_GetVelocity(entityID, out Vector2 velocity);
                return velocity;
            }
            set
            {
                InternalCalls.ForceComponent_SetVelocity(entityID, ref value);
            }
        }
    }
}
