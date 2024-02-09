/******************************************************************************/
/*!
\par        Image Engine
\file       LockedDoor.cs

\author     Ng Wen Wen (wenwen.ng@digipen.edu)
\date       Feb 8, 2024

\brief      For locked doors that require a key to unlock

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;

namespace Object
{
    public class LockedDoor : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        private bool isUnlocked = false;
        private float doorColliderYOffset = 300.0f;

        public float Range;

        /*  _________________________________________________________________________ */
        /*! LockedDoor

        @return *this

        Default constructor for the locked door entity. Inherits from entity class.
        */
        public LockedDoor() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! LockedDoor
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a locked door entity.
        */
        public LockedDoor(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the locked door entity.
        */
        void OnCreate()
        {

        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the locked door entity.
        */
        void OnUpdate(float dt)
        {
            if ((player.Collider.X <= Translation.X + Range) && (player.Collider.X >= Translation.X - Range) &&
                (player.Collider.Y <= Translation.Y + Range) && (player.Collider.Y >= Translation.Y - Range) && player.KeyCollected && !isUnlocked)
            {
                Console.WriteLine("Door Unlocked");
                player.KeyCollected = false;
                isUnlocked = true;
                Collider = new Vector2(Collider.X, Collider.Y - doorColliderYOffset);
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