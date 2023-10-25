/******************************************************************************/
/*!
\par        Image Engine
\file       EnemyPatrolState.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 24, 2023

\brief      The patrol state for the enemy entity.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using Object;
using System;

public class EnemyPatrolState : EnemyBaseState
{
    public override void EnterState(BasicEnemy enemy)
    {
     
    }

    public override void UpdateState(BasicEnemy enemy)
    {


        if (enemy.isFacingRight)
        {
            // Position that is 1 pixel in front of the enemy
            Vector2 groundRayPos = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2) + 10, enemy.Translation.Y - (enemy.Scale.Y / 2) - 1);
            PhysicsWrapper.Raycast(groundRayPos, groundRayPos, out RaycastHit groundRayCast);

            // Raycast for the line of sight
            Vector2 losRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2) + 30, enemy.Translation.Y + (enemy.Scale.Y / 2));
            PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X + (enemy.Scale.X / 2), enemy.Translation.Y + (enemy.Scale.Y / 2)),
                losRayEnd, out RaycastHit losRayCast);

            if (losRayCast.tag == "Player")
            {
                enemy.SwitchState(enemy.ChaseState);
            }

            enemy.MoveRight();

            if (groundRayCast.tag != "Platform")
            {
                enemy.directionChanged = true;
                enemy.MoveLeft();
                enemy.SwitchState(enemy.IdleState);
            }
        }

        else
        {
            // Position that is 1 pixel in front of the enemy
            Vector2 groundRayPos = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2) - 10, enemy.Translation.Y - (enemy.Scale.Y / 2) - 1);
            PhysicsWrapper.Raycast(groundRayPos, groundRayPos, out RaycastHit groundRayCast);

            // Raycast for the line of sight
            Vector2 losRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2) - 30, enemy.Translation.Y + (enemy.Scale.Y / 2));
            PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X + (enemy.Scale.X / 2), enemy.Translation.Y + (enemy.Scale.Y / 2)),
                losRayEnd, out RaycastHit losRayCast);

            if (losRayCast.tag == "Player")
            {
                enemy.SwitchState(enemy.ChaseState);
            }

            enemy.MoveLeft();

            if (groundRayCast.tag != "Platform")
            {
                enemy.directionChanged = true;
                enemy.MoveRight();
                enemy.SwitchState(enemy.IdleState);
            }
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}   
