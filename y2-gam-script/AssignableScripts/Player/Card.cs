/******************************************************************************/
/*!
\par        Image Engine
\file       Card.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 23, 2023

\brief      Script for the card. Includes the UI card.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class Card : Entity
    {
        private Vector2 direction;
        private float timeAlive = 0.0f;
        private bool Alive = false;

        private readonly float MAX_TIME_ALIVE = 3.5f;
        private readonly float speed = 35.0f;

        private uint CardUIID;
        private Vector3 CardUIMaxScale;

        private bool firstTime = true;

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
            if (firstTime)
            {
                CardUIID = GameplayWrapper.GetIDFromTag("CardUI");
                CardUIMaxScale = GetScaleFromEntity(CardUIID);
                firstTime = false;
            }

            if (!IsEditorMode())
            {
                if (Alive)
                {
                    // Card Related
                    timeAlive += dt;
                    Velocity += direction * speed * dt;

                    SetEntityColour(CardUIID,
                        new Vector4(1, 1, 1, (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, 1)));
                    
                    SetScaleFromEntity(CardUIID, new Vector3(
                        (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, CardUIMaxScale.X),
                        (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, CardUIMaxScale.Y), 1));
                    
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
                                ResetCardUI();
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
                        //SetEntityColour(GameplayWrapper.GetIDFromTag("CardUI"), new Vector4(1, 1, 1, 0));
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

        void ResetCardUI()
        {
            SetEntityColour(CardUIID, new Vector4(1, 1, 1, 1));
            SetScaleFromEntity(CardUIID, new Vector3(CardUIMaxScale.X, CardUIMaxScale.Y, 1));
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

        static double ScaleToRange(double value, double oldMin, double oldMax, double newMin, double newMax)
        {
            if (oldMin == oldMax)
                return newMin;

            double normalizedValue = (value - oldMin) / (oldMax - oldMin);
            double scaledValue = normalizedValue * (newMax - newMin) + newMin;

            return Math.Min(Math.Max(scaledValue, newMin), newMax);
        }
    }
}
