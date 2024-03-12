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
        enemy.SetText("Patrol State");
        enemy.AnimationState = (int)AnimationCodeEnemy.RUN;
    }

    public override void UpdateState(BasicEnemy enemy, float dt)
    {
        // Calculate offsets based on isFacingRight
        float visionOffset = enemy.IsFacingRight ? enemy.VisionRange : -enemy.VisionRange;

        // Raycast for line of sight
        Vector2 losRayEnd = new Vector2(enemy.Collider.X + (enemy.Scale.X / 2.0f) + visionOffset, enemy.Collider.Y + enemy.VisionHeightOffset);
        PhysicsWrapper.Raycast(new Vector2(enemy.Collider.X, enemy.Collider.Y + enemy.VisionHeightOffset), losRayEnd, enemy.entityID, out RaycastHit losRayCast);

        if (losRayCast.tag == "Player")
        {
            float attackOffset = enemy.IsFacingRight ? enemy.AttackRange : -enemy.AttackRange;
            Vector2 attackRayEnd = new Vector2(enemy.Collider.X + (enemy.Scale.X / 2.0f) + attackOffset, enemy.Collider.Y);
            PhysicsWrapper.Raycast(new Vector2(enemy.Collider.X, enemy.Collider.Y), attackRayEnd, enemy.entityID, out RaycastHit attackRayCast);

            if (attackRayCast.tag == "Player")
            {
                enemy.SwitchState(enemy.AttackState);
            }         
        }

        if (PhysicsWrapper.IsCollidedEntity(enemy.entityID, enemy.player.entityID))
        {
            enemy.player.Dead = true;
        }

        Vector2 losGround = new Vector2(enemy.Collider.X + (enemy.Scale.X / 2.0f) + visionOffset, enemy.Collider.Y + enemy.GroundHeightOffset);
        PhysicsWrapper.Raycast(losGround, losGround, enemy.entityID, out RaycastHit losGroundCast);

        // Raycast for ground
        if (losRayCast.layer == "Scaffolding" || losRayCast.layer == "Platform")
        {
            enemy.SwitchState(enemy.IdleState);
        }

        else if (losGroundCast.layer != "Scaffolding" && losGroundCast.layer != "Platform")
        {
            enemy.SwitchState(enemy.IdleState);
        }
     
        if (enemy.IsFacingRight)
        {
            enemy.MoveRight(dt);
        }
        else
        {
            enemy.MoveLeft(dt);
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}   
