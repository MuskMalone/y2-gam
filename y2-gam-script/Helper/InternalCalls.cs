/******************************************************************************/
/*!
\par        Image Engine
\file       InternalCalls.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 26, 2023

\brief      All C# internal calls go here, where information from CPP code 
            can be accessed in C#, and vice versa

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents withthe prior
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
        internal extern static void GameplayComponent_GetPressed(ref bool isPressed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_SetPressed(ref bool isPressed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_FireCard(ref Vector2 startPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_SpawnPrefab(String fileName, ref Vector2 startPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_Destroy(ref uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_GetPlayerPos(ref Vector2 playerPos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_GetPlayerID(ref uint playerID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_GetEntityIDByTag(ref uint entityID, String tag);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_IsSwappable(ref uint entityHandle, ref bool outIsSwappable);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_Swap(ref uint lhs,ref uint rhs);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GameplayComponent_SlowdownTime(ref bool flag);
        #endregion

        #region UI
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_GetIsUIButtonClicked(ref uint entityID, ref bool outIsClicked);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void UIComponent_GetIsUIButtonHover(ref uint entityID, ref bool outIsHover);
        #endregion

        #region EngineCore
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object EngineCore_GetScriptInstance(ref uint entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_GetMousePos(ref Vector2 outMousePos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_PlayAudio(String audioFileName, ref int loopCount);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_StopAudio();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_StopAudioWithFilename(String audioFileName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_ResumeAudioWithFilename(String audioFileName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_PauseAudioWithFilename(String audioFileName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_LoadScene(String sceneName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_SaveScene(String sceneName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static String EngineCore_GetCurrentScene();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_IsEditorMode(ref bool isEditorMode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_SetText(ref uint entityID, String text);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void EngineCore_Quit();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float EngineCore_GetFPS();
        #endregion

        #region Pathfinding
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PathfindingComponent_GetPath(ref uint entityID, ref Vector2 closestNode, 
            ref Vector2 nextNode, ref Vector2 nodeType);
        #endregion

        #region Physics
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_GetRaycast(ref Vector2 origin, ref Vector2 end, ref uint optionalEntityID,
           ref bool hit, ref uint entityHandle, out String tag, out String layer);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_Collided(ref uint entityHandle, ref bool collidedOrNot);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_CollidedEntity(ref uint lhsEntityHandle, ref uint rhsEntityHandle,
            ref bool collidedOrNot);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_GetColliderDimensions(ref uint entityHandle, ref Vector2 dim);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void PhysicsComponent_SetColliderDimensions(ref uint entityHandle, ref Vector2 dim);

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
        internal extern static void PhysicsComponent_GetColliderPos(ref uint entityHandle, ref Vector2 pos);

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
        internal extern static void PhysicsComponent_SetColliderPos(ref uint entityHandle, ref Vector2 pos);
        #endregion

        #region Graphics

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimationComponent_ResetAnimationState(ref uint entityHandle);

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
        internal extern static void AnimationComponent_GetAnimationState(ref uint entityHandle, ref int animationState);

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
        internal extern static void AnimationComponent_SetAnimationState(ref uint entityHandle, ref int animationState);

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
        internal extern static void GraphicsComponent_SetSprite(ref uint entityHandle, String fileName);

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
        internal extern static void GraphicsComponent_GetScale(ref uint entityHandle, ref Vector3 scale);

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
        internal extern static void GraphicsComponent_SetScale(ref uint entityHandle, ref Vector3 scale);

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
        internal extern static void GraphicsComponent_GetRotation(ref uint entityHandle, ref Vector3 rotation);

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
        internal extern static void GraphicsComponent_SetRotation(ref uint entityHandle, ref Vector3 rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void GraphicsComponent_SetColour(ref uint entityHandle, ref Vector4 colour);
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
        internal extern static void TransformComponent_GetTranslation(ref uint entityHandle, ref Vector2 translation);

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
        internal extern static void TransformComponent_SetTranslation(ref uint entityHandle, ref Vector2 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(ref uint entityHandle, ref float rotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(ref uint entityHandle, ref float rotation);
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
        internal extern static void ForceComponent_GetForce(ref uint entityHandle, ref Vector2 force);

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
        internal extern static void ForceComponent_SetForce(ref uint entityHandle, ref Vector2 force);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ForceComponent_SetGravity(ref uint entityHandle, ref Vector2 force);

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
        internal extern static void ForceComponent_GetMass(ref uint entityHandle, ref float mass);

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
        internal extern static void ForceComponent_SetMass(ref uint entityHandle, ref float mass);

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
        internal extern static void ForceComponent_GetVelocity(ref uint entityHandle, ref Vector2 velocity);

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
        internal extern static void ForceComponent_SetVelocity(ref uint entityHandle, ref Vector2 velocity);
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
        internal extern static bool Input_IsKeyPressed(ref int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsKeyClicked

        @param key
        The current key to check.

        @return bool
        Returns true if clicked.

        Check if the key is being clicked.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyClicked(ref int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsKeyReleased

        @param key
        The current key to check.

        @return bool
        Returns true if released.

        Check if the key is being released.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyReleased(ref int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsMousePressed

        @param key
        The current key to check.

        @return bool
        Returns true if pressed.

        Check if the mouse key is being pressed.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMousePressed(ref int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsMouseClicked

        @param key
        The current key to check.

        @return bool
        Returns true if clicked.

        Check if the mouse key is being clicked.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseClicked(ref int key);

        /*  _________________________________________________________________________ */
        /*! Input_IsMouseReleased

        @param key
        The current key to check.

        @return bool
        Returns true if released

        Check if the mouse key is being released.
        */
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseReleased(ref int key);
        #endregion
    }
}
