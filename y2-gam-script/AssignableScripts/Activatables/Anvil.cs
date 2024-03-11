/******************************************************************************/
/*!
\par        Image Engine
\file       Anvil.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 28, 2024

\brief      Script for handling Anvil animations and sounds.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;

namespace Object
{
    public class Anvil : Entity
    {
        private bool wasCollided = true;
        private int AnvilAudioIncrement = 1;
        private int MAX_ANVIL_AUDIO_FILES = 2;
        /*  _________________________________________________________________________ */
        /*! Anvil

        @return *this

        Default constructor for the Anvil entity. Inherits from entity class.
        */
        public Anvil() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Anvil
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Anvil entity.
        */
        public Anvil(uint entityHandle) : base(entityHandle)
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
            bool isCollided = PhysicsWrapper.IsCollidedWithAnything(entityID);

            if (!wasCollided && isCollided)
            {
                //PlayPositionalAudio("anvil_drop.wav", 0, Translation);
                //PlayPositionalAudio("anvil_drop_" + AnvilAudioIncrement + ".wav", 0, Translation);
                PlayAudio("anvil_drop_" + AnvilAudioIncrement + ".wav", 0);
            }

            AnvilAudioIncrement++;

            if (AnvilAudioIncrement > MAX_ANVIL_AUDIO_FILES)
            {
                AnvilAudioIncrement = 1;
            }

            wasCollided = isCollided;
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
