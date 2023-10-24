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

namespace Image
{
    [StructLayout(LayoutKind.Sequential)]
    public struct RaycastHit
    {
        public Vector3 normal;
        public Vector3 point;
        public float distance;
        public uint entityID;
        public string tag;
    }
    public static class PhysicsWrapper
    {
        public static bool Raycast(Vector3 origin, Vector3 direction, out RaycastHit result, float maxDistance)
        {
            InternalCalls.PhysicsComponent_GetRaycast(origin, direction, maxDistance, out IntPtr resultPtr);
            
            if (resultPtr != IntPtr.Zero)
            {
                result = new RaycastHit();
                IntPtr resultStructPtr = IntPtr.Add(resultPtr, Marshal.SizeOf(typeof(RaycastHit)));
                RaycastHit resultCpp = Marshal.PtrToStructure<RaycastHit>(resultStructPtr);

                result.normal = resultCpp.normal;
                result.point = resultCpp.point;
                result.distance = resultCpp.distance;
                result.entityID = resultCpp.entityID;
                result.tag = resultCpp.tag;

                return true;
            }

            else
            {
                result = new RaycastHit();
                return false;
            }
        }
    }
}
