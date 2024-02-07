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
    public class KingButton : Entity
    {
        LevelSelect lvlSelect = GameplayWrapper.FindEntityByName("Background").As<LevelSelect>();
        bool hovered = false;

        /*  _________________________________________________________________________ */
        /*! KingButton

        @return *this

        Default constructor for the KingButton entity. Inherits from entity class.
        */
        public KingButton() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! KingButton
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a KingButton entity.
        */
        public KingButton(uint entityHandle) : base(entityHandle)
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
            if (UIHover)
            {
                lvlSelect.CurrentAnimationIndex = (int)AnimationCodeLevelSelect.KING;
                hovered = true;

                if (UIClicked)
                {
                    LoadScene("Level2");
                }
            }

            else
            {
                if (hovered)
                {
                    lvlSelect.CurrentAnimationIndex = (int)AnimationCodeLevelSelect.NONE;
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

