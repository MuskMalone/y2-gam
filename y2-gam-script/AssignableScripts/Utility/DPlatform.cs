using Image;
using System;

namespace Object
{
    public class DPlatform : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        //public float range;
        private bool hasStepped = false;
        private float timer = 0.0f;
        private float FadeOutTime = 0.0f;
        private float FadeOutTimer = 3.0f;
        private float ColourValue = 0.0f;
        private bool FadedOut = false;
        private Vector2 temp_translation;


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
            temp_translation = Translation;
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
            
            if(hasStepped)
            {
                if (!FadedOut)
                {
                    timer -= dt;
                }
                if(timer <= 0.0f)
                {
                    FadeOut(dt);
                    FadedOut = true;
                }
            }

            if(player.Dead)
            {
                
                ResetPlatform();
            }
            
            //if(FadedOut)
            //{
                
            //}
        }

        void FadeOut(float dt)
        {
            //FADEOUT

            //Console.WriteLine("Reached end of 23sec");
            
            Console.WriteLine("FadeOutTime:" + FadeOutTime);
            if (FadeOutTime <= FadeOutTimer)
            {
                FadeOutTime += dt;
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
                //GameplayWrapper.DestroyEntity(entityID);
                Translation = new Vector2(9999, 9999);
            }

        }

        void ResetPlatform()
        {
            timer = 2.0f;
            FadeOutTime = 0.0f;
            hasStepped = false;
            FadedOut = false;
            Colour = new Vector4(1, 1, 1, 1);
            Translation = temp_translation;
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

