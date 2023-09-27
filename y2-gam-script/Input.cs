/******************************************************************************/
/*!
\par        Image Engine
\file       Input.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      A helper wrapper class that allows the user to type fewer
            lines to obtain input status.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Image
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyPressed((int)keyCode);
        }

        public static bool IsKeyClicked(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyClicked((int)keyCode);
        }

        public static bool IsKeyReleased(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyReleased((int)keyCode);
        }

        public static bool IsMousePressed(KeyCode keyCode)
        {
            return InternalCalls.Input_IsMousePressed((int)keyCode);
        }

        public static bool IsMouseClicked(KeyCode keyCode)
        {
            return InternalCalls.Input_IsMouseClicked((int)keyCode);
        }

        public static bool IsMouseReleased(KeyCode keyCode)
        {
            return InternalCalls.Input_IsMouseReleased((int)keyCode);
        }
    }
}
