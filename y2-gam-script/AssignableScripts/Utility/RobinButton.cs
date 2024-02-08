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

namespace Object
{
    public class RobinButton : Entity
    {
        LevelSelect lvlSelect = GameplayWrapper.FindEntityByName("Background").As<LevelSelect>();
        bool hovered = false;

        public float LoadingScreenTimer = 0;
        public float MAX_LOADING_SCREEN_DISPLAY_TIME;
        bool TimerStart = false;

        /*  _________________________________________________________________________ */
        /*! RobinButton

        @return *this

        Default constructor for the RobinButton entity. Inherits from entity class.
        */
        public RobinButton() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! RobinButton
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a RobinButton entity.
        */
        public RobinButton(uint entityHandle) : base(entityHandle)
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
            if (!TimerStart)
            {
                if (UIHover)
                {
                    lvlSelect.CurrentAnimationIndex = (int)AnimationCodeLevelSelect.ROBIN;
                    hovered = true;

                    if (UIClicked)
                    {
                        TimerStart = true;
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

            else
            {
                LoadingScreenTimer += dt;
                lvlSelect.CurrentAnimationIndex = (int)AnimationCodeLevelSelect.ROBINLOAD;

                if (LoadingScreenTimer >= MAX_LOADING_SCREEN_DISPLAY_TIME)
                {
                    LoadingScreenTimer = 0;
                    LoadScene("Level1");
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

