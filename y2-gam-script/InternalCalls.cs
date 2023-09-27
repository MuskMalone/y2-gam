﻿/******************************************************************************/
/*!
\par        Image Engine
\file       InternalCalls.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      All C# internal calls go here, where information from CPP code 
            can be accessed in C#, and vice versa

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System.Runtime.CompilerServices;

namespace Image
{
    public static class InternalCalls
    {
        #region AnimationComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimationComponent_GetAnimationState(uint entityHandle, out int animationState);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimationComponent_SetAnimationState(uint entityHandle, ref int animationState);
        #endregion

        #region TransformComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(uint entityHandle, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(uint entityHandle, ref Vector3 translation);
        #endregion

        #region ForceComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_GetForce(uint entityHandle, out Vector2 force);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetForce(uint entityHandle, ref Vector2 force);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_GetMass(uint entityHandle, out float mass);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetMass(uint entityHandle, ref float mass);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_GetVelocity(uint entityHandle, out Vector2 velocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetVelocity(uint entityHandle, ref Vector2 velocity);
        #endregion

        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyPressed(int key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyClicked(int key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyReleased(int key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMousePressed(int key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseClicked(int key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseReleased(int key);
        #endregion
    }
}
