
using Image;
using System;

namespace Object
{
    public class CastleTimer : Entity
    {
        //Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        DPlatform dplatform = GameplayWrapper.FindEntityByName("DPlatform").As<DPlatform>();
        private string displayTime;

        /*  _________________________________________________________________________ */
        /*! Key

        @return *this

        Default constructor for the key entity. Inherits from entity class.
        */
        public CastleTimer() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Key
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a key entity.
        */
        public CastleTimer(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the key entity.
        */
        void OnCreate()
        {

            //SetText("Timer: ");
        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the key entity.
        */
        void OnUpdate(float dt)
        {
            displayTime = floor(dplatform.FadeOutClock).ToString();
            SetText("Timer: " + displayTime);
        }
        int floor(float value)
        {

            int intValue = (int)value;

            if (value >= 0 || value == intValue)
            {
                return intValue;
            }
            else
            {

                return intValue - 1;
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