/******************************************************************************/
/*!
\par        Image Engine
\file       Swappable.cs

\author     (@digipen.edu)
\date       Jan 21, 2024

\brief      For the swap animation change state.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;

namespace Object
{

    public class Swappable : Entity
    {

        /*  _________________________________________________________________________ */
        /*! Swappable

        @return *this

        Default constructor for the swappable entity. Inherits from entity class.
        */
        public Swappable() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Swappable
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a swappable entity.
        */
        public Swappable(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the swappable entity.
        */
        void OnCreate()
        {
            AnimationState = (int)AnimationCodeSwappable.APPEAR;
        }


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the swappable entity.
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