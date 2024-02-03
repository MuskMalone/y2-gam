/******************************************************************************/
/*!
\par        Image Engine
\file       EnemyAttackState.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 24, 2023

\brief      The attack state for the enemy entity.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using Object;
using System;

public class EnemyAttackState : EnemyBaseState
{
    public override void EnterState(BasicEnemy enemy)
    {
        enemy.SetText("Attack State");
        enemy.AnimationState = (int)AnimationCodeEnemy.ATTACK;
    }

    public override void UpdateState(BasicEnemy enemy, float dt)
    {
        enemy.AnimationState = (int)AnimationCodeEnemy.ATTACK;
        
        float attackOffset = enemy.IsFacingRight ? enemy.AttackRange : -enemy.AttackRange;
        Vector2 attackRayEnd = new Vector2(enemy.Collider.X + (enemy.Scale.X / 2.0f) + attackOffset, enemy.Collider.Y);
        PhysicsWrapper.Raycast(new Vector2(enemy.Collider.X, enemy.Collider.Y), attackRayEnd, enemy.entityID, out RaycastHit attackRayCast);

        if (attackRayCast.tag != "Player")
        {
            enemy.SwitchState(enemy.IdleState);
        }

        else if (attackRayCast.tag == "Player")
        {
            Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
            player.Dead = true;
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}
