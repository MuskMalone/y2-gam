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
using System.Threading;

namespace Object
{

    public class Death : Entity
    {
        public float TimeInState;
        public float MaxDuration;
        public float ScaleSpeed;
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
            //Translation = new Vector2(832.334f, 440f); //The centre of the UI screen space
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
                Colour = new Vector4(1, 1, 1, 1);
                Scale = new Vector3(Scale.X + ScaleSpeed, Scale.Y + ScaleSpeed, 0);
                Translation = new Vector2(player.Translation.X, player.Translation.Y);
                TimeInState += dt;

                if (TimeInState >= MaxDuration)
                {
                    player.PlayDeathAnimation = false;
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