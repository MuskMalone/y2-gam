/******************************************************************************/
/*!
\par        Image Engine
\file       

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The main script for a ‘player’ entity. Has OnCreate and OnUpdate 
            functions. Currently the player input is located here.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class Button : Entity
    {
        

        public Button() : base()
        {
            Console.WriteLine("Button Default Constructor Called!");
        }

        public Button(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

   
        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {
            //LoadScene();
            Console.WriteLine("ACTIVATED");
        }

        void OnExit()
        {
            
        }

    }
}
