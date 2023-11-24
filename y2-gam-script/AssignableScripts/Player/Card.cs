/******************************************************************************/
/*!
\par        Image Engine
\file       Card.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 23, 2023

\brief      

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;

namespace Object
{
    public class Card : Entity
    {
        private Vector2 direction;
        private float timeAlive = 0.0f;
        private bool Alive = false;

        private readonly float MAX_TIME_ALIVE = 10.0f;
        private readonly float speed = 35.0f;

        /*  _________________________________________________________________________ */
        /*! Card

        @return *this

        Default constructor for the card entity. Inherits from entity class.
        */
        public Card() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Card
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a card entity.
        */
        public Card(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the card entity.
        */
        void OnCreate()
        {
            ResetCardPos();
        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the card entity.
        */
        void OnUpdate(float dt)
        {
            if (!IsEditorMode())
            {
                if (Alive)
                {
                    // Card Related
                    timeAlive += dt;
                    Velocity += direction * speed * dt;

                    if ((timeAlive >= MAX_TIME_ALIVE))
                    {
                        ResetCardPos();
                    }

                    // Swap Related
                    if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_LEFT)) {
                        if (PhysicsWrapper.Raycast(MousePos, MousePos, entityID, out RaycastHit mouseRayCast))
                        {
                            if (GameplayWrapper.IsSwappable(mouseRayCast.id))
                            {
                                GameplayWrapper.Swap(entityID, mouseRayCast.id);
                                ResetCardPos();
                            }
                        }
                    }
                    /*
                    if (PhysicsWrapper.IsCollidedWithAnything(entityID))
                    {
                        ResetCardPos();
                    }
                    */
                    /*
                    if (!PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2), Collider.Y + (ColliderDimensions.Y / 2) + 1),
                        new Vector2(Collider.X - (ColliderDimensions.X / 2), Collider.Y - (ColliderDimensions.Y / 2) - 1), GameplayWrapper.PlayerID, out RaycastHit LeftTBRayCast) &&
                            !PhysicsWrapper.Raycast(new Vector2(Collider.X + (ColliderDimensions.X / 2), Collider.Y + (ColliderDimensions.Y / 2) + 1),
                        new Vector2(Collider.X + (ColliderDimensions.X / 2), Collider.Y - (ColliderDimensions.Y / 2) - 1), GameplayWrapper.PlayerID, out RaycastHit RightTBRayCast) &&
                        !PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2) - 1, Collider.Y + (ColliderDimensions.Y / 2)),
                        new Vector2(Collider.X + (ColliderDimensions.X / 2) + 1, Collider.Y + (ColliderDimensions.Y / 2)), GameplayWrapper.PlayerID, out RaycastHit topLeftRayCast) &&
                        !PhysicsWrapper.Raycast(new Vector2(Collider.X - (ColliderDimensions.X / 2) - 1, Collider.Y - (ColliderDimensions.Y / 2)),
                        new Vector2(Collider.X + (ColliderDimensions.X / 2) + 1, Collider.Y - (ColliderDimensions.Y / 2)), GameplayWrapper.PlayerID, out RaycastHit topRightRayCast))
                    {
                        ResetCardPos();
                    }
                    */
                }

                else
                {
                    if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_RIGHT))
                    {
                        FireCard();
                    }
                }
            }
        }

        /*  _________________________________________________________________________ */
        /*! OnExit

        @return none

        Called on exit.
        */
        void OnExit()
        {

        }

        void ResetCardPos()
        {
            Translation = new Vector2(9999, 9999);
            Collider = new Vector2(9999, 9999);
            Colour = new Vector4(1, 1, 1, 0);
            timeAlive = 0.0f;
            Alive = false;
            Velocity = new Vector2(0, 0);
        }

        void FireCard()
        {
            Translation = GameplayWrapper.PlayerPos;
            Collider = GameplayWrapper.PlayerPos;
            direction = MousePos - GameplayWrapper.PlayerPos;
            direction = PhysicsWrapper.Normalize(direction);
            Colour = new Vector4(1, 1, 1, 1);
            Alive = true;
        }
    }
}
