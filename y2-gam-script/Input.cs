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
