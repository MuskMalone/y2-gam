﻿/******************************************************************************/
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
        /*  _________________________________________________________________________ */
        /*! IsKeyPressed

        @param keyCode
        The current key to check.

        @return bool
        Returns true if pressed.

        Check if the key is being pressed.
        */
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyPressed((int)keyCode);
        }

        /*  _________________________________________________________________________ */
        /*! IsKeyClicked

        @param keyCode
        The current key to check.

        @return bool
        Returns true if clicked.

        Check if the key is being clicked.
        */
        public static bool IsKeyClicked(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyClicked((int)keyCode);
        }

        /*  _________________________________________________________________________ */
        /*! IsKeyReleased

        @param keyCode
        The current key to check.

        @return bool
        Returns true if released.

        Check if the key is being released.
        */
        public static bool IsKeyReleased(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyReleased((int)keyCode);
        }

        /*  _________________________________________________________________________ */
        /*! IsMousePressed

        @param keyCode
        The current key to check.

        @return bool
        Returns true if pressed.

        Check if the mouse key is being pressed.
        */
        public static bool IsMousePressed(KeyCode keyCode)
        {
            return InternalCalls.Input_IsMousePressed((int)keyCode);
        }

        /*  _________________________________________________________________________ */
        /*! IsMouseClicked

        @param keyCode
        The current key to check.

        @return bool
        Returns true if clicked.

        Check if the mouse key is being clicked.
        */
        public static bool IsMouseClicked(KeyCode keyCode)
        {
            return InternalCalls.Input_IsMouseClicked((int)keyCode);
        }

        /*  _________________________________________________________________________ */
        /*! IsMouseReleased

        @param keyCode
        The current key to check.

        @return bool
        Returns true if released

        Check if the mouse key is being released.
        */
        public static bool IsMouseReleased(KeyCode keyCode)
        {
            return InternalCalls.Input_IsMouseReleased((int)keyCode);
        }
    }
}
