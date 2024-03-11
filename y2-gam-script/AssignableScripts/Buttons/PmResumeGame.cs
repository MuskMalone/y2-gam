﻿/******************************************************************************/
/*!
\par        Image Engine
\file       PmResumeGame.cs

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Feb 7, 2024

\brief      For resume game button in pause menu

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;
using System.Diagnostics;
//using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Object
{
    public class PmResumeGame : Entity
    {
        //bool firstTime = true;
        //public bool isRPaused = false;
        private Vector2 temp_translation;
        //PauseMenu2 pausemenu = GameplayWrapper.FindEntityByName("PauseMenu2").As<PauseMenu2>();
        Player player;

        //private bool firstTime = true;

        /*  _________________________________________________________________________ */
        /*! PmResumeGame

        @return *this

        Default constructor for the PmResumeGame entity. Inherits from entity class.
        */
        public PmResumeGame() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! PmResumeGame
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a PmResumeGame entity.
        */
        public PmResumeGame(uint entityHandle) : base(entityHandle)
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
            Colour = new Vector4(1, 1, 1, 1);
            player = GameplayWrapper.FindEntityByName("Player").As<Player>();
            temp_translation = Translation;
        }

        //void PauseGame()
        //{

        //    if (UIHover)
        //    {
        //        //Console.WriteLine("Quitgame Hovered Over");
        //        Colour = new Vector4(1, 1, 1, 1);

        //    }
        //    else
        //    {
        //        Colour = new Vector4(1, 0, 1, 0);
        //    }
        //    if (UIClicked && firstTime)
        //    {
        //        //GameplayWrapper.SpawnPrefab("pf_confirmation", new Vector2(750, 252));
        //        //StopAudio();
        //        //LoadScene("Level1");
        //        isRPaused = false;
        //        ResumeGame();
        //        firstTime = false;
        //    }
        //}

        //void ResumeGame()
        //{
        //    Colour = new Vector4(1, 0, 1, 0);
        //}


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update.
        */
        void OnUpdate(float dt)
        {
            //if(UIClicked && firstTime)

            if (!player.isPaused)
            {
                Translation = new Vector2(99999, 99999);
                //Colour = new Vector4(1, 0, 1, 0);
            }
            else
            {
                Translation = temp_translation;
                //Colour = new Vector4(1, 1, 1, 1);
 
            }

            if (UIClicked)
            {
                Translation = new Vector2(99999, 99999);
                player.ResumeGame();
            }
            //if(Input.IsKeyClicked(KeyCode.KEY_P))
            //{
            //    if(!isRPaused)
            //    {
            //        Translation = new Vector2(1200, 381);
            //        isRPaused = true;
            //    }
            //    else
            //    {
            //        isRPaused = false;
            //    }
            //}
            //if (!isPaused)
            //{
            //    Colour = new Vector4(1, 0, 1, 0);

            //}
            //else
            //{
            //    PauseGame();
            //}

            //if (Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
            //{
            //    if (!isPaused)
            //    {
            //        SaveScene("Level1");
            //        PauseGame();
            //        isPaused = true;
            //    }
            //    else
            //    {
            //        ResumeGame();
            //        isPaused = false;
            //    }
            //    //firstTime = false;
            //}


            //if (UIClicked)
            //{
            //    if (MousePos.X < (Translation.X + Scale.X / 2) && MousePos.X > (Translation.X - Scale.X / 2) && MousePos.Y < (Translation.Y + Scale.Y / 2) && MousePos.Y > (Translation.Y - Scale.Y / 2))
            //    {
            //        Console.WriteLine("QUITCLICKED");
            //        Colour = new Vector4(1, 0, 1, 0);
            //    }
            //}
            //if (UIHover)
            //{
            //    Colour = new Vector4(1, 1, 0, 1);
            //    //SetSprite("start_glow");
            //}
            //else
            //{
            //    Colour = new Vector4(1, 0, 1, 0);
            //}
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


