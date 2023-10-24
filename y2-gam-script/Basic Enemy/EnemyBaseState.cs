/******************************************************************************/
/*!
\par        Image Engine
\file       EnemyBaseState.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 24, 2023

\brief      The base state for the enemy entity. All states inherit from this.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using Object;

public abstract class EnemyBaseState
{
    public abstract void EnterState(BasicEnemy enemy);

    public abstract void UpdateState(BasicEnemy enemy);

    public abstract void EnterOnCollision(BasicEnemy enemy);
}
