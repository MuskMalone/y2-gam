/******************************************************************************/
/*!
\par        Image Engine
\file       Cutscene.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 8, 2023

\brief      Script for cutscenes.

\copyright  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class Cutscene : Entity
    {
        public int CurrentAnimationIndex = 0;
        public int LAST_CUTSCENE = 4;

        /*  _________________________________________________________________________ */
        /*! Cutscene

        @return *this

        Default constructor for the Cutscene entity. Inherits from entity class.
        */
        public Cutscene() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Cutscene
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Cutscene entity.
        */
        public Cutscene(uint entityHandle) : base(entityHandle)
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

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update.
        */
        void OnUpdate(float dt)
        {
            if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_LEFT))
            {
                CurrentAnimationIndex++;
            }

            AnimationState = CurrentAnimationIndex;

            if (CurrentAnimationIndex >= LAST_CUTSCENE)
            {
                LoadScene("LevelSelect");
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