/******************************************************************************/
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
        public bool hovered = false;

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
            Colour = new Vector4(1, 1, 1, 0);
            player = GameplayWrapper.FindEntityByName("Player").As<Player>();
            temp_translation = Translation;
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
            //if(UIClicked && firstTime)

            if (!player.isPaused)
            {
                Translation = new Vector2(99999, 99999);
                Colour = new Vector4(1, 1, 1, 0);
            }
            else
            {
                Translation = new Vector2(808, 383);
                Colour = new Vector4(1, 1, 1, 1);
 
            }

            if (UIClicked)
            {
                PlayAudio("menu_click.wav", 0, (int)ReverbCode.OFF);
                Colour = new Vector4(1, 1, 1, 0);
                Translation = new Vector2(99999, 99999);
                player.ResumeGame();
            }
            if (UIHover)
            {
                hovered = true;
            }
            else
            {
                hovered = false;
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


