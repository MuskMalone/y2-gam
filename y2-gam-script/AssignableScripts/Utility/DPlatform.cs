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
        public float FadeOutTimer = 45.0f;
        public float FadeOutClock = 45.0f;
        private float ColourValue = 0.0f;
        private bool FadedOut = false;
        private Vector2 temp_translation;
        private Vector2 temp_collider;
        public string platformTag;


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
            temp_collider = Collider;
            
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
            
            //Console.WriteLine("FadeOutTime:" + FadeOutTime);
            if (FadeOutTime <= FadeOutTimer)
            {
                //FadeOutClock -= dt;
                FadeOutTime += dt;
                float normalizedFadeOutTime = FadeOutTime / FadeOutTimer;
                float easeOut = normalizedFadeOutTime * (2 - normalizedFadeOutTime);
                ColourValue = 1.0f - easeOut;
                ColourValue = Math.Max(ColourValue, 0.0f);
                //Console.WriteLine("Colour Value: " + ColourValue);
                Colour = new Vector4(1, 1, 1, ColourValue);
            }
            //FINISH FADEOUT
            else
            {
                //GameplayWrapper.DestroyEntity(entityID);
                if (platformTag == "DPlatform1")
                {
                    Translation = new Vector2(9999, 9979);
                    Collider = new Vector2(9999, 9979);
                }
                else if (platformTag == "DPlatform2")
                {
                    Translation = new Vector2(9999, 9959);
                    Collider = new Vector2(9999, 9959);
                }
                else if (platformTag == "DPlatform3")
                {
                    Translation = new Vector2(9999, 9939);
                    Collider = new Vector2(9999, 9939);
                }
                else if (platformTag == "DPlatform4")
                {
                    Translation = new Vector2(9999, 9919);
                    Collider = new Vector2(9999, 9919);
                }
                else if (platformTag == "DPlatform5")
                {
                    Translation = new Vector2(9999, 9899);
                    Collider = new Vector2(9999, 9899);
                }
                else if (platformTag == "DPlatform6")
                {
                    Translation = new Vector2(9999, 9879);
                    Collider = new Vector2(9999, 9879);
                }
                else if (platformTag == "DPlatform7")
                {
                    Translation = new Vector2(9999, 9859);
                    Collider = new Vector2(9999, 9859);
                }
                else if (platformTag == "DPlatform8")
                {
                    Translation = new Vector2(9999, 9839);
                    Collider = new Vector2(9999, 9839);
                }
                else if (platformTag == "DPlatform9")
                {
                    Translation = new Vector2(9999, 9819);
                    Collider = new Vector2(9999, 9819);
                }
                else if (platformTag == "DPlatform10")
                {
                    Translation = new Vector2(9999, 9799);
                    Collider = new Vector2(9999, 9799);
                }
                else if (platformTag == "DPlatform11")
                {
                    Translation = new Vector2(9999, 9759);
                    Collider = new Vector2(9999, 9759);
                }
                else if (platformTag == "DPlatform12")
                {
                    Translation = new Vector2(9999, 9739);
                    Collider = new Vector2(9999, 9739);
                }
                else if (platformTag == "DPlatform13")
                {
                    Translation = new Vector2(9999, 9719);
                    Collider = new Vector2(9999, 9719);
                }
                else if (platformTag == "DPlatform14")
                {
                    Translation = new Vector2(9999, 9699);
                    Collider = new Vector2(9999, 9699);
                }
                else if (platformTag == "DPlatform15")
                {
                    Translation = new Vector2(9999, 9679);
                    Collider = new Vector2(9999, 9679);
                }
                else if (platformTag == "DPlatform16")
                {
                    Translation = new Vector2(9999, 9659);
                    Collider = new Vector2(9999, 9659);
                }
                else if (platformTag == "DPlatform17")
                {
                    Translation = new Vector2(9999, 9639);
                    Collider = new Vector2(9999, 9639);
                }
                
                //Collider = new Vector2(9999, 9999);
            }

        }

        void ResetPlatform()
        {
            //Console.WriteLine("HERE");
            timer = 0.0f;
            FadeOutTime = 0.0f;
            hasStepped = false;
            FadedOut = false;
            Colour = new Vector4(1, 1, 1, 1);
            Translation = temp_translation;
            Collider = temp_collider;
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

