/******************************************************************************/
/*!
\par        Image Engine
\file       EnemyIdleState.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 24, 2023

\brief      The idle state for the enemy entity.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using Object;
using System;

public class EnemyIdleState : EnemyBaseState
{
    public override void EnterState(BasicEnemy enemy)
    {
        enemy.AnimationState = (int)AnimationCode.IDLE;
    }

    public override void UpdateState(BasicEnemy enemy)
    {
        // Change direction after 2 seconds
        if (enemy.TimeInState >= 2.0f)
        {
            enemy.SwitchState(enemy.PatrolState);
        }

        if (enemy.isFacingRight)
        {
            // Raycast for the line of sight
            Vector2 losRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2) + 30, enemy.Translation.Y + (enemy.Scale.Y / 2));
            PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X + (enemy.Scale.X / 2), enemy.Translation.Y + (enemy.Scale.Y / 2)),
                losRayEnd, out RaycastHit losRayCast);

            if (losRayCast.tag == "Player")
            {
                enemy.SwitchState(enemy.ChaseState);
            }
        }

        else
        {
            // Raycast for the line of sight
            Vector2 losRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2) - 30, enemy.Translation.Y + (enemy.Scale.Y / 2));
            PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X + (enemy.Scale.X / 2), enemy.Translation.Y + (enemy.Scale.Y / 2)),
                losRayEnd, out RaycastHit losRayCast);

            if (losRayCast.tag == "Player")
            {
                enemy.SwitchState(enemy.ChaseState);
            }
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}