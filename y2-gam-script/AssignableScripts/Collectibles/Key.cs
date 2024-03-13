/******************************************************************************/
/*!
\par        Image Engine
\file       Key.cs

\author     Ng Wen Wen (wenwen.ng@digipen.edu)
\date       Feb 8, 2024

\brief      For collectible keys

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;

namespace Object
{
    public class Key : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        public float Range;

        /*  _________________________________________________________________________ */
        /*! Key

        @return *this

        Default constructor for the key entity. Inherits from entity class.
        */
        public Key() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Key
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a key entity.
        */
        public Key(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the key entity.
        */
        void OnCreate()
        {

        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the key entity.
        */
        void OnUpdate(float dt)
        {
            if ((player.Collider.X <= Translation.X + Range) && (player.Collider.X >= Translation.X - Range) &&
                (player.Collider.Y <= Translation.Y + Range) && (player.Collider.Y >= Translation.Y - Range) && !player.KeyCollected)
            {
                Console.WriteLine("Key Collected");
                //player.numKey++;
                player.KeyCollected = true;
                PlayAudio("Key_Collection.wav", 0, (int)ReverbCode.OFF);
                GameplayWrapper.DestroyEntity(entityID);
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