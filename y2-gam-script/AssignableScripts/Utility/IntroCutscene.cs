﻿
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
        private bool done = false;
        
        //Time for each panels
        //private float Panel1Time = 6.0f;
        //private float Panel2Time = 9.5f;
        //private float Panel3Time = 14.7f;

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
            if (cutsceneTag == "c0p1")
            {
                FadeOutBegins = 24.3f;
                PanelTime += dt;
                if (PanelTime < 6.0f)
                {
                    //CurrPanel = 1
                    FadeIn(dt, FadeOutBegins);
                }
                else if (PanelTime >= FadeOutBegins)
                {
                    FadeOut(dt);
                }
            }

            if (cutsceneTag == "c0p2")
            {
                FadeOutBegins = 24.3f;
                if (PanelTime < 6.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime < 9.5f)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            if (cutsceneTag == "c0p3")
            {
                FadeOutBegins = 24.3f;
                if (PanelTime < 9.5f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime < 14.7f)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            if (cutsceneTag == "c0p4")
            {
                FadeOutBegins = 24.3f;
                if (PanelTime < 14.7f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime < FadeOutBegins)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }
            
            //CUTSCENE PAGE 2
            if (cutsceneTag == "c1p1")
            {
                FadeOutBegins = 44.0f;

                if (PanelTime < 25.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime > 25.0f && PanelTime < 34.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            if (cutsceneTag == "c1p2")
            {
                FadeOutBegins = 44.0f;
                if (PanelTime < 34.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime < 41.0f)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            if (cutsceneTag == "c1p3")
            {
                FadeOutBegins = 44.0f;
                if (PanelTime < 41.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime < FadeOutBegins)
                    {
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            //CUTSCENE PAGE 3
            if (cutsceneTag == "c2p1")
            {
                FadeOutBegins = 44.0f;

                if (PanelTime < 25.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime > 25.0f && PanelTime < 34.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            if (cutsceneTag == "c2p2")
            {
                FadeOutBegins = 44.0f;

                if (PanelTime < 25.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime > 25.0f && PanelTime < 34.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            if (cutsceneTag == "c2p3")
            {
                FadeOutBegins = 44.0f;

                if (PanelTime < 25.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime > 25.0f && PanelTime < 34.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
                    }
                }
            }

            if (cutsceneTag == "c2p4")
            {
                FadeOutBegins = 44.0f;

                if (PanelTime < 25.0f)
                {
                    PanelTime += dt;
                    Colour = new Vector4(1, 1, 1, 0);
                }
                else
                {
                    PanelTime += dt;
                    if (PanelTime > 25.0f && PanelTime < 34.0f)
                    {
                        //CurrPanel = 1
                        FadeIn(dt, FadeOutBegins);
                    }
                    else if (PanelTime >= FadeOutBegins)
                    {
                        FadeOut(dt);
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

