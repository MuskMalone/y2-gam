/******************************************************************************/
/*!
\par        Image Engine
\file       GameplayWrapper.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 24, 2023

\brief      Gameplay wrapper for the C++ physics component.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;

namespace Image
{
    public static class GameplayWrapper
    {
        public static void SpawnPrefab(String prefabName, Vector2 spawnPosition)
        {
            InternalCalls.GameplayComponent_SpawnPrefab(prefabName, ref spawnPosition);
        }

        public static bool IsSwappable(uint entityHandle)
        {
            bool swap = true;
            InternalCalls.GameplayComponent_IsSwappable(ref entityHandle, ref swap);
            return swap;
        }

        public static void Swap(uint lhs, uint rhs)
        {
            InternalCalls.GameplayComponent_Swap(ref lhs, ref rhs);
        }

        public static void FireCard(Vector2 startPos)
        {
            InternalCalls.GameplayComponent_FireCard(ref startPos);
        }

        public static void SlowdownTime(bool flag)
        {
            InternalCalls.GameplayComponent_SlowdownTime(ref flag);
        }

        public static void DestroyEntity(uint entID)
        {
            InternalCalls.GameplayComponent_Destroy(ref entID);
        }

        public static Vector2 PlayerPos
        {
            get
            {
                Vector2 playerPos = new Vector2();
                InternalCalls.GameplayComponent_GetPlayerPos(ref playerPos);
                return playerPos;
            }
        }

        public static uint PlayerID
        {
            get
            {
                uint playerID = 0;
                InternalCalls.GameplayComponent_GetPlayerID(ref playerID);
                return playerID;
            }
        }

        public static uint GetIDFromTag(String tag)
        {
            uint id = 0;
            InternalCalls.GameplayComponent_GetEntityIDByTag(ref id, tag);
            return id;
        }

        public static Entity FindEntityByName(String tag)
        {
            uint id = 0;
            InternalCalls.GameplayComponent_GetEntityIDByTag(ref id, tag);
            if (id == 0)
                return null;

            return new Entity(id);
        }

        public static void ResetAnimationState(uint entity)
        {
            uint ent = entity;
            InternalCalls.AnimationComponent_ResetAnimationState(ref ent);
        }
    }
}

