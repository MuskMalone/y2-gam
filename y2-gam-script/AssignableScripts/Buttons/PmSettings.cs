/******************************************************************************/
/*!
\par        Image Engine
\file       PmMenu.cs

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Feb 7, 2024

\brief      For menu button in pause menu

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
    public class PmSettings : Entity
    {
        private Vector2 temp_translation;
        Player player;
        //PmSettingsImage pmsettingsimage;
        public bool hovered = false;

        /*  _________________________________________________________________________ */
        /*! PmMenu

        @return *this

        Default constructor for the PmMenu entity. Inherits from entity class.
        */
        public PmSettings() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! PmMenu
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a PmMenu entity.
        */
        public PmSettings(uint entityHandle) : base(entityHandle)
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
            //pmsettingsimage = GameplayWrapper.FindEntityByName("PmSettingsImage").As<PmSettingsImage>();
            temp_translation = Translation;
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
            if (player.isPaused)
            {
                if (UIClicked)
                {
                    
                    SaveScene(InternalCalls.EngineCore_GetPrevSceneVar());
                    LoadScene("Settings");
                    PlayAudio("menu_click.wav", 0, (int)ReverbCode.OFF);
                }

                if (UIHover)
                {
                    hovered = true;
                }
                else
                {
                    hovered = false;
                }
                //    pmsettingsimage.AnimationState = 1;
                //}
                //else
                //{
                //    pmsettingsimage.AnimationState = 0;
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


