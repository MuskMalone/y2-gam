/******************************************************************************/
/*!
\par        Image Engine
\file       CloseDoor.cs

\author      (@digipen.edu)
\date       Oct 24, 2023

\brief      The main script for destroying closed door. Has OnCreate and OnUpdate 
            functions.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;

namespace Object
{

    public class CloseDoor : Entity
    {
        /*  _________________________________________________________________________ */
        /*! CloseDoor

        @return *this

        Default constructor for the closed door entity. Inherits from entity class.
        */
        public CloseDoor() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! CloseDoor
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a closed door entity.
        */
        public CloseDoor(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the closed door entity.
        */
        // Don't worry about the 'unused' message, as the one using/referencing it is the C++ code!
        void OnCreate()
        {
            
        }


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the closed door entity.
        */
        void OnUpdate(float dt)
        {
            if (!IsEditorMode())
            {

                if (GameplayWrapper.IsPressed == true)
                {
                    GameplayWrapper.DestroyEntity(entityID);

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

    }
}
