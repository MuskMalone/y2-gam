
using Image;
using System;

namespace Object
{
    public class OnetoTwoCutscene : Entity
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
        private float fadeoutbeginsC0 = 19.1f;
        private float fadeoutbeginsC1 = 30.0f;
        //private float fadeoutbeginsC2 = 61.5f;
        //private float fadeoutbeginsC3 = 89.0f;
        //private float fadeoutbeginsC4 = 98.0f;

        /*  _________________________________________________________________________ */
        /*! Cutscene

        @return *this

        Default constructor for the Cutscene entity. Inherits from entity class.
        */
        public OnetoTwoCutscene() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Cutscene
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Cutscene entity.
        */
        public OnetoTwoCutscene(uint entityHandle) : base(entityHandle)
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
            //PlayAudio("cutscene_transition.wav", -1, (int)ReverbCode.OFF);
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
                    if (PanelTime <= 16.0f)
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

            if (cutsceneTag == "c0p5")
            {
                FadeOutBegins = fadeoutbeginsC0;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 16.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime <= 17.1)
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

            if (cutsceneTag == "c0p6")
            {
                FadeOutBegins = fadeoutbeginsC0;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 17.1f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime <= FadeOutBegins)
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
                if (PanelTime < 20.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 22.0f)
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
                if (PanelTime < 22.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 24.0f)
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
                if (PanelTime < 24.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 26.0f)
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

            if (cutsceneTag == "c1p4")
            {
                FadeOutBegins = fadeoutbeginsC1;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime < 26.0f)
                {
                    //PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    //PanelTime += dt;
                    if (PanelTime < 28.0f)
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

            if (cutsceneTag == "c1p5")
            {
                FadeOutBegins = fadeoutbeginsC1;
                if (FadedOut == false)
                {
                    PanelTime += dt;
                }
                if (PanelTime <= 28.0f)
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
                if (FadedOut == true)
                {
                    LoadScene("Level2");
                }
            }

            //if (CutsceneIndex >= LastCutscene)
            //{
            //    LoadScene("LevelSelect"); // Load the next scene if the last cutscene is reached
            //}
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
            //StopAudio();
        }
    }
}

