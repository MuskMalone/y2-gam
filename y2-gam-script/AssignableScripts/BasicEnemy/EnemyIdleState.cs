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

public class EnemyIdleState : EnemyBaseState
{
    public override void EnterState(BasicEnemy enemy)
    {
        enemy.AnimationState = (int)AnimationCodeEnemy.WALK;
        enemy.SetText("Idle State");
    }

    public override void UpdateState(BasicEnemy enemy, float dt)
    {
        if (enemy.TimeInState >= 2.0f)
        {
            if (enemy.IsFacingRight)
            {
                enemy.MoveLeft(dt);
            }
            else
            {
                enemy.MoveRight(dt);
            }
            enemy.SwitchState(enemy.PatrolState);
        }

        // Calculate losRayEnd based on isFacingRight
        float offset = enemy.IsFacingRight ? enemy.VisionRange : -enemy.VisionRange;
        Vector2 losRayEnd = new Vector2(enemy.Collider.X + (enemy.Scale.X / 2.0f) + offset, enemy.Collider.Y);

        // Perform the raycast
        PhysicsWrapper.Raycast(new Vector2(enemy.Collider.X, enemy.Collider.Y), losRayEnd, enemy.entityID, out RaycastHit losRayCast);

        // Check the raycast result for "Player" tag
        if (losRayCast.tag == "Player")
        {
            float attackOffset = enemy.IsFacingRight ? enemy.AttackRange : -enemy.AttackRange;
            Vector2 attackRayEnd = new Vector2(enemy.Collider.X + (enemy.Scale.X / 2.0f) + attackOffset, enemy.Collider.Y);
            PhysicsWrapper.Raycast(new Vector2(enemy.Collider.X, enemy.Collider.Y), attackRayEnd, enemy.entityID, out RaycastHit attackRayCast);

            if (attackRayCast.tag == "Player")
            {
                enemy.SwitchState(enemy.AttackState);
            }
            //else
            //{
                //enemy.SwitchState(enemy.ChaseState);
            //}
        }

        if (PhysicsWrapper.IsCollidedEntity(enemy.entityID, enemy.player.entityID))
        {
            enemy.player.Dead = true;
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}