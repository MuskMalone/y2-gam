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
            InternalCalls.GameplayComponent_SpawnPrefab(out prefabName, out spawnPosition);
        }

        public static bool IsSwappable(uint entityHandle)
        {
            InternalCalls.GameplayComponent_IsSwappable(ref entityHandle, out bool isCollided);
            return isCollided;
        }

        public static void Swap(uint lhs, uint rhs)
        {
            InternalCalls.GameplayComponent_Swap(ref lhs, ref rhs);
        }

        public static void FireCard(Vector2 startPos)
        {
            InternalCalls.GameplayComponent_FireCard(ref startPos);
        }

        public static void DestroyEntity(uint entID)
        {
            InternalCalls.GameplayComponent_Destroy(ref entID);
        }

        public static Vector2 PlayerPos
        {
            get
            {
                InternalCalls.GameplayComponent_GetPlayerPos(out Vector2 playerPos);
                return playerPos;
            }
        }

        public static uint PlayerID
        {
            get
            {
                InternalCalls.GameplayComponent_GetPlayerID(out uint playerID);
                return playerID;
            }
        }

        public static uint GetIDFromTag(String tag)
        {
            InternalCalls.GameplayComponent_GetEntityIDByTag(out uint id, out tag);
            return id;
        }

        public static void SlowdownTime(bool flag)
        {
            InternalCalls.GameplayComponent_SlowdownTime(out flag);
        }
    }
}

