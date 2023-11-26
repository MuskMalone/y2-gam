using Image;
using System;
using static System.Runtime.CompilerServices.RuntimeHelpers;

namespace Object
{
    public class PmQuit : Entity
    {
        //bool firstTime = true;
        bool isPaused = false;
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public PmQuit() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
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

        }

        void PauseGame()
        {
            
            if (UIHover)
            {
                //Console.WriteLine("Quitgame Hovered Over");
                Colour = new Vector4(1, 1, 1, 1);
               
            }
            else
            {
                Colour = new Vector4(1, 0, 1, 0);
            }
            if (UIClicked)
            {
                GameplayWrapper.SpawnPrefab("Confirmation", new Vector2(Translation.X, Translation.Y));
                
                if (Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
                {
                    ResumeGame();
                    isPaused = false;
                    //firstTime = false;
                }
            }
        }

        void ResumeGame()
        {
            Colour = new Vector4(1, 0, 1, 0);
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
            if (!IsEditorMode())
            {
                if (!isPaused)
                {
                    Colour = new Vector4(1, 0, 1, 0);

                }
                else
                {
                    PauseGame();
                }

                if (Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
                {
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
                    //firstTime = false;
                }


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


