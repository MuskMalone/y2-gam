/******************************************************************************/
/*!
\par        Image Engine
\file       InternalCalls.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 23, 2023

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
        #region Gameplay
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_GetPressed(out bool isPressed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_SetPressed(ref bool isPressed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_FireCard(ref Vector2 startPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_SpawnPrefab(out String fileName, out Vector2 startPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_Destroy(ref uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_GetPlayerPos(out Vector2 playerPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_GetPlayerID(out uint playerID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_GetEntityIDByTag(out uint entityID, out String tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_IsSwappable(ref uint entityHandle, out bool outIsSwappable);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_Swap(ref uint lhs, ref uint rhs);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_SlowdownTime(out bool flag);
        #endregion

        #region UI
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_GetIsUIButtonClicked(ref uint entityID, out bool outIsClicked);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_GetIsUIButtonHover(ref uint entityID, out bool outIsHover);
        #endregion

        #region Serialization
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SerializationComponent_GetIsFacingRight(ref uint entityID, out bool outFacingDirection);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SerializationComponent_SetIsFacingRight(uint entityID, ref bool facingDirection);
        #endregion

        #region EngineCore
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_GetMousePos(out Vector2 outMousePos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_PlayAudio(out String audioFileName, out int loopCount);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_LoadScene(out String sceneName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_IsEditorMode(out bool isEditorMode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_SetText(uint entityID, out String text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_Quit();
        #endregion

        #region Pathfinding
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PathfindingComponent_GetPath(ref uint entityID, out Vector2 closestNode, 
            out Vector2 nextNode, out Vector2 nodeType);
        #endregion

        #region Physics
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_GetRaycast(ref Vector2 origin, ref Vector2 end, uint optionalEntityID,
           out bool hit, out uint entityHandle, out String tag, out String layer);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_Collided(ref uint entityHandle, out bool collidedOrNot);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_GetColliderDimensions(ref uint entityHandle, out Vector2 dim);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_SetColliderDimensions(uint entityHandle, ref Vector2 dim);

        /*  _________________________________________________________________________ */
        /*! PhysicsComponent_GetColliderPos

        @param entityID
        The ID of the entity.

        @param outPos
        The current collider position of the entity.

        @return none.

        Get the collider current position of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_GetColliderPos(ref uint entityHandle, out Vector2 pos);

        /*  _________________________________________________________________________ */
        /*! PhysicsComponent_SetColliderPos

        @param entityID
        The ID of the entity.

        @param translation
        Updated  collider position of the entity.

        @return none.

        Set the collider current position of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_SetColliderPos(uint entityHandle, ref Vector2 pos);
        #endregion

        #region Graphics
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
        internal extern static void AnimationComponent_GetAnimationState(ref uint entityHandle, out int animationState);

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
        /*! GraphicsComponent_SetSprite

        @param entityHandle
        The ID of the entity.

        @param fileName
        Name of the file.

        @return none.

        Set the current sprite of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GraphicsComponent_SetSprite(uint entityHandle, out String fileName);

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
        internal extern static void GraphicsComponent_GetScale(ref uint entityHandle, out Vector3 scale);

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

        /*  _________________________________________________________________________ */
        /*! GraphicsComponent_GetRotation

        @param entityID
        The ID of the entity.

        @param outRotation
        The current rotation of the entity.

        @return none.

        Get the current rotation of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GraphicsComponent_GetRotation(ref uint entityHandle, out Vector3 rotation);

        /*  _________________________________________________________________________ */
        /*! GraphicsComponent_SetRotation

        @param entityID
        The ID of the entity.

        @param rotation
        Updated rotation of the entity.

        @return none.

        Set the current rotation of the entity in C#.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GraphicsComponent_SetRotation(uint entityHandle, ref Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GraphicsComponent_SetColour(uint entityHandle, ref Vector4 colour);
        #endregion

        #region Transform
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
        internal extern static void TransformComponent_GetTranslation(ref uint entityHandle, out Vector2 translation);

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
        internal extern static void TransformComponent_SetTranslation(uint entityHandle, ref Vector2 translation);
        #endregion

        #region Force
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
        internal extern static void ForceComponent_GetForce(ref uint entityHandle, out Vector2 force);

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

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetGravity(uint entityHandle, ref Vector2 force);

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
        internal extern static void ForceComponent_GetMass(ref uint entityHandle, out float mass);

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
        internal extern static void ForceComponent_GetVelocity(ref uint entityHandle, out Vector2 velocity);

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
