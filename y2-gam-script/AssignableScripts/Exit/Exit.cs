/******************************************************************************/
/*!
\par        Image Engine
\file       Exit.cs

\author      (@digipen.edu)
\date       Oct 24, 2023

\brief      The main script for exit. Has OnCreate and OnUpdate 
            functions.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;
using System;

namespace Object
{

    public class Exit : Entity
    {
        private string scene;
        /*  _________________________________________________________________________ */
        /*! Exit

        @return *this

        Default constructor for the exit entity. Inherits from entity class.
        */
        public Exit() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Exit
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a exit entity.
        */
        public Exit(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the exit entity.
        */
        // Don't worry about the 'unused' message, as the one using/referencing it is the C++ code!
        void OnCreate()
        {
            scene = GetCurrentScene();
        }


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the exit entity.
        */
        void OnUpdate(float dt)
        {
            Vector2 exitEnd = new Vector2(Collider.X - (Scale.X / 2.0f), Collider.Y);
            if (PhysicsWrapper.Raycast(Collider, exitEnd, entityID, out RaycastHit exitHit) && exitHit.tag == "Player")
            {
                //Console.WriteLine("Exit");
                if (scene == "Level1")
                {
                    
                    //load lvl2 till lvl1transition player script is ok
                    LoadScene("Level2");
                    //LoadScene("Level1Transition");

                }
           
            }

            //exitEnd = new Vector2(Collider.X - (Scale.Y / 2.0f), Collider.Y);
            if (PhysicsWrapper.IsCollidedWithAnything(entityID))
            {
                //if (scene == "Level1Transition")
                //{
                //    LoadScene("Level2");

                //}
                //if (scene == "Level2")
                //{
                //    LoadScene("MainMenu");

                //}
                if (scene == "Level2")
                {
                    LoadScene("Level1Transition");

                }
                if (scene == "Level1Transition")
                {
                    LoadScene("MainMenu");

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
