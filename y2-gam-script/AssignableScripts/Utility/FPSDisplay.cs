/******************************************************************************/
/*!
\par        Image Engine
\file       FPSDisplay.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 2, 2024

\brief      Simple script to display the FPS (Entity must have text component).

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class FPSDisplay : Entity
    {

        /*  _________________________________________________________________________ */
        /*! FPSDisplay

        @return *this

        Default constructor for the FPSDisplay entity. Inherits from entity class.
        */
        public FPSDisplay() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! FPSDisplay
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a FPSDisplay entity.
        */
        public FPSDisplay(uint entityHandle) : base(entityHandle)
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
            SetText("FPS: " + InternalCalls.EngineCore_GetFPS().ToString());
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
