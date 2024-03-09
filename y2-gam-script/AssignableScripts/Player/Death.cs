/******************************************************************************/
/*!
\par        Image Engine
\file       Death.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       March 8, 2024

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
                    float easedScale = Easing.EaseInQuart(StartingScale, TargetScale, t);
                    Scale = new Vector3(easedScale, easedScale, 0);
                    TimeInState += dt;
                }

                else if (DeathAnimationSequence == 1)
                {
                    float easedScale = Easing.EaseInBounce(StartingScale, TargetScale, t);
                    Scale = new Vector3(easedScale, easedScale, 0);
                    TimeInState += dt;
                }

                else if (DeathAnimationSequence == 2)
                {
                    float easedScale = Easing.EaseInBack(StartingScale, TargetScale, t);
                    Scale = new Vector3(easedScale, easedScale, 0);
                    TimeInState += dt;
                }

                else if (DeathAnimationSequence == 3)
                {
                    float easedScale = Easing.QuickSpikeEaseOut(StartingScale, TargetScale, t);
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
                float easedScale = Easing.EaseInQuart(TargetScale, StartingScale, t);
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
    }
}