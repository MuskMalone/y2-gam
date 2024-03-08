/******************************************************************************/
/*!
\par        Image Engine
\file       Easing.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       March 8, 2024

\brief      Library

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;

namespace Image
{
    public static class Easing
    {
        public static float EaseInQuart(float start, float end, float value)
        {
            end -= start;
            return end * value * value * value * value + start;
        }

        public static float EaseOutBounce(float start, float end, float value)
        {
            value /= 1.0f;
            end -= start;
            if (value < (1 / 2.75f))
            {
                return end * (7.5625f * value * value) + start;
            }
            else if (value < (2 / 2.75f))
            {
                value -= (1.5f / 2.75f);
                return end * (7.5625f * (value) * value + .75f) + start;
            }
            else if (value < (2.5 / 2.75))
            {
                value -= (2.25f / 2.75f);
                return end * (7.5625f * (value) * value + .9375f) + start;
            }
            else
            {
                value -= (2.625f / 2.75f);
                return end * (7.5625f * (value) * value + .984375f) + start;
            }
        }

        public static float EaseInBounce(float start, float end, float value)
        {
            end -= start;
            float d = 1.0f;
            return end - EaseOutBounce(0, end, d - value) + start;
        }

        public static float EaseInOutBounce(float start, float end, float value)
        {
            end -= start;
            float d = 1.0f;
            if (value < d * 0.5f) return EaseInBounce(0, end, value * 2) * 0.5f + start;
            else return EaseOutBounce(0, end, value * 2 - d) * 0.5f + end * 0.5f + start;
        }

        public static float EaseInBack(float start, float end, float value)
        {
            end -= start;
            value /= 1;
            float s = 1.70158f;
            return end * (value) * value * ((s + 1) * value - s) + start;
        }

        public static float EaseOutBack(float start, float end, float value)
        {
            float s = 1.70158f;
            end -= start;
            value = (value) - 1;
            return end * ((value) * value * ((s + 1) * value + s) + 1) + start;
        }

        public static float QuickSpikeEaseOut(float start, float end, float value)
        {
            if (value <= .2f)
                return Linear(start, end, value / .2f);

            return EaseInQuint(end, start, value / .8f);
        }

        public static float Linear(float start, float end, float value)
        {
            float a = (1.0f - value) * start;
            float b = value * end;
            return a + b;
        }

        public static float EaseInQuint(float start, float end, float value)
        {
            end -= start;
            return end * value * value * value * value * value + start;
        }
    }
}
