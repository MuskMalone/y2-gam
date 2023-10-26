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
using System.Collections.Generic;

enum NodeState
{
    WALK,
    JUMP
}

public class EnemyChaseState : EnemyBaseState
{   
    static List<Vector2> path = new List<Vector2>();
    static List<NodeState> nodeTypes = new List<NodeState>();
    public override void EnterState(BasicEnemy enemy)
    {
        InternalCalls.PathfindingComponent_GetPath(enemy.entityID, out Vector2 close, 
            out Vector2 next, out Vector2 nodeType);

        if (close == Vector2.Zero || next == Vector2.Zero)
        {
            enemy.SwitchState(enemy.IdleState);
        }

        path.Add(close);
        path.Add(next);

        nodeTypes.Add((NodeState)nodeType.X);
        nodeTypes.Add((NodeState)nodeType.Y);
    }

    public override void UpdateState(BasicEnemy enemy)
    {   
        switch (nodeTypes[0])
        {
            case NodeState.WALK:
                // Walk(enemy);
                Vector2 direction = path[0] - enemy.Translation;
                direction = PhysicsWrapper.Normalize(direction);
                enemy.Force += direction * enemy.MovementForce;
                break;

            case NodeState.JUMP:
                // Jump(enemy);
                break;
        }

    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    } 
}
