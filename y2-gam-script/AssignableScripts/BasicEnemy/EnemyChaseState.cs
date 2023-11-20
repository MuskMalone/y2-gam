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
using System.Collections.Generic;
using System;

enum NodeState
{
    WALK,
    JUMP
}

public class EnemyChaseState : EnemyBaseState
{   
    // Static variables
    static int currentPathIndex = 0;
    static List<Vector2> path = new List<Vector2>();
    static List<NodeState> nodeTypes = new List<NodeState>();

    public override void EnterState(BasicEnemy enemy)
    {
        enemy.SetText("Chase State");
        enemy.AnimationState = (int)AnimationCode.RUN;
        RecalculatePath(enemy);
    }

    public override void UpdateState(BasicEnemy enemy, float dt)
    {
        switch (nodeTypes[0])
        {
            case NodeState.WALK:

                Vector2 direction = new Vector2(path[currentPathIndex].X - enemy.Translation.X, 1.0f);
                float distanceToClosestNode = direction.Length();

                float thresholdForNodeCloseness = 3.0f;
                if (distanceToClosestNode < thresholdForNodeCloseness)
                {
                    currentPathIndex++;
                    if (currentPathIndex >= path.Count)
                    {
                        RecalculatePath(enemy);
                        return;
                    }

                    direction = path[currentPathIndex] - enemy.Translation;
                }

                // Distance to next node
                float distanceToNextNode = (currentPathIndex + 1 < path.Count)
                    ? (path[currentPathIndex + 1] - enemy.Translation).Length()
                    : float.MaxValue;

                if (distanceToNextNode < thresholdForNodeCloseness)
                {
                    currentPathIndex++;
                    if (currentPathIndex >= path.Count)
                    {
                        RecalculatePath(enemy);
                        return;
                    }
                    direction = path[currentPathIndex] - enemy.Translation;
                }

                direction = PhysicsWrapper.Normalize(direction);
                if (direction.X > 0.0f) // Move right
                {
                    enemy.MoveRight(dt);
                }
                else // Move left
                {
                    enemy.MoveLeft(dt);
                }

                if (enemy.JumpTimer <= 0.0f)
                { 
                    float jumpThreshold = 0.9f;
                    if (Math.Abs(direction.Y) > jumpThreshold) // Node is higher than the enemy
                    {  
                        enemy.Jump(dt);
                        enemy.JumpTimer = enemy.JumpCooldown;
                    }
                }

                // Update the jump cooldown timer
                if (enemy.JumpTimer > 0.0f)
                {
                    enemy.JumpTimer -= dt;
                }

                // Calculate losRayEnd based on isFacingRight
                float offset = enemy.isFacingRight ? enemy.AttackRange : -enemy.AttackRange;
                Vector2 losRayEnd = new Vector2(enemy.Translation.X + (enemy.Scale.X / 2.0f) + offset, enemy.Translation.Y);

                // Perform the raycast
                PhysicsWrapper.Raycast(new Vector2(enemy.Translation.X, enemy.Translation.Y), losRayEnd, enemy.entityID, out RaycastHit losRayCast);

                // Check the raycast result for "Player" tag
                if (losRayCast.tag == "Player")
                {
                    enemy.SwitchState(enemy.AttackState);
                }

                break;

            case NodeState.JUMP:
                break;
        }
    }

    public override void EnterOnCollision(BasicEnemy enemy)
    {

    }

    void RecalculatePath(BasicEnemy enemy)
    {
        path.Clear();
        nodeTypes.Clear();

        InternalCalls.PathfindingComponent_GetPath(enemy.entityID, out Vector2 close, 
            out Vector2 next, out Vector2 nodeType);

        // If the path is empty, switch back to idle state
        if (close == Vector2.Zero || next == Vector2.Zero)
        {
            enemy.SwitchState(enemy.IdleState);
            return;
        }

        // Populate the path and node type lists
        path.Add(close);
        path.Add(next);

        nodeTypes.Add((NodeState)nodeType.X);
        nodeTypes.Add((NodeState)nodeType.Y);

        // Reset the current path index
        currentPathIndex = 0;
    }
}
