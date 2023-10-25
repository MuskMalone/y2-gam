/******************************************************************************/
/*!
\par        Image Engine
\file       EnemyChaseState.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 24, 2023

\brief      The chase state for the enemy entity.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using Object;
using System;

public class EnemyChaseState : EnemyBaseState
{
    public override void EnterState(BasicEnemy enemy)
    {
        Console.WriteLine("Entering chase state");
    }

    public override void UpdateState(BasicEnemy enemy)
    {

    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }
}
