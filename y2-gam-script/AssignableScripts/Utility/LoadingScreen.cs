/******************************************************************************/
/*!
\par        Image Engine
\file       LoadingScreen.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 8, 2023

\brief      Script for loading screen.

\copyright  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class LoadingScreen : Entity
    {
        public int TOTAL_LOADING_SCREENS = 2;
        public int CurrentAnimationIndex;
        public float LoadingScreenTimer = 0;
        public float MAX_LOADING_SCREEN_DISPLAY;

        /*  _________________________________________________________________________ */
        /*! LoadingScreen

        @return *this

        Default constructor for the LoadingScreen entity. Inherits from entity class.
        */
        public LoadingScreen() : base()
        {
            
        }

        /*  _________________________________________________________________________ */
        /*! LoadingScreen
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a LoadingScreen entity.
        */
        public LoadingScreen(uint entityHandle) : base(entityHandle)
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
            var rand = new Random();        
            CurrentAnimationIndex = rand.Next(0, TOTAL_LOADING_SCREENS);
            //PlayAudio("player_hit_the_ground.wav", 0);
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
