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
using System;

namespace Object
{
    public class Card : Entity
    {
        private Vector2 direction;
        private float timeAlive = 0.0f;
        private bool firstTime = true;

        private readonly float MAX_TIME_ALIVE = 2.0f;
        private readonly float speed = 20.0f;

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
            direction = MousePos - Translation;
            direction = PhysicsWrapper.Normalize(direction);
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
                timeAlive += dt;
                Velocity += direction * speed * dt;

                if ((timeAlive >= MAX_TIME_ALIVE) && firstTime)
                {
                    firstTime = false;
                    DestroyEntity(entityID);
                }

                /*
                if ((PhysicsWrapper.IsCollidedWithAnything(entityID) && firstTime))
                {
                    firstTime = false;
                    DestroyEntity(entityID);
                }
                */
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
    }
}
