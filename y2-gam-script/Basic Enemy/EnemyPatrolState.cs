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

public class EnemyPatrolState : EnemyBaseState
{
    public override void EnterState(BasicEnemy enemy)
    {

    }

    public override void UpdateState(BasicEnemy enemy)
    {
        enemy.MoveLeft();
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}   
