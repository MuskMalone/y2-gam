using Image;
using System;

namespace Object
{
    public class DPlatform : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        //public float range;
        private bool hasStepped = false;
        private float timer = 2.0f;
        private float FadeOutTime = 0.0f;
        private float FadeOutTimer = 8.0f;
        private float ColourValue = 0.0f;
        private bool FadedOut = false;


        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public DPlatform() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public DPlatform(uint entityHandle) : base(entityHandle)
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
            if(player.onPlatform == true && !hasStepped)
            {
                Console.WriteLine("On platform");
                hasStepped = true;
            }
            
            if(hasStepped && !FadedOut)
            {
                timer -= dt;
                if(timer <= 0.0f)
                {
                    FadeOut(dt);
                    FadedOut = true;
                }
            }
            
            if(FadedOut)
            {
                GameplayWrapper.DestroyEntity(entityID);
            }
        }

        void FadeOut(float dt)
        {
            //FADEOUT

            //Console.WriteLine("Reached end of 23sec");
            FadeOutTime += dt;
            Console.WriteLine("FadeOutTime:" + FadeOutTime);
            if (FadeOutTime <= FadeOutTimer)
            {
                float normalizedFadeOutTime = FadeOutTime / FadeOutTimer;
                float easeOut = normalizedFadeOutTime * (2 - normalizedFadeOutTime);
                ColourValue = 1.0f - easeOut;
                ColourValue = Math.Max(ColourValue, 0.0f);
                Console.WriteLine("Colour Value: " + ColourValue);
                Colour = new Vector4(1, 1, 1, ColourValue);
            }
            //FINISH FADEOUT
            else
            {
                Colour = new Vector4(1, 1, 1, 0.0f);

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

