/******************************************************************************/
/*!
\par        Image Engine
\file       Button.cs

\author      (@digipen.edu)
\date       Oct 24, 2023

\brief      The main script for button activation. Has OnCreate and OnUpdate 
            functions.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;
using System.Net.WebSockets;

namespace Object
{

    public class Button : Entity
    {

        private bool closeSpawn = true;
        private bool openSpawn = false;

        /*  _________________________________________________________________________ */
        /*! Button

        @return *this

        Default constructor for the button entity. Inherits from entity class.
        */
        public Button() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Button
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a button entity.
        */
        public Button(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the button entity.
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

        Called on every update for the button entity.
        */
        void OnUpdate(float dt)
        {
            if (!IsEditorMode())
            {

                Vector2 buttonEnd = new Vector2(Translation.X, Translation.Y + (Scale.Y / 2.0f));
                if (PhysicsWrapper.Raycast(Collider, buttonEnd, entityID, out RaycastHit buttonHit) && (buttonHit.tag == "Player" || buttonHit.tag == "SwappableBox"))
                {
                    //Console.WriteLine("Player touched a button!");
                    GameplayWrapper.IsPressed = true;

                }

                // button pressed , openSpawn is not spawned , closedSpawn is spawned 
                if (GameplayWrapper.IsPressed == true && openSpawn == false && closeSpawn == true)
                {
                    //Console.WriteLine("pressed");
                    GameplayWrapper.SpawnPrefab("OpenDoor", new Vector2(100, -33));

                    closeSpawn = false;
                    openSpawn = true;
                }

                // button not pressed , openSpawn is spawned , closedSpawn is not spawned
                if (GameplayWrapper.IsPressed == false && closeSpawn == false && openSpawn == true)
                {

                    GameplayWrapper.SpawnPrefab("CloseDoor", new Vector2(100, -33));

                    closeSpawn = true;
                    openSpawn = false;
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
