/******************************************************************************/
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

using System;
using System.Runtime.CompilerServices;

namespace Image
{
    public static class InternalCalls
    {
        #region PhysicsComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_GetRaycast(Vector3 origin, Vector3 direction,
            float maxDistance, out IntPtr raycastHit);
        #endregion

        #region GraphicsComponent
        /*  _________________________________________________________________________ */
        /*! AnimationComponent_GetAnimationState

        @param entityHandle
        The ID of the entity.

        @param outAnimationState
        The current animation state of the entity.

        @return none.

        Get the current animation state of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimationComponent_GetAnimationState(uint entityHandle, out int animationState);

        /*  _________________________________________________________________________ */
        /*! AnimationComponent_SetAnimationState

        @param entityHandle
        The ID of the entity.

        @param animationState
        Updated animation state of the entity.

        @return none.

        Set the current animation state of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimationComponent_SetAnimationState(uint entityHandle, ref int animationState);

        /*  _________________________________________________________________________ */
        /*! GraphicsComponent_GetScale

        @param entityHandle
        The ID of the entity.

        @param scale
        Current scale of the entity.

        @return none.

        Get the current scale of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GraphicsComponent_GetScale(uint entityHandle, out Vector3 scale);

        /*  _________________________________________________________________________ */
        /*! GraphicsComponent_SetScale

        @param entityHandle
        The ID of the entity.

        @param scale
        Updated scale of the entity.

        @return none.

        Set the current scale of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GraphicsComponent_SetScale(uint entityHandle, ref Vector3 scale);
        #endregion

        #region TransformComponent
        /*  _________________________________________________________________________ */
        /*! TransformComponent_GetTranslation

        @param entityHandle
        The ID of the entity.

        @param translation
        The current position of the entity.

        @return none.

        Get the current position of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(uint entityHandle, out Vector3 translation);

        /*  _________________________________________________________________________ */
        /*! TransformComponent_SetTranslation

        @param entityHandle
        The ID of the entity.

        @param translation
        Updated position of the entity.

        @return none.

        Set the current position of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(uint entityHandle, ref Vector3 translation);
        #endregion

        #region ForceComponent
        // For Force
        /*  _________________________________________________________________________ */
        /*! ForceComponent_GetForce

        @param entityHandle
        The ID of the entity.

        @param outForce
        The current force.

        @return none.

        Get the current force of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_GetForce(uint entityHandle, out Vector2 force);

        /*  _________________________________________________________________________ */
        /*! ForceComponent_SetForce

        @param entityHandle
        The ID of the entity.

        @param force
        The updated force.

        @return none.

        Set the current force of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetForce(uint entityHandle, ref Vector2 force);

        /*  _________________________________________________________________________ */
        /*! ForceComponent_GetMass

        @param entityHandle
        The ID of the entity.

        @param mass
        The current mass.

        @return none.

        Get the current mass of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_GetMass(uint entityHandle, out float mass);

        /*  _________________________________________________________________________ */
        /*! ForceComponent_SetMass

        @param entityHandle
        The ID of the entity.

        @param mass
        The updated mass.

        @return none.

        Set the current mass of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetMass(uint entityHandle, ref float mass);

        /*  _________________________________________________________________________ */
        /*! ForceComponent_GetVelocity

        @param entityHandle
        The ID of the entity.

        @param outVelocity
        The current velocity

        @return none.

        Get the current velocity of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_GetVelocity(uint entityHandle, out Vector2 velocity);

        /*  _________________________________________________________________________ */
        /*! ForceComponent_SetVelocity

        @param entityHandle
        The ID of the entity.

        @param velocity
        The updated velocity

        @return none.

        Set the current velocity of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetVelocity(uint entityHandle, ref Vector2 velocity);
        #endregion

        #region Input
        // For Input
        /*  _________________________________________________________________________ */
        /*! Input_IsKeyPressed

        @param key
        The current key to check.

        @return bool
        Returns true if pressed.

        Check if the key is being pressed.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyPressed(int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsKeyClicked

        @param key
        The current key to check.

        @return bool
        Returns true if clicked.

        Check if the key is being clicked.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyClicked(int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsKeyReleased

        @param key
        The current key to check.

        @return bool
        Returns true if released.

        Check if the key is being released.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyReleased(int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsMousePressed

        @param key
        The current key to check.

        @return bool
        Returns true if pressed.

        Check if the mouse key is being pressed.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMousePressed(int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsMouseClicked

        @param key
        The current key to check.

        @return bool
        Returns true if clicked.

        Check if the mouse key is being clicked.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseClicked(int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsMouseReleased

        @param key
        The current key to check.

        @return bool
        Returns true if released

        Check if the mouse key is being released.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseReleased(int key);
        #endregion
    }
}
