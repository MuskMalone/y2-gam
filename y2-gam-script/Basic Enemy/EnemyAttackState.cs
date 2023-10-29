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
        enemy.AnimationState = (int)AnimationCode.ATTACK;
    }

    public override void UpdateState(BasicEnemy enemy, float dt)
    {
        enemy.AnimationState = (int)AnimationCode.ATTACK;

        float attackOffset = enemy.isFacingRight ? enemy.AttackRange : -enemy.AttackRange;
        Vector2 attackRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2.0f) + attackOffset, enemy.Translation.Y);
        PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X, enemy.Translation.Y), attackRayEnd, enemy.entityID, out RaycastHit attackRayCast);

        if (attackRayCast.tag != "Player")
        {
            enemy.SwitchState(enemy.IdleState);
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}
