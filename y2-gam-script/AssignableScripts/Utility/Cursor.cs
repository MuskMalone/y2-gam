/******************************************************************************/
/*!
\par        Image Engine
\file       Cursor.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 2, 2024

\brief      Simple script to display the cursor.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;

namespace Object
{
    public class Cursor : Entity
    {

        /*  _________________________________________________________________________ */
        /*! Cursor

        @return *this

        Default constructor for the cursor entity. Inherits from entity class.
        */
        public Cursor() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Cursor
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a cursor entity.
        */
        public Cursor(uint entityHandle) : base(entityHandle)
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
            Translation = new Vector2(MousePosUI.X, MousePosUI.Y);
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
