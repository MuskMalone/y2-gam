/******************************************************************************/
/*!
\par        Image Engine
\file       EnterLevelOne.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 22, 2023

\brief      Script for changing to scene.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class EnterLevelOne : Entity
    {
        bool firstTime = true;
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public EnterLevelOne() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public EnterLevelOne(uint entityHandle) : base(entityHandle)
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
            if (!IsEditorMode())
            {
                if (UIClicked && firstTime)
                {
                    LoadScene("Scene1");
                    firstTime = false;
                }

                if (UIHover)
                {
                    Colour = new Vector4(1, 1, 0, 1);
                }
                else
                {
                    Colour = new Vector4(1, 0, 1, 0);
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

