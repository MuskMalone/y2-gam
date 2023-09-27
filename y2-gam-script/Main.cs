﻿using System;

namespace Image
{
    public class Entity
    {
        protected uint entityID;
        public Entity(uint entityHandle)
        {     
            entityID = entityHandle;
            Console.WriteLine("Entity Non-Default, Single Parameter Constructor Called!");
        }
        public Entity()
        {
            entityID = 0;
            Console.WriteLine("Entity Parameterless Default Constructor Called!");
        }

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