/******************************************************************************/
/*!
\par        Image Engine
\file       LevelSelect.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 7, 2023

\brief      Script for changing to scene.

\copyright  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class LevelSelect : Entity
    {
        public int CurrentAnimationIndex = (int)AnimationCodeLevelSelect.NONE;
        public bool LoadingScreenActive = false;

        /*  _________________________________________________________________________ */
        /*! LevelSelect

        @return *this

        Default constructor for the LevelSelect entity. Inherits from entity class.
        */
        public LevelSelect() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! LevelSelect
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a LevelSelect entity.
        */
        public LevelSelect(uint entityHandle) : base(entityHandle)
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
            AnimationState = CurrentAnimationIndex;
            if (CurrentAnimationIndex == (int)AnimationCodeLevelSelect.KINGLOAD ||
                CurrentAnimationIndex == (int)AnimationCodeLevelSelect.ROBINLOAD)
            {
                PlayAudio("LevelTransition.wav", 0);
            }
        }

        /*  _________________________________________________________________________ */
        /*! OnExit

        @return none

        Called on exit.
        */
        void OnExit()
        {
            StopAudioWithFilename("LevelTransition.wav");
        }
    }
}


