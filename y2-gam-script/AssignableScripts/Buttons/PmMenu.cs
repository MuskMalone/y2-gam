/******************************************************************************/
/*!
\par        Image Engine
\file       PmMenu.cs

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Feb 7, 2024

\brief      For menu button in pause menu

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
    public class PmMenu : Entity
    {
        //bool firstTime = true;
        //private bool isPaused = false;
        private Vector2 temp_translation;
        //private Vector2 temp_translation2;
        public bool hovered = false;
        Player player;
        //PmResumeGame resume = GameplayWrapper.FindEntityByName("PmResumeGame").As<PmResumeGame>();
        /*  _________________________________________________________________________ */
        /*! PmMenu

        @return *this

        Default constructor for the PmMenu entity. Inherits from entity class.
        */
        public PmMenu() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! PmMenu
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a PmMenu entity.
        */
        public PmMenu(uint entityHandle) : base(entityHandle)
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
            Colour = new Vector4(1, 0, 1, 0);
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
            if (!player.isPaused)
            {
                Colour = new Vector4(1, 0, 1, 0);

            }
            else
            {
                Colour = new Vector4(1, 1, 1, 1);
                if (UIClicked)
                {
                    LoadScene("MainMenu");
                    PlayAudio("menu_click.wav", 0);
                    //GameplayWrapper.SpawnPrefab("pf_confirmation", new Vector2(750, 252));
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


