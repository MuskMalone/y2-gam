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
using System.Runtime.CompilerServices;

public class EnemyPatrolState : EnemyBaseState
{
    public override void EnterState(BasicEnemy enemy)
    {
        enemy.SetText("Patrol State");
    }

    public override void UpdateState(BasicEnemy enemy, float dt)
    {
        // Calculate offsets based on isFacingRight
        float forwardOffset = enemy.isFacingRight ? 50.0f : -50.0f;
        float visionOffset = enemy.isFacingRight ? enemy.VisionRange : -enemy.VisionRange;

        // Position that is 10 pixels in front of the enemy
        Vector2 groundRayPos = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2.0f) + forwardOffset, enemy.Translation.Y - (enemy.Scale.Y / 2.0f) - 1.0f);
        PhysicsWrapper.Raycast(groundRayPos, groundRayPos, enemy.entityID, out RaycastHit groundRayCast);

        // Raycast for line of sight
        Vector2 losRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2.0f) + visionOffset, enemy.Translation.Y);
        PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X, enemy.Translation.Y), losRayEnd, enemy.entityID, out RaycastHit losRayCast);

        if (losRayCast.tag == "Player")
        {
            float attackOffset = enemy.isFacingRight ? enemy.AttackRange : -enemy.AttackRange;
            Vector2 attackRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2.0f) + attackOffset, enemy.Translation.Y);
            PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X, enemy.Translation.Y), attackRayEnd, enemy.entityID, out RaycastHit attackRayCast);

            if (attackRayCast.tag == "Player")
            {
                enemy.SwitchState(enemy.AttackState);
            }
            else
            {
                enemy.SwitchState(enemy.ChaseState);
            }
        }

        // Perform movement based on the groundRayCast result
        String platformTag = "PLATFORM";
        if (groundRayCast.tag != platformTag)
        {
            if (enemy.isFacingRight)
            {
                enemy.MoveLeft();
            }
            else
            {
                enemy.MoveRight();
            }
            enemy.SwitchState(enemy.IdleState);
        }
        else
        {
            if (enemy.isFacingRight)
            {
                enemy.MoveRight();
            }
            else
            {
                enemy.MoveLeft();
            }
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}   
