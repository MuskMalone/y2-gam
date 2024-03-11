/******************************************************************************/
/*!
\par        Image Engine
\file       PmHowToPlayScene.cs

\author     Ng Yue Zhi (n.yuezhi@digipen.edu)
\date       Feb 7, 2024

\brief      For loading the how to play scene

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
    public class PmHowToPlayScene : Entity
    {
        //bool firstTime = true;
        //bool isPaused = false;
        /*  _________________________________________________________________________ */
        /*! PmHowToPlayScene

        @return *this

        Default constructor for the PmHowToPlayScene entity. Inherits from entity class.
        */
        public PmHowToPlayScene() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! PmHowToPlayScene
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a PmHowToPlayScene entity.
        */
        public PmHowToPlayScene(uint entityHandle) : base(entityHandle)
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
            //if (Input.IsKeyClicked(KeyCode.KEY_Q))
            //{

            if(UIClicked)
            {
                LoadScene(InternalCalls.EngineCore_GetPrevSceneVar());
            }
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


