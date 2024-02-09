﻿/******************************************************************************/
/*!
\par        Image Engine
\file       PmConfirmationWindow.cs

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Feb 7, 2024

\brief      For quit button in pause menu

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Object
{
    public class PmConfirmationWindow : Entity
    {
        //bool firstTime = true;
        //bool isPaused = true;
        PmQuit quit = GameplayWrapper.FindEntityByName("PmQuit").As<PmQuit>();
        /*  _________________________________________________________________________ */
        /*! PmConfirmationWindow

        @return *this

        Default constructor for the PmConfirmationWindow entity. Inherits from entity class.
        */
        public PmConfirmationWindow() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! PmConfirmationWindow
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a PmConfirmationWindow entity.
        */
        public PmConfirmationWindow(uint entityHandle) : base(entityHandle)
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
            //GameplayWrapper.SpawnPrefab("ConfirmationYes", new Vector2(Translation.X - 150, Translation.Y - 170));
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
            //if(isPaused)
            //Console.WriteLine("QuitClicked= " + quit.quitClicked);
            if (quit.quitClicked == true)
            {
                Colour = new Vector4(1, 1, 1, 1);
                if (Input.IsKeyClicked(KeyCode.KEY_Y))
                {
                    QuitGame();
                }
                //resume game
                else if (Input.IsKeyClicked(KeyCode.KEY_N) || Input.IsKeyClicked(KeyCode.KEY_P))
                {
                    
                    //Colour = new Vector4(1, 1, 1, 0);
                    //GameplayWrapper.DestroyEntity(entityID);
                    quit.quitClicked = false;
                    //Console.WriteLine("2QuitClicked= " + quit.quitClicked);
                }
            }
            else if (quit.quitClicked == false)
            {
                //Console.WriteLine("here");
                Colour = new Vector4(1, 0, 1, 0);
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


