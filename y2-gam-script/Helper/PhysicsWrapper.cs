﻿/******************************************************************************/
/*!
\par        Image Engine
\file       PhysicsWrapper.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       March 8, 2023

\brief      Physics wrapper for the C++ physics component.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Runtime.InteropServices;

namespace Image
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RaycastHit
    {
        public uint id;
        public string tag;
        public string layer;
    }
    public static class PhysicsWrapper
    {
        public static bool IsCollidedWithAnything(uint entityID)
        {
            bool isCollided = false;
            InternalCalls.PhysicsComponent_Collided(ref entityID, ref isCollided);
            return isCollided;
        }

        public static bool IsCollidedEntity(uint lhsEntityID, uint rhsEntityID)
        {
            bool isCollided = false;
            InternalCalls.PhysicsComponent_CollidedEntity(ref lhsEntityID, ref rhsEntityID, ref isCollided);
            return isCollided;
        }

        public static bool IsCollidedWithLayer(uint entityID, string layer)
        {
            bool isCollided = false;
            InternalCalls.PhysicsComponent_CollidedLayer(ref entityID, layer, ref isCollided);
            return isCollided;
        }
        public static bool IsIntersectedWithLayer(uint entityID, string layer)
        {
            bool isCollided = false;
            InternalCalls.PhysicsComponent_IntersectedLayer(ref entityID, layer, ref isCollided);
            return isCollided;
        }

        public static bool Raycast(Vector2 origin, Vector2 end, uint optionalEntityID, out RaycastHit result)
        {
            bool hit = false;
            uint entID = 0;
            //string tagString = "";
            //string layerString = "";

            InternalCalls.PhysicsComponent_GetRaycast(ref origin, ref end, ref optionalEntityID,
                ref hit, ref entID, out String tagString, out String layerString);
            
            if (hit)
            {
                result = new RaycastHit
                {
                    //entityID = entityID,
                    //normal = normal,
                    //point = point,
                    //distance = distance,
                    id = entID,
                    tag = tagString,
                    layer = layerString
                };
            }

            else
            {
                RaycastHit rh;
                rh.tag = "";
                rh.layer = "";
                rh.id = 0;
                result = rh;
            }

            return hit;
        }

        public static Vector2 Normalize(Vector2 vector)
        {
            float length = (float)Math.Sqrt(vector.X * vector.X + vector.Y * vector.Y);
            return new Vector2(vector.X / length, vector.Y / length);
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
        public static Vector2 CalculateAcceleration(Vector2 force, float mass)
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
