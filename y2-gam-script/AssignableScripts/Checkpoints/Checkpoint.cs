/******************************************************************************/
/*!
\par        Image Engine
\file       Checkpoint.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Jan 21, 2024

\brief      The main script for a ‘checkpoint’ entity. Has OnCreate and OnUpdate 
            functions.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;

namespace Object
{
    public class Checkpoint : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        public float Range;

        /*  _________________________________________________________________________ */
        /*! Checkpoint

        @return *this

        Default constructor for the checkpoint entity. Inherits from entity class.
        */
        public Checkpoint() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Checkpoint
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a checkpoint entity.
        */
        public Checkpoint(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the checkpoint entity.
        */
        void OnCreate()
        {

        }


        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the checkpoint entity.
        */
        void OnUpdate(float dt)
        {
            if ((player.Collider.X <= Translation.X + Range) && (player.Collider.X >= Translation.X - Range) && 
                (player.Collider.Y <= Translation.Y + Range) && (player.Collider.Y >= Translation.Y - Range))
            {
                AnimationState = (int)AnimationCodeCheckpoint.ACTIVATED;
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
