/******************************************************************************/
/*!
\par        Image Engine
\file       PhysicsWrapper.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 24, 2023

\brief      Physics wrapper for the C++ physics component.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Image
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RaycastHit
    {
        //public Vector2 normal;
        //public Vector2 point;
        //public float distance;
        //public int entityID;
        public string tag;
        public string layer;
    }
    public static class PhysicsWrapper
    {
        public static bool IsCollidedWithAnything(uint entityID)
        {
            InternalCalls.PhysicsComponent_Collided(ref entityID, out bool isCollided);
            return isCollided;
        }
        public static bool Raycast(Vector2 origin, Vector2 end, uint optionalEntityID, out RaycastHit result)
        {
            InternalCalls.PhysicsComponent_GetRaycast(ref origin, ref end, optionalEntityID,
                out bool hit, out String tagString, out String layerString);
            
            if (hit)
            {
                result = new RaycastHit
                {
                    //entityID = entityID,
                    //normal = normal,
                    //point = point,
                    //distance = distance,
                    tag = tagString,
                    layer = layerString
                };
            }

            else
            {
                RaycastHit rh;
                rh.tag = "";
                rh.layer = "";
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
