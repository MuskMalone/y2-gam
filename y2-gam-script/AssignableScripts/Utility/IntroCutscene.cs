
using Image;
using System;

namespace Object
{
    public class IntroCutscene : Entity
    {
        public int CutsceneIndex = 0;
        public int LastCutscene = 4;
        //public int CurrPanel = 1;
        public float PanelTime = 0.0f;
        //fade out
        //Counter for fade out
        public float FadeOutTime = 0.0f;
        //Max duration of fade out
        public float FadeOutTimer = 1.0f;
        //The Panel1Time in which fade out begins
        public float FadeOutBegins = 24.3f;

        //fade in counter
        private float PanelFadeInCounter = 0.0f;
        //Amount of time to fade in
        private float PanelFadeInTimer = 1.0f;
        private float normalizeTime = 0.0f;
        //Variable to hold opacity value 
        private float ColourValue = 0.0f;

        public string cutsceneTag;
        private bool FadedOut = false;

        //Time for each panels
        //private float Panel1Time = 6.0f;
        //private float Panel2Time = 9.5f;
        //private float Panel3Time = 14.7f;
        private float fadeoutbeginsC0 = 24.3f;
        private float fadeoutbeginsC1 = 43.5f;
        private float fadeoutbeginsC2 = 61.5f;
        private float fadeoutbeginsC3 = 89.0f;
        private float fadeoutbeginsC4 = 98.0f;

        /*  _________________________________________________________________________ */
        /*! Cutscene

        @return *this

        Default constructor for the Cutscene entity. Inherits from entity class.
        */
        public IntroCutscene() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Cutscene
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Cutscene entity.
        */
        public IntroCutscene(uint entityHandle) : base(entityHandle)
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
            //if (cutsceneTag != null)
            //{
            //    introCutscene = GameplayWrapper.FindEntityByName(cutsceneTag).As<IntroCutscene>();
            //}
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
            //PlayAudio("openingCutscene", 0);
            if (cutsceneTag == "c0p1")
            {
                FadeOutBegins = fadeoutbeginsC0;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 6.0f)
                {
                    //CurrPanel = 1
                    FadeIn(dt, FadeOutBegins);
                }
                else if (PanelTime >= FadeOutBegins)
                {
                    FadeOut(dt);
                    FadedOut = true;
                }
            }

            if (cutsceneTag == "c0p2")
            {
                FadeOutBegins = fadeoutbeginsC0;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 6.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 9.5f)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c0p3")
            {
                FadeOutBegins = fadeoutbeginsC0;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 9.5f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 14.7f)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c0p4")
            {
                FadeOutBegins = fadeoutbeginsC0;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 14.7f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < FadeOutBegins)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }
            
            //CUTSCENE PAGE 2
            if (cutsceneTag == "c1p1")
            {
                FadeOutBegins = fadeoutbeginsC1;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 25.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 34.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c1p2")
            {
                FadeOutBegins = fadeoutbeginsC1;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 34.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 41.0f)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c1p3")
            {
                FadeOutBegins = fadeoutbeginsC1;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 41.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < FadeOutBegins)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            //CUTSCENE PAGE 3
            //1 and 3 together
            if (cutsceneTag == "c2p1" || cutsceneTag == "c2p1p2")
            {
                FadeOutBegins = fadeoutbeginsC2;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 44.5f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 46.7f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }
            //2 alone
            if (cutsceneTag == "c2p2")
            {
                FadeOutBegins = fadeoutbeginsC2;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 46.7f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 50.3f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            //4 and 5 together
            if (cutsceneTag == "c2p3" || cutsceneTag == "c2p3p2")
            {
                FadeOutBegins = fadeoutbeginsC2;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 50.3f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 53.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            //6 alone
            if (cutsceneTag == "c2p4")
            {
                FadeOutBegins = fadeoutbeginsC2;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 53.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < FadeOutBegins)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            //PAGE 4
            if (cutsceneTag == "c3p1")
            {
                FadeOutBegins = fadeoutbeginsC3;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 62.6f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 73.9f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c3p2")
            {
                FadeOutBegins = fadeoutbeginsC3;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 73.9f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 80.8f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c3p3")
            {
                FadeOutBegins = fadeoutbeginsC3;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 80.8f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 84.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c3p4")
            {
                FadeOutBegins = fadeoutbeginsC3;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 84.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 85.1f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }


            if (cutsceneTag == "c3p5")
            {
                FadeOutBegins = fadeoutbeginsC3;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 85.1f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < FadeOutBegins)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (cutsceneTag == "c4p1")
            {
                FadeOutBegins = fadeoutbeginsC4;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 91.1f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < FadeOutBegins)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                        FadedOut = true;
                    }
                }
            }

            if (CutsceneIndex >= LastCutscene)
            {
                LoadScene("LevelSelect"); // Load the next scene if the last cutscene is reached
            }
        }

        void FadeIn(float dt, float fadeOutBegins)
        {
            //Time for audio to finish, after fade in, before fade out
            if (PanelFadeInCounter >= PanelFadeInTimer && PanelTime < fadeOutBegins)
            {
                Colour = new Vector4(1, 1, 1, 1);
                //Update the panel time
                //PanelTime += dt;
                Console.WriteLine("Panel1Time AFT2: " + PanelTime);

            }
            //Fading in 
            else if (PanelFadeInCounter < PanelFadeInTimer)
            {
                //Console.WriteLine("Panel1Time: " + PanelTime);
                //PanelTime += dt; // Update the panel time
                PanelFadeInCounter += dt;

                // Calculate the exact color value based on elapsed time, scaled to the range 0 to 1
                normalizeTime = PanelFadeInCounter / PanelFadeInTimer;
                ColourValue = normalizeTime * normalizeTime;
                ColourValue = Math.Min(ColourValue, 1.0f); // Ensure the value does not exceed 1

                Console.WriteLine("ColourValue: " + ColourValue);
                Colour = new Vector4(1, 1, 1, ColourValue); // Update the colour with the new value
            }
        }

        void FadeOut(float dt)
        {
            //FADEOUT

            //Console.WriteLine("Reached end of 23sec");
            FadeOutTime += dt;
            if (FadeOutTime <= FadeOutTimer)
            {
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

