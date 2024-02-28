/******************************************************************************/
/*!
\par        Image Engine
\file       Death.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 2, 2024

\brief      The script for the death animation of the player.

\copyright  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
using Image;

namespace Object
{

    public class Death : Entity
    {
        public float TimeInState;
        public float MaxDuration;
        public float BackToLifeDuration;

        public float TargetScale;
        public float StartingScale;
        public float DeathAnimationSequence = 0;
        private float MAX_DEATH_SEQUENCE = 3;
        public bool PlayLifeAnimation = false;

        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();

        /*  _________________________________________________________________________ */
        /*! Death

        @return *this

        Default constructor for the button entity. Inherits from entity class.
        */
        public Death() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Death
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a death entity.
        */
        public Death(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the death entity.
        */
        void OnCreate()
        {

        }


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the death entity.
        */
        void OnUpdate(float dt)
        {
            if (player.PlayDeathAnimation)
            {
                float t = TimeInState / MaxDuration;
                Colour = new Vector4(1, 1, 1, 1);
                Translation = new Vector2(player.Translation.X, player.Translation.Y);

                if (DeathAnimationSequence == 0)
                {
                    float easedScale = EaseInQuart(StartingScale, TargetScale, t);
                    Scale = new Vector3(easedScale, easedScale, 0);
                    TimeInState += dt;
                }

                else if (DeathAnimationSequence == 1)
                {
                    float easedScale = EaseInBounce(StartingScale, TargetScale, t);
                    Scale = new Vector3(easedScale, easedScale, 0);
                    TimeInState += dt;
                }

                else if (DeathAnimationSequence == 2)
                {
                    float easedScale = EaseInBack(StartingScale, TargetScale, t);
                    Scale = new Vector3(easedScale, easedScale, 0);
                    TimeInState += dt;
                }

                else if (DeathAnimationSequence == 3)
                {
                    float easedScale = QuickSpikeEaseOut(StartingScale, TargetScale, t);
                    Scale = new Vector3(easedScale, easedScale, 0);
                    TimeInState += dt;
                }

                if (TimeInState >= MaxDuration)
                {
                    player.PlayDeathAnimation = false;
                    PlayLifeAnimation = true;
                    TimeInState = 0;
                    DeathAnimationSequence++;

                    if (DeathAnimationSequence > MAX_DEATH_SEQUENCE || DeathAnimationSequence < 0)
                    {
                        DeathAnimationSequence = 0;
                    }
                }
            }

            else if (PlayLifeAnimation)
            {
                float t = TimeInState / BackToLifeDuration;
                Translation = new Vector2(player.Translation.X, player.Translation.Y);
                float easedScale = EaseInQuart(TargetScale, StartingScale, t);
                Scale = new Vector3(easedScale, easedScale, 0);
                TimeInState += dt;

                if (TimeInState >= BackToLifeDuration)
                {
                    PlayLifeAnimation = false;
                    TimeInState = 0;
                }
            }

            else
            {
                Colour = new Vector4(1, 1, 1, 0);
                Scale = new Vector3(0, 0, 0);
                Translation = new Vector2(player.Translation.X, player.Translation.Y);
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

        static float EaseInQuart(float start, float end, float value)
        {
            end -= start;
            return end * value * value * value * value + start;
        }

        static float EaseOutBounce(float start, float end, float value)
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

        static float EaseInBounce(float start, float end, float value)
        {
            end -= start;
            float d = 1.0f;
            return end - EaseOutBounce(0, end, d - value) + start;
        }

        static float EaseInOutBounce(float start, float end, float value)
        {
            end -= start;
            float d = 1.0f;
            if (value < d * 0.5f) return EaseInBounce(0, end, value * 2) * 0.5f + start;
            else return EaseOutBounce(0, end, value * 2 - d) * 0.5f + end * 0.5f + start;
        }

        static float EaseInBack(float start, float end, float value)
        {
            end -= start;
            value /= 1;
            float s = 1.70158f;
            return end * (value) * value * ((s + 1) * value - s) + start;
        }

        static float EaseOutBack(float start, float end, float value)
        {
            float s = 1.70158f;
            end -= start;
            value = (value) - 1;
            return end * ((value) * value * ((s + 1) * value + s) + 1) + start;
        }

        static float QuickSpikeEaseOut(float start, float end, float value)
        {
            if (value <= .2f)
                return Linear(start, end, value / .2f);

            return EaseInQuint(end, start, value / .8f);
        }

        static float Linear(float start, float end, float value)
        {
            float a = (1.0f - value) * start;
            float b = value * end;
            return a + b;
        }

        static float EaseInQuint(float start, float end, float value)
        {
            end -= start;
            return end * value * value * value * value * value + start;
        }
    }
}