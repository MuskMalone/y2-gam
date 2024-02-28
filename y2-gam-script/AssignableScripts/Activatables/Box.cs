/******************************************************************************/
/*!
\par        Image Engine
\file       Box.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 27, 2024

\brief      Script for handling box animations and sounds.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;

namespace Object
{
    public class Box : Entity
    {
        /*  _________________________________________________________________________ */
        /*! Box

        @return *this

        Default constructor for the Box entity. Inherits from entity class.
        */
        public Box() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Box
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Box entity.
        */
        public Box(uint entityHandle) : base(entityHandle)
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
