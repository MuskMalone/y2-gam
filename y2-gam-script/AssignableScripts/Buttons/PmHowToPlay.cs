/******************************************************************************/
/*!
\par        Image Engine
\file       PmHowToPlay.cs

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Feb 7, 2024

\brief      For how to play button in pause menu

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
    public class PmHowToPlay : Entity
    {
        //bool firstTime = true;
        //private bool isPaused = false;
        private Vector2 temp_translation;
        Player player;
        public bool hovered = false;

        //PmResumeGame resume = GameplayWrapper.FindEntityByName("PmResumeGame").As<PmResumeGame>();

        /*  _________________________________________________________________________ */
        /*! PmHowToPlay

        @return *this

        Default constructor for the PmHowToPlay entity. Inherits from entity class.
        */
        public PmHowToPlay() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! PmHowToPlay
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a PmHowToPlay entity.
        */
        public PmHowToPlay(uint entityHandle) : base(entityHandle)
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

        //void PauseGame()
        //{
        //    Translation = new Vector2(1205, 345);
        //}

        //void ResumeGame()
        //{
        //    //Colour = new Vector4(1, 0, 1, 0);
        //    Translation = temp_translation;
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
            if (!player.isPaused)
            {
                Colour = new Vector4(1, 0, 1, 0);
            }
            else
            {
                Colour = new Vector4(1, 1, 1, 1);
                if (UIClicked)
                {
                    SaveScene(InternalCalls.EngineCore_GetPrevSceneVar());
                    LoadScene("HowToPlay");
                    PlayAudio("menu_click.wav", 0, (int)ReverbCode.OFF);
                }
                if(UIHover)
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


