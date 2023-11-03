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
        enemy.SetText("Idle State");
        enemy.AnimationState = (int)AnimationCode.IDLE;
    }

    public override void UpdateState(BasicEnemy enemy, float dt)
    {
        // Change direction after 2 seconds
        if (enemy.TimeInState >= 2.0f)
        {
            enemy.SwitchState(enemy.PatrolState);
        }

        // Calculate losRayEnd based on isFacingRight
        float offset = enemy.isFacingRight ? enemy.VisionRange : -enemy.VisionRange;
        Vector2 losRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2.0f) + offset, enemy.Translation.Y);

        // Perform the raycast
        PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X, enemy.Translation.Y), losRayEnd, enemy.entityID, out RaycastHit losRayCast);

        // Check the raycast result for "Player" tag
        if (losRayCast.tag == "Player")
        {
            //Console.WriteLine("Player spotted!");
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
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}