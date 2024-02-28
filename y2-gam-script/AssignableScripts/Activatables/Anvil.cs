/******************************************************************************/
/*!
\par        Image Engine
\file       Anvil.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 27, 2024

\brief      Script for handling Anvil animations and sounds.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;

namespace Object
{
    public class Anvil : Entity
    {
        /*  _________________________________________________________________________ */
        /*! Anvil

        @return *this

        Default constructor for the Anvil entity. Inherits from entity class.
        */
        public Anvil() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Anvil
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Anvil entity.
        */
        public Anvil(uint entityHandle) : base(entityHandle)
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
