/******************************************************************************/
/*!
\par        Image Engine
\file       PmQuit.cs

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
    public class PmQuit : Entity
    {
        //bool firstTime = true;
        //private bool isPaused = false;
        public bool quitClicked = false;
        private Vector2 temp_translation;
        //PmResumeGame resume = GameplayWrapper.FindEntityByName("PmResumeGame").As<PmResumeGame>();
        Player player;
        /*  _________________________________________________________________________ */
        /*! PmQuit

        @return *this

        Default constructor for the PmQuit entity. Inherits from entity class.
        */
        public PmQuit() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! PmQuit
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a PmQuit entity.
        */
        public PmQuit(uint entityHandle) : base(entityHandle)
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
            Translation = new Vector2(1202, 425);
            //Translation = new Vector2(1500, 500);
            //if (UIHover)
            //{
            //    //Console.WriteLine("Quitgame Hovered Over");
            //    Colour = new Vector4(1, 1, 1, 1);

            //}
            //else
            //{
            //    Colour = new Vector4(1, 0, 1, 0);
            ////}
            //if (UIClicked)
            //{
            //    GameplayWrapper.SpawnPrefab("Confirmation", new Vector2(Translation.X, Translation.Y));

            //    if (Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
            //    {
            //        ResumeGame();
            //        isPaused = false;
            //        //firstTime = false;
            //    }
            //}
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
                Translation = new Vector2(99999, 99999);
                //Colour = new Vector4(1, 0, 1, 0);

            }
            else
            {
                Translation = new Vector2(807, 425);
                //Colour = new Vector4(1, 1, 1, 1);
                if (UIClicked)
                {
                    Translation = new Vector2(99999, 99999);
                    quitClicked = true; 
                }
            }

            //if (resume.isRPaused == false)
            //{
            //    isPaused = false;
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


