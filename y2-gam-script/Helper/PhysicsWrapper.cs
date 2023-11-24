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
        public Vector2 normal;
        public Vector2 point;
        public float distance;
        public uint entityID;
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
            InternalCalls.PhysicsComponent_GetRaycast(out origin, out end, out optionalEntityID,
            out bool hit, out Vector2 normal, out Vector2 point, out float distance, out uint entityID, out String tagString,
            out String layerString);
            
            if (hit)
            {
                /* Marshal method
                result = new RaycastHit();
                IntPtr resultStructPtr = IntPtr.Add(resultPtr, Marshal.SizeOf(typeof(RaycastHit)));
                RaycastHit resultCpp = Marshal.PtrToStructure<RaycastHit>(resultStructPtr);

                result.normal = resultCpp.normal;
                result.point = resultCpp.point;
                result.distance = resultCpp.distance;
                result.entityID = resultCpp.entityID;
                */

                result = new RaycastHit
                {
                    entityID = entityID,
                    normal = normal,
                    point = point,
                    distance = distance,
                    tag = tagString,
                    layer = layerString
                };

                return true;
            }

            else
            {
                result = new RaycastHit();
                return false;
            }
        }

        // Normalize function
        public static Vector2 Normalize(Vector2 vector)
        {
            float length = (float)Math.Sqrt(vector.X * vector.X + vector.Y * vector.Y);
            return new Vector2(vector.X / length, vector.Y / length);
        }
    }
}
