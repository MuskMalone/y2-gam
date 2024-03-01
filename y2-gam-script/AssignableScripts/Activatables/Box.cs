/******************************************************************************/
/*!
\par        Image Engine
\file       Box.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 28, 2024

\brief      Script for handling box animations and sounds.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;

namespace Object
{
    public class Box : Entity
    {
        private bool wasScaffoldingCollided = true;
        private bool wasWoodCollided = true;

        public int WoodAudioIncrement = 1;
        public int MAX_WOOD_AUDIO_FILES = 4;

        public int ScaffoldingAudioIncrement = 1;
        public int MAX_SCAFFOLDING_AUDIO_FILES = 2;

        /*  _________________________________________________________________________ */
        /*! Box

        @return *this

        Default constructor for the Box entity. Inherits from entity class.
        */
        public Box() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Box
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Box entity.
        */
        public Box(uint entityHandle) : base(entityHandle)
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
            bool isScaffoldingCollided = PhysicsWrapper.IsCollidedWithLayer(entityID, "Scaffolding");

            if (!wasScaffoldingCollided && isScaffoldingCollided)
            {
                PlayPositionalAudio("BoxDropScaffolding_" + ScaffoldingAudioIncrement + ".wav", 0, Translation);

                ScaffoldingAudioIncrement++;
                if (ScaffoldingAudioIncrement > MAX_SCAFFOLDING_AUDIO_FILES)
                {
                    ScaffoldingAudioIncrement = 1;
                }
            }

            wasScaffoldingCollided = isScaffoldingCollided;

            bool isWoodCollided = PhysicsWrapper.IsCollidedWithLayer(entityID, "Platform");

            if (!wasWoodCollided && isWoodCollided)
            {
                PlayPositionalAudio("BoxDropWood_" + WoodAudioIncrement + ".wav", 0, Translation);

                WoodAudioIncrement++;
                if (WoodAudioIncrement > MAX_WOOD_AUDIO_FILES)
                {
                    WoodAudioIncrement = 1;
                }
            }

            wasWoodCollided = isWoodCollided;
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
