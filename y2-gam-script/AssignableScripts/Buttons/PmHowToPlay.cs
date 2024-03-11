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
            player = GameplayWrapper.FindEntityByName("Player").As<Player>();
            temp_translation = Translation;
        }

        void PauseGame()
        {

            //if (UIHover)
            //{
            //    //Console.WriteLine("Quitgame Hovered Over");
            //    Colour = new Vector4(1, 1, 1, 1);

            //}
            //else
            //{
            //    Colour = new Vector4(1, 0, 1, 0);
            //}
            //if (UIClicked)
            //{

            //    GameplayWrapper.SpawnPrefab("HowToPlay", new Vector2(750, 252));
            //    //if(Input.IsKeyClicked(KeyCode.KEY_Q))
            //    //{

            //    //}
            //    //LoadScene("HowToPlay");
            //}
            Translation = new Vector2(1205, 345);
        }

        void ResumeGame()
        {
            //Colour = new Vector4(1, 0, 1, 0);
            Translation = temp_translation;
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
                    SaveScene("Level1");
                    LoadScene("HowToPlay");
                    //StopAudio();
                    //LoadScene("HowToPlay");
                    //GameplayWrapper.SpawnPrefab("pf_confirmation", new Vector2(750, 252));
                }
            }

            //if (resume.isRPaused == false)
            //{
            //    isPaused = false;
            //}
            //else
            //{
            //    isPaused = true;
            //}

            //if (Input.IsKeyClicked(KeyCode.KEY_P))
            //{
            //    if (!isPaused)
            //    {
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


