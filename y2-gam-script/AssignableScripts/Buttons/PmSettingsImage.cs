using Image;
using System.Configuration;

namespace Object
{
    public class PmSettingsImage : Entity
    {
        PmSettings pmsettings;
        Player player;
        public PmSettingsImage() : base()
        {

        }

        public PmSettingsImage(uint entityHandle) : base(entityHandle)
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
            pmsettings = GameplayWrapper.FindEntityByName("PmSettings").As<PmSettings>();
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
                if (pmsettings.hovered)
                {
                    AnimationState = 1;
                }
                else
                {
                    AnimationState = 0;
                }
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

