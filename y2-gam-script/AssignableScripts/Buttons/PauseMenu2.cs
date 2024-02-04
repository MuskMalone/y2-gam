using Image;
using System;
//using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Object
{
    public class PauseMenu2 : Entity
    {
        //private bool IsKeyPressed = false;
        bool isPaused = false;
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        PmResumeGame resume = GameplayWrapper.FindEntityByName("PmResumeGame").As<PmResumeGame>();

        private Vector2 temp_translation;
        private Vector2 temp_translation2;
        
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public PauseMenu2() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public PauseMenu2(uint entityHandle) : base(entityHandle)
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
            temp_translation = Translation;
        }

        void PauseGame()
        {
            //Colour = new Vector4(1, 1, 1, 1);
            //GameplayWrapper.SpawnPrefab("Quit", new Vector2(1009, 497));
            //GameplayWrapper.SpawnPrefab("HowToPlay", new Vector2(Translation.X + 259, Translation.Y - 96));
            //temp_translation2 = new Vector2(player.Translation.X + 1000, 460);
            //Translation = temp_translation2;
            Translation = new Vector2(1000, 460);
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
            //if (!isPaused)
            //{
            //    //Colour = new Vector4(1, 0, 1, 0);
            //    ResumeGame();
            //}
            //else
            //{
            //    PauseGame();
            //}

            //if (resume.isRPaused == false)
            //{
            //    isPaused = false;
            //}

            if (!isPaused)
            {
                Colour = new Vector4(1, 0, 1, 0);

            }
            else
            {
                Colour = new Vector4(1, 1, 1, 1);
                
            }
            if (Input.IsKeyClicked(KeyCode.KEY_P))
            {
                //if (!IsKeyPressed)
                //{
                if (!isPaused)
                {
                    PauseGame();
                    isPaused = true;
                }
                else
                {                       
                    ResumeGame();
                    isPaused = false;
                }
                    //IsKeyPressed = true;
                //}
            }

            //else
            //{
            //    IsKeyPressed = false;
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



