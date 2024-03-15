using Image;
using System.Configuration;

namespace Object
{
    public class PmPauseImage : Entity
    {
        Player player;
        public PmPauseImage() : base()
        {

        }

        public PmPauseImage(uint entityHandle) : base(entityHandle)
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
            Colour = new Vector4(1, 1, 1, 0);
            player = GameplayWrapper.FindEntityByName("Player").As<Player>();
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
            if (!player.isPaused)
            {
                Colour = new Vector4(1, 1, 1, 0);
            }
            else
            {
                Colour = new Vector4(1, 1, 1, 1);

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

