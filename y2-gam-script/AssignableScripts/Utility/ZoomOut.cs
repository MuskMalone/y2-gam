/******************************************************************************/
/*!
\par        Image Engine
\file       ZoomOut.cs

\author     Darren Ng (ng.m@digipen.edu)
\date       Feb 9, 2024

\brief      Simple script to display the ZoomOut.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class ZoomOut : Entity
    {
        public float zoomLevel;
        public float timer;
        public float maxTimer;
        private bool toZoom=false;
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        /*  _________________________________________________________________________ */
        /*! ZoomOut

        @return *this

        Default constructor for the ZoomOut entity. Inherits from entity class.
        */
        public ZoomOut() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! ZoomOut
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a ZoomOut entity.
        */
        public ZoomOut(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation.
        */

        void OnCreate()
        {

        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update.
        */
        void OnUpdate(float dt)
        {

            if (PhysicsWrapper.IsCollidedEntity(entityID, player.entityID))
            {
                Collider = new Vector2(3000.0f, 3000.0f);
                toZoom = true;
            }

            if (toZoom)
            {

                timer += dt;
                InternalCalls.GraphicsComponent_SetZoom(Linear(0.55f, zoomLevel,timer / maxTimer));
                if (timer >= maxTimer)
                {
                    timer = 0;
                    toZoom = false;
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

        static float Linear(float start, float end, float value)
        {
            float a = (1.0f - value) * start;
            float b = value * end;
            return a + b;
        }
    }
}
